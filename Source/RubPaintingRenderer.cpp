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
    const float RubPaintingRenderer::CURSOR_SIZE = 75.0f;
    
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
        Vector2f dims = WindowingUtils::getWindowDimensions();
        float wx = dims.getX();
        float wy = dims.getY();
        float ratio = wy / wx;
        float x = e.getX();
        float y = (e.getY() / ratio);
        
        float h	= (float)wy;
        float fh = (float)h - UIElement::CONTROL_BAR_HEIGHT;
        float yr = fh / h;
        
        float r = (float)frameDimensions.getX() / (float)frameDimensions.getY();
        float fw = getSizef(fh) / r;
        float xr = ((fw - 1.0f) / 2.0f);
        
        x += xr;
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
        
        ((RubTextureBlock *)textureBlock)->update(nx, ny, nc);
    }
    
    void RubPaintingRenderer::reset(void)
    {
        ((RubTextureBlock *)textureBlock)->reset();
    }

    void RubPaintingRenderer::setMatrixInverse(const Matrixf *inverse)
    {
        this->inverse = inverse;
    }
    
    void  RubPaintingRenderer::setPaintingArea(float paintingArea)
    {
        this->paintingArea = paintingArea;
    }
}