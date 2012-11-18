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
#include <JDHUtility/GLMatrixf.h>
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
        transform = new GLMatrixf();
        initVbo();
    }
    
    Renderer::~Renderer(void)
    {
        NDELETE(transform);
    }
    
    void Renderer::initScene(void) const
    {
        
    }
    
    void Renderer::render(void) const
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(transform->getMatrix());
        
        /*
        float *m = transform->getMatrix();
        printf("%2.1f\t%2.1f\t%2.1f\t%2.1f\r\n", m[0], m[1], m[2], m[3]);
        printf("%2.1f\t%2.1f\t%2.1f\t%2.1f\r\n", m[4], m[5], m[6], m[7]);
        printf("%2.1f\t%2.1f\t%2.1f\t%2.1f\r\n", m[8], m[9], m[10], m[11]);
        printf("%2.1f\t%2.1f\t%2.1f\t%2.1f\r\n", m[12], m[13], m[14], m[15]);
        printf("-----\r\n");
         */
        

        currentImage->bind();
        vbo->render();
        currentImage->unbind();
        
        glPopMatrix();
    }
    
    void Renderer::setCurrentImage(GLTexture *currentImage)
    {
        this->currentImage = currentImage;
    }
    
    void Renderer::setHomography(const cv::Mat &homography)
    {
        uchar *m = homography.data;
        
        if(homography.cols == 3 && homography.rows == 3)
        {
         printf("%d\t%d\t%d\r\n", m[0], m[1], m[2]);
         printf("%d\t%d\t%d\r\n", m[3], m[4], m[5]);
         printf("%d\t%d\t%d\r\n", m[6], m[7], m[8]);
         printf("-----\r\n");
        }
        
        
        for(unsigned int i = 0; i < homography.rows; i ++)
        {
            for(int j = 0; j < homography.cols; j++)
            {
                uchar h = homography.at<uchar>(i, j);
                float t = (float)h / 255.0f;
                
                transform->setValue(t, j, i);
            }
        }
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