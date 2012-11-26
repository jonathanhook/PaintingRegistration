/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2012 Jonathan Hook. All rights reserved.
 *
 * PaintingRegistration is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PaintingRegistration is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PaintingRegistration.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "JDHUtility/FileLocationUtility.h"
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/OpenGL.h"
#include "JDHUtility/WindowingUtils.h"
#include "Browser.h"
#include "Camera.h"
#include "PaintingRenderer.h"
#include "PaintingTracker.h"
#include "App.h"

namespace PaintingRegistration
{
    /* Public */
    App::App(unsigned int width, unsigned height, std::string resourcePath, std::string documentsPath)
    {        
        FileLocationUtility::setResourcePath(resourcePath);
        FileLocationUtility::setDocumentsPath(documentsPath);
        WindowingUtils::DEVICE_WINDOW_WIDTH = width;
        WindowingUtils::DEVICE_WINDOW_HEIGHT = height;
    
        fData = NULL;
        texture = new GLTexture("texture.jpg");
        tracker = new PaintingTracker();
        renderedOnce = false;
        
        initScene(width, height);
        initUI(width, height);
        initTextureHandle();
    }
    
    App::~App(void)
    {
        NDELETE(camera);
        NDELETE(browser);
        NDELETE(texture);
        NDELETE(tracker);
    }
    
    void App::render(void) const
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(uiMode == CAMERA)
        {
            updateTexture();
            camera->setCameraTexture(cameraTexture);
            camera->render();
        }
        else if(uiMode == BROWSER)
        {
            browser->render();
        }
    }
    
    void App::setLatestFrame(const unsigned char *fData, unsigned int fWidth, unsigned int fHeight)
    {
        this->fData = fData;
        this->fWidth = fWidth;
        this->fHeight = fHeight;
    }
    
    void App::train(void)
    {
        tracker->train("target.jpg");
    }
    
    void App::update(void)
    {

    }
    
    /* Private */
    void App::browser_Clicked(UIElement *e)
    {
        uiMode = CAMERA;
        registerEventHandler(camera);
        unregisterEventHandler(browser);
    }
    
    void App::camera_Clicked(UIElement *e)
    {
        if(tracker->compute(fData, fWidth, fHeight))
        {
            uiMode = BROWSER;
        
            browser->getPainting()->setMatrix(tracker->getGlMatrix());
            browser->getPainting()->setVertices(tracker->getVertices());
            
            unregisterEventHandler(camera);
            registerEventHandler(browser);
        }
    }
    
    void App::initScene(unsigned int width, unsigned height)
    {
        if(height == 0)
        {
            height = 1;
        }
        
        float ratio = (float)width / (float)height;
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, width, height);
        glOrtho(0.0f, 1.0f, 1.0f / ratio, 0.0f, -100.0f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    
    void App::initUI(unsigned int width, unsigned int height)
    {
        uiMode = CAMERA;
        
        camera = new Camera(Point2i(0, 0), Point2i(width, height), Point2i(640, 480), Point2i(1024, 1024));
        camera->setClickedCallback(MakeDelegate(this, &App::camera_Clicked));
        registerEventHandler(camera);
        
        browser = new Browser(Point2i(0, 0), Point2i(width, height));
        browser->setClickedCallback(MakeDelegate(this, &App::browser_Clicked));
    }
    
    void App::initTextureHandle(void)
    {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &cameraTexture);
        glDisable(GL_TEXTURE_2D);
    }
    
    void App::updateTexture(void) const
    {
        if(fData != NULL)
        {            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, cameraTexture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fWidth, fHeight, GL_BGRA, GL_UNSIGNED_BYTE, fData);
            glDisable(GL_TEXTURE_2D);
        }
    }
}