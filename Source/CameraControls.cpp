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
#include "JDHUtility/FileLocationUtility.h"
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/Ndelete.h"
#include "CameraControls.h"

namespace PaintingRegistration
{
    /* Public */
    CameraControls::CameraControls(const Point2i &position, const Point2i &dimensions) :
        UIElement(position, dimensions)
    {
        texture = new GLTexture("camera.png");
    }
    
    CameraControls::~CameraControls(void)
    {
        NDELETE(texture);
    }
    
    void CameraControls::render(void) const
    {
        float x = getSizef(position.getX());
		float y	= getSizef(position.getY());
		float h	= getSizef(dimensions.getY());
		float w	= getSizef(dimensions.getX());
        float tw = getSizef(texture->getWidth());
        float tx = (w / 2.0f) - (tw / 2.0f);
        
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
        glScalef(w, h, 1.0f);
        
        if(selected)
        {
            BLUE.use();
        }
        else
        {
            GREY.use();
        }
        
        GLPrimitives::getInstance()->renderSquare();
        
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
        glTranslatef(0.0, 0.0f, 0.0f);
        glScalef(tx, h, 1.0f);
        
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        GLPrimitives::getInstance()->renderSquare();
        
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
        glTranslatef(tx + tw, 0.0f, 0.0f);
        glScalef(tx, h, 1.0f);
        
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        GLPrimitives::getInstance()->renderSquare();
        
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
        glTranslatef(tx, 0.0f, 0.0f);
        glScalef(tw, h, 1.0f);
        
        texture->bind(GL_REPLACE);
        GLPrimitives::getInstance()->renderSquare();
        texture->unbind();

        glPopMatrix();
        
        glDisable(GL_BLEND);
        glPopMatrix();
    }
}