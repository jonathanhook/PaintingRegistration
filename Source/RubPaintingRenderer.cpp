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
#include "RubPaintingRenderer.h"

namespace PaintingRegistration
{
    /* Public */
    RubPaintingRenderer::RubPaintingRenderer(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims) :
        PaintingRegistration::PaintingRenderer(position, dims, frameDims, textureDims, targetDims)
    {
        inverse = new Matrixf(4, 4);
    }
    
    RubPaintingRenderer::~RubPaintingRenderer(void)
    {
        NDELETE(inverse);
    }
    
    static float _s_x = 1.0f;
    static float _s_y = 1.0f;
    
    void RubPaintingRenderer::fingerUpdated(const FingerEventArgs &e)
    {
        float wx = WindowingUtils::getWindowDimensions().getX();
        float wy = WindowingUtils::getWindowDimensions().getY();
        float ratio = wy / wx;
        float x = e.getX();
        float y = (e.getY() / ratio);
        
        float fx = (float)frameDimensions.getY() * x;
        float fy = (float)frameDimensions.getX() * y;
        
        Point3f p(fx, fy, 0.0f);
        p = inverse->transform(p);
        
        float scale = p.getZ() + 1.0f;
        p.scale(scale, scale);
        
        _s_x = p.getX() / ((float)targetDimensions.getX());
        _s_y = p.getY() / ((float)targetDimensions.getY());
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
        
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(5.0f);
        
        UIElement::BLUE.use();
        GLPrimitives::getInstance()->renderSquareOutline();
        
        glTranslatef(_s_x - 0.075f, _s_y - 0.075f, 0.0f);
        glScalef(0.15f, 0.15f, 1.0f);
        
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        GLPrimitives::getInstance()->renderSquareOutline();
        
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glPopMatrix();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
    
    void RubPaintingRenderer::setMatrix(const Matrixf *matrix)
    {
        PaintingRenderer::setMatrix(matrix);
        Matrixf result = matrix->getInverse();
        inverse->setValues(result);
    }
    


}