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
#include <JDHUtility/GLTexture.h>
#include <JDHUtility/GLVbo.h>
#include <JDHUtility/Ndelete.h>
#include <JDHUtility/OpenGL.h>
#include "Renderer.h"

namespace PaintingRegistration
{
    /* Public */
    Renderer::Renderer(void)
    {
        initVbo();
    }
    
    Renderer::~Renderer(void)
    {

    }
    
    void Renderer::initScene(void) const
    {
        
    }
    
    void Renderer::render(void) const
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//        if(currentImage->isTexture())
//        {
//            currentImage->bind();
            vbo->render();
//            currentImage->unbind();
//        }
    }
    
    void Renderer::setCurrentImage(GLTexture *currentImage)
    {
        this->currentImage = currentImage;
    }
    
    void Renderer::setHomography(const cv::Mat &homography)
    {
        this->homography = homography;
    }
    
    /* Private */
    void Renderer::initVbo(void)
    {
        GLfloat data[12] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f
		};
        
        GLfloat textureData[8] =
		{
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};
        
        vbo = new GLVbo(GL_TRIANGLE_STRIP, GL_STATIC_DRAW, data, 4, textureData);
    }
}