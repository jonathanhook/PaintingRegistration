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
#include "SlideTextureBlock.h"
#include "TextureBlock.h"
#include "UIElement.h"
#include "PaintingRenderer.h"

namespace PaintingRegistration
{
    /* Constants */
    const std::string PaintingRenderer::TEXTURE_FILENAME_FORMAT = "%i.jpg";
    const unsigned int PaintingRenderer::NUM_TEXTURES = 24;
    
    /* Public */
    PaintingRenderer::PaintingRenderer(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims) :
        UIElement(position, dims)
    {
        this->frameDimensions = frameDims;
        this->targetDimensions = targetDims;
        
        GLfloat camData[12] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f
		};
        
        float u0 = 0.0f;
        float u1 = (float)frameDims.getX() / (float)textureDims.getX();
        float v0 = 0.0f;
        float v1 = (float)frameDims.getY() / (float)textureDims.getY();
        GLfloat camTextureData[8] =
		{
			u0, v1,
			u1, v1,
			u0, v0,
			u1, v0
		};
        
        camVbo = new GLVbo(GL_TRIANGLE_STRIP, GL_STATIC_DRAW, camData, 4, camTextureData);
    }
    
    PaintingRenderer::~PaintingRenderer(void)
    {
        NDELETE(camVbo);
        NDELETE(textureBlock);
    }
    
    const Matrixf *PaintingRenderer::getMatrix(void) const
    {
        return matrix;
    }
    
    void PaintingRenderer::render(void) const
    {
        renderCameraImage();
        renderPerspective();
    }
    
    void PaintingRenderer::setCameraTexture(GLuint cameraTexture)
    {
        this->cameraTexture = cameraTexture;
    }
    
    void PaintingRenderer::setMatrix(const Matrixf *matrix)
    {
        this->matrix = matrix;
    }
    
    void PaintingRenderer::setPosition(float position)
    {
        // HACK: shows problem in architecture...
        ((SlideTextureBlock *)textureBlock)->setPosition(position);
    }
    
    /* Private */
    void PaintingRenderer::renderCameraImage(void) const
    {
        float x = getSizef(position.getX());
		float y	= getSizef(position.getY());
		float w	= getSizef(dimensions.getX());
        float h	= getSizef(dimensions.getY());
        float r = (float)frameDimensions.getX() / (float)frameDimensions.getY();
        
        float fh = getSizef(dimensions.getY() - UIElement::CONTROL_BAR_HEIGHT);
        float fw = fh / r;
        float ft = (fw - w) / 2.0f;
        
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x - ft, y, 0.0f);
		glScalef(fw, fh, 1.0f);
        
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, cameraTexture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        
        camVbo->render();
        
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        
        GLPrimitives::getInstance()->renderSquare();
        
        glDisable(GL_BLEND);
        glPopMatrix();
    }
    
    void PaintingRenderer::renderPerspective(void) const
    {
		float h	= getSizef(dimensions.getY());
		float w	= getSizef(dimensions.getX());
        float r = (float)frameDimensions.getX() / (float)frameDimensions.getY();
        float fh = getSizef(dimensions.getY() - UIElement::CONTROL_BAR_HEIGHT);
        float a = fh / h;
        float fw = fh / r;
        float ft = (fw - w) / 2.0f;
    
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0f, frameDimensions.getY(), frameDimensions.getX(), 0.0f, -100.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMultMatrixf((float *)matrix->getPtr());
        glScalef(targetDimensions.getX(), targetDimensions.getY(), 1.0f);
        glTranslatef(-ft, 0.0f, 0.0f);
        glScalef(fw, a, 1.0f);
        
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        textureBlock->bind();
        GLPrimitives::getInstance()->renderSquare();
        textureBlock->unbind();
        
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(5.0f);
        
        UIElement::BLUE.use();
        GLPrimitives::getInstance()->renderSquareOutline();
        
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glPopMatrix();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
}