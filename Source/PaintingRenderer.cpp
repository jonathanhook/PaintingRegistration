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
#include "JDHUtility/GLPrimitives.h"
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/GLVbo.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/OpenGL.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/WindowingUtils.h"
#include "UIElement.h"
#include "PaintingRenderer.h"

namespace PaintingRegistration
{
    /* Public */
    PaintingRenderer::PaintingRenderer(void)
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
        
        vbo = new GLVbo(GL_TRIANGLE_STRIP, GL_DYNAMIC_DRAW, data, 4, textureData);
    }
    
    PaintingRenderer::~PaintingRenderer(void)
    {
        NDELETE(vbo);
    }
    
    void PaintingRenderer::render(void) const
    {
        float w = WindowingUtils::getWindowDimensions().getX();
        float h = WindowingUtils::getWindowDimensions().getY();
        float ratio = h / w;
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScalef(1.0f, ratio, 1.0f);
        
        UIElement::GREY.use();
        GLPrimitives::getInstance()->renderSquare();
        
        currentImage->bind(GL_REPLACE);
        vbo->render();
        currentImage->unbind();
        
        glPopMatrix();
    }
    
    void PaintingRenderer::setCurrentImage(GLTexture *currentImage)
    {
        this->currentImage = currentImage;
    }
    
    void PaintingRenderer::setVertices(const Point2f *vertices)
    {
        this->vertices = vertices;
        
        GLfloat data[12] =
        {
            vertices[0].getX(), vertices[0].getY(), 0.0f,
            vertices[3].getX(), vertices[3].getY(), 0.0f,
            vertices[1].getX(), vertices[1].getY(), 0.0f,
            vertices[2].getX(), vertices[2].getY(), 0.0f,
		};
        
        printf("(%2.2f, %2.2f)\t(%2.2f, %2.2f)\r\n", vertices[0].getX(), vertices[0].getY(), vertices[1].getX(), vertices[1].getY());
        printf("(%2.2f, %2.2f)\t(%2.2f, %2.2f)\r\n", vertices[3].getX(), vertices[3].getY(), vertices[2].getX(), vertices[2].getY());
        
        vbo->update(GL_TRIANGLE_STRIP, GL_DYNAMIC_DRAW, data, 4);
    }
}