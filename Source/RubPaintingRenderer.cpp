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
#include "JDHUtility/CrossPlatformTime.h"
#include "JDHUtility/GLPrimitives.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/WindowingUtils.h"
#include "RubTextureBlock.h"
#include "RubPaintingRenderer.h"

namespace PaintingRegistration
{
    /* Static */
    const float RubPaintingRenderer::CURSOR_SIZE = 50.0f;
    const int RubPaintingRenderer::UPDATE_RATE = 10;
    
    /* Public */
    RubPaintingRenderer::RubPaintingRenderer(const std::string textureFilenameFormat, const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims, bool mode) :
        PaintingRegistration::PaintingRenderer(textureFilenameFormat, position, dims, frameDims, textureDims, targetDims)
    {
        this->mode = mode;
        
        textureBlock = new RubTextureBlock(textureFilenameFormat, 1, NUM_TEXTURES);
    }
    
    RubPaintingRenderer::~RubPaintingRenderer(void)
    {
    }
    
    void RubPaintingRenderer::fingerAdded(const FingerEventArgs &e)
    {
        lx = e.getX();
        ly = e.getY();
        averageSpeed = 0.0f;
    }
    
    void RubPaintingRenderer::fingerUpdated(const FingerEventArgs &e)
    {
        float dx = fabs(e.getX() - lx);
        float dy = fabs(e.getY() - ly);
        float d = sqrt((dx * dx) + (dy * dy));
        averageSpeed = d * 0.25f + (averageSpeed * (1.0f - 0.25f));
        
        lx = e.getX();
        ly = e.getY();
        
        Vector2f dims = WindowingUtils::getWindowDimensions();
        float wx = dims.getX();
        float wy = dims.getY();
        float ratio = wy / wx;
        float x = e.getX();
        float y = (e.getY() / ratio);
        
        float w	= (float)wx;
        float h	= (float)wy;
        float fh = (float)h - UIElement::CONTROL_BAR_HEIGHT;
        float yr = fh / h;
    
        float r = (float)frameDimensions.getX() / (float)frameDimensions.getY();
        float fw = getSizef(fh) / r;
        
        if(fw < 1.0f)
        {
            fw = 1.0f;
            fh = w * r;
            yr = fh / h;
        }

        x /= fw;
        y /= yr;
    
        float fx = (float)frameDimensions.getY() * x;
        float fy = (float)frameDimensions.getX() * y;
        
        float a = fx;
        float b = fy;
        float c = 1.0f;
        
        const float *iPtr = inverse->getPtr();
        float vx = iPtr[0] * a + iPtr[1] * b + iPtr[2] * c;
        float vy = iPtr[3] * a + iPtr[4] * b + iPtr[5] * c;
        float vz = iPtr[6] * a + iPtr[7] * b + iPtr[8] * c;
        vx /= vz;
        vy /= vz;
        
        float nx = vx / ((float)targetDimensions.getX());
        float ny = vy / ((float)targetDimensions.getY());
        float nc = CURSOR_SIZE / (float)frameDimensions.getX();
        
        ((RubTextureBlock *)textureBlock)->update(nx, ny, nc, mode, averageSpeed);
    }
    
    void RubPaintingRenderer::reset(void)
    {
        ((RubTextureBlock *)textureBlock)->reset();
    }

    void RubPaintingRenderer::setMatrixInverse(const Matrixf *inverse)
    {
        this->inverse = inverse;
    }
    
    void RubPaintingRenderer::setMode(bool mode)
    {
        this->mode = mode;
    }
    
    void  RubPaintingRenderer::setPaintingArea(float paintingArea)
    {
        this->paintingArea = paintingArea;
    }
}