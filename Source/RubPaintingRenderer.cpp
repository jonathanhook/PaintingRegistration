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
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/WindowingUtils.h"
#include "RubTextureBlock.h"
#include "RubPaintingRenderer.h"

namespace PaintingRegistration
{
    /* Static */
    const float RubPaintingRenderer::MAX_CURSOR_SIZE = 100.0f;
    
    /* Public */
    RubPaintingRenderer::RubPaintingRenderer(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims) :
        PaintingRegistration::PaintingRenderer(position, dims, frameDims, textureDims, targetDims)
    {
        textureBlock = new RubTextureBlock(TEXTURE_FILENAME_FORMAT, 1, NUM_TEXTURES);
    }
    
    RubPaintingRenderer::~RubPaintingRenderer(void)
    {
    }
    
    void RubPaintingRenderer::fingerUpdated(const FingerEventArgs &e)
    {
        float wx = WindowingUtils::getWindowDimensions().getX();
        float wy = WindowingUtils::getWindowDimensions().getY();
        float ratio = wy / wx;
        float x = e.getX();
        float y = (e.getY() / ratio);
        
        float fx = (float)frameDimensions.getY() * x;
        float fy = (float)frameDimensions.getX() * y;
        
        float a = fx;
        float b = fy;
        float c = 1.0f;
        
        const float *matrix = inverse->getPtr();
        float vx = matrix[0] * a + matrix[1] * b + matrix[2] * c;
        float vy = matrix[3] * a + matrix[4] * b + matrix[5] * c;
        float vz = matrix[6] * a + matrix[7] * b + matrix[8] * c;
        vx /= vz;
        vy /= vz;
        
        float nx = vx / ((float)targetDimensions.getX());
        float ny = vy / ((float)targetDimensions.getY());

        ((RubTextureBlock *)textureBlock)->update(nx, ny, 0.1f);
    }
    
    void RubPaintingRenderer::renderPerspective(void) const
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0f, frameDimensions.getY(), frameDimensions.getX(), 0.0f, -100.0f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMultMatrixf((float *)matrix->getPtr());
        glScalef(targetDimensions.getX(), targetDimensions.getY(), 1.0f);
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
    
    void RubPaintingRenderer::setMatrixInverse(const Matrixf *inverse)
    {
        this->inverse = inverse;
    }
}