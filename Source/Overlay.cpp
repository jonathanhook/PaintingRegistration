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
#include "JDHUtility/Ndelete.h"
#include "Overlay.h"

namespace PaintingRegistration
{
    /* Public */
    Overlay::Overlay(std::string path, bool isVisible, const Point2i &position, const Point2i &dimensions) :
        UIElement(position, dimensions)
    {
        this->isVisible = isVisible;
        
        texture = new GLTexture(path);
    }
    
    Overlay::~Overlay(void)
    {
        NDELETE(texture);
    }
    
    bool Overlay::getIsVisible(void) const
    {
        return isVisible;
    }
    
    void Overlay::render(void) const
    {
        if(isVisible)
        {
            float x = getSizef(position.getX());
            float y	= getSizef(position.getY());
            float w	= getSizef(dimensions.getX());
            float h	= getSizef(dimensions.getY());
            
            float tw = getSizef(texture->getWidth());
            float th = getSizef(texture->getHeight());
            float tx = (w / 2.0f) - (tw / 2.0f);
            float ty = (h / 2.25f) - (th / 2.0f);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glTranslatef(x, y, 0.0f);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glScalef(w, h, 1.0f);
            
            glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
            GLPrimitives::getInstance()->renderSquare();
            
            glPopMatrix();
            
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(tx, ty, 0.0f);
            glScalef(tw, th, 1.0f);
        
            texture->bind(GL_REPLACE);
            GLPrimitives::getInstance()->renderSquare();
            texture->unbind();
        
            glPopMatrix();
            
            glDisable(GL_BLEND);
            glPopMatrix();
        }
    }
    
    void Overlay::setIsVisible(bool isVisible)
    {
        this->isVisible = isVisible;
    }
}