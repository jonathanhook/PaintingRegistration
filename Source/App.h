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
#include <string>
#include <opencv2/opencv.hpp>
#include "MultiTouchEvents/MultiTouchEventManager.h"

using namespace MultiTouchEvents;

namespace JDHUtility
{
    class GLTexture;
}
using namespace JDHUtility;

namespace PaintingRegistration
{
    class SlideBrowser;
    class RubBrowser;
    class Camera;
    class Overlay;
    class PaintingTracker;
    class UIElement;
    
    class App :
        public MultiTouchEventManager
    {
    public:
        enum UIMode { CAMERA, BROWSER };
        
        App(unsigned int width, unsigned int height, unsigned int cameraWidth, unsigned int cameraHeight, std::string resourcePath, std::string documentsPath, std::string textureFilenameFormat, int numTextures);
        ~App(void);
        
        bool getIsProcessing(void) const;
        
        void render(void) const;
        void setLatestFrame(const unsigned char *fData);
        void train(void);
        void update(void);
        
    private:
        //static const unsigned int TEXTURE_DIM = 512;
        
        SlideBrowser *slideBrowser;
        RubBrowser *rubBrowser;
        Camera *camera;
        GLuint cameraTexture;
        Overlay *processing;
        PaintingTracker *tracker;
        GLTexture *texture;
        UIMode uiMode;
        bool isProcessing;
        unsigned int cameraWidth;
        unsigned int cameraHeight;
        const unsigned char *fData;
        bool browserMode;
        bool resetBrowser;
        unsigned int textureDim;
        
        void browser_Clicked(UIElement *e);
        void camera_Clicked(UIElement *e);
        void initScene(unsigned int width, unsigned int height);
        void initUI(const std::string &textureFilenameFormat, unsigned int width, unsigned int height, int numTextures);
        void initTextureHandle(void);
        void tracker_Completed(bool result);
        void updateTexture(void) const;
        void setBrowser(bool mode);
    };
}