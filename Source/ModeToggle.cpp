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
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/GLPrimitives.h"
#include "ModeToggle.h"

namespace PaintingRegistration
{
    /* Public */
    ModeToggle::ModeToggle(const Point2i &position, const Point2i &dimensions, const std::string &t0, const std::string &t1) :
        UIElement(position, dimensions)
    {
        texture0 = new GLTexture(t0);
        texture1 = new GLTexture(t1);
        isToggled = false;
    }
    
    ModeToggle::~ModeToggle(void)
    {
        NDELETE(texture0);
        NDELETE(texture1);
    }
    
    bool ModeToggle::getIsToggled(void) const
    {
        return isToggled;
    }
    
    void ModeToggle::fingerRemoved(const FingerEventArgs &e)
	{
		if(captured && capturedId == e.getId())
		{
			captured	= false;
			selected	= false;
            
			if(released != NULL)
			{
				released(this);
			}
            
			if(clicked != NULL && contains(e))
			{
				clicked(this);
			}
            
            isToggled = !isToggled;
		}
	}
    
    void ModeToggle::render(void) const
    {
        float x = getSizef(position.getX());
		float y	= getSizef(position.getY());
		float h	= getSizef(dimensions.getY());
		float w	= getSizef(dimensions.getX());
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
        glScalef(w, h, 1.0f);    
        
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        
        if(selected)
        {
            BLUE.use();
        }
        else
        {
            GREY.use();
        }
        
        GLPrimitives::getInstance()->renderSquare();
        
        const GLTexture *t = isToggled ? texture0 : texture1;
        
        t->bind(GL_REPLACE);
        GLPrimitives::getInstance()->renderSquare();
        t->unbind();
        
        glDisable(GL_BLEND);
        glPopMatrix();
        
        
        /*
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
        glPopMatrix();*/
    }
}