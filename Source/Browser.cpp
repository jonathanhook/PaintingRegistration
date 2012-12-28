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
#include "JDHUtility/GLPrimitives.h"
#include "JDHUtility/GLTexture.h"
#include "PaintingRenderer.h"
#include "BrowserControls.h"
#include "Browser.h"

namespace PaintingRegistration
{
    /* Public */
    Browser::Browser(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims) :
        UIElement(position, dims)
    {
        clicked = NULL;
        texture = new GLTexture("frame.png");
    }
    
    Browser::~Browser(void)
    {
        NDELETE(painting);
        NDELETE(texture);
    }
    
    bool Browser::contains(const FingerEventArgs &e) const
    {
        return false;
    }
    
    PaintingRenderer *Browser::getPainting(void)
    {
        return painting;
    }
    
    void Browser::render(void) const
    {
        painting->render();
        
        float x = getSizef(position.getX());
		float y	= getSizef(position.getY());
		float w	= getSizef(dimensions.getX());
        float fh = getSizef(dimensions.getY() - UIElement::CONTROL_BAR_HEIGHT);
        
        glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(w, fh, 1.0f);
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        texture->bind(GL_REPLACE);
        GLPrimitives::getInstance()->renderSquare();
        texture->unbind();
        
        glDisable(GL_BLEND);
        glPopMatrix();
        
        controls->render();
    }
    
    /* Private */
    void Browser::controls_Clicked(UIElement *e)
    {
        if(clicked != NULL)
        {
            clicked(e);
        }
    }
}