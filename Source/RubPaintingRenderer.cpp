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
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/WindowingUtils.h"
#include "RubPaintingRenderer.h"

namespace PaintingRegistration
{
    /* Public */
    RubPaintingRenderer::RubPaintingRenderer(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &layerDims) :
        PaintingRegistration::PaintingRenderer(position, dims, frameDims, textureDims, layerDims)
    {
        inverse = new Matrixf(4, 4);
    }
    
    RubPaintingRenderer::~RubPaintingRenderer(void)
    {
        NDELETE(inverse);
    }
    
    void RubPaintingRenderer::fingerUpdated(const FingerEventArgs &e)
    {
        // calculate inverse homography when matrix changed
        
        // project point through inverse homography (z=0)
        
        // thats the answer!
        
        
        
        
        
        
        float x = 0.0f;//e.getX();
        float y = 0.0f;//e.getY();
        
        printf("\n");
        printf("\n");
        printf("%2.2f\t%2.2f\n", x, y);
        
        float rx, ry, rz;
        //transform(matrix, x, y, 0.0f, rx, ry, rz);
        
        printf("%2.2f\t%2.2f\t%2.2f\n", rx, ry, rz);
        
        /*
        float wx = WindowingUtils::getWindowDimensions().getX();
        float wy = WindowingUtils::getWindowDimensions().getY();
        float r = wy / wx;
        
        float x = e.getX() * wx;
        float y = e.getY() * (wy / r);
        float z;
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT16_OES, GL_FLOAT, &z);
        
        GLdouble ox, oy, oz;
        //gluUnProject(x, y, z, (GLdouble *)modelview, (GLdouble *)projection, viewport, &ox, &oy, &oz);
        
        
        GLdouble m[16];
        GLdouble p[16];
        GLint v[4];
        
        gluUnProject(x, y, z, m, p, v, &ox, &oy, &oz);
        
  
        printf("%2.2f\t%2.2f\t%2.2f\n", ox, oy, oz);
        */
        // the result divided by the dimensions of the picture is what we want
    }
    
    void RubPaintingRenderer::renderPerspective(void) const
    {
    }
    
    /* Private */
    void RubPaintingRenderer::calculateInverse(float *m, float *i)
    {
        
    }
    
    float RubPaintingRenderer::calculateDeterminant(float *m)
    {
        
        
        
        return 0.0f;
    }
    
    void RubPaintingRenderer::transform(float *matrix, float x, float y, float z, float &rx, float &ry, float &rz)
    {
        float a = x;
		float b = y;
		float c = z;
		float d = 1.0f;
        
		rx = matrix[0] * a + matrix[4] * b + matrix[8]  * c + matrix[12] * d;
		ry = matrix[1] * a + matrix[5] * b + matrix[9]  * c + matrix[13] * d;
		rz = matrix[3] * a + matrix[6] * b + matrix[10] * c + matrix[14] * d;
    }
    
    /*
     float rx, ry, rz;
     
     transform(0.0f, 0.0f, 0.0f, rx, ry, rz);
     printf("%2.2f %2.2f\t\t\t\t", rx, ry);
     
     transform(721.0f, 0.0f, 0.0f, rx, ry, rz);
     printf("%2.2f %2.2f\n", rx, ry);
     
     transform(0.0f, 1060.0f, 0.0f, rx, ry, rz);
     printf("%2.2f %2.2f\t\t\t\t", rx, ry);
     
     transform(721.0f, 1060.0f, 0.0f, rx, ry, rz);
     printf("%2.2f %2.2f\n\n\n\n", rx, ry);*/

}