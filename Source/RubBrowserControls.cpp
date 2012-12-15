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
#include "ExitButton.h"
#include "RubBrowserControls.h"

namespace PaintingRegistration
{
    /* Public */
    RubBrowserControls::RubBrowserControls(const Point2i &position, const Point2i &dimensions) :
        BrowserControls(position, dimensions)
    {
        int px = position.getX();
        int py = position.getY();
        int dx = dimensions.getX() - CONTROL_BAR_HEIGHT;
        int dy = dimensions.getY();
        
        bx = getSizef(dx);
        by = getSizef(dy);
        
        px += dx;
        dy = CONTROL_BAR_HEIGHT;
        
        exit = new ExitButton(Point2i(px, py), Point2i(dy, dy));
        exit->setClickedCallback(MakeDelegate(this, &RubBrowserControls::exit_Clicked));
        registerEventHandler(exit);
    }
    
    RubBrowserControls::~RubBrowserControls(void)
    {
    }
    
    void RubBrowserControls::render(void) const
    {
        float x = getSizef(position.getX());
		float y	= getSizef(position.getY());
        
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(bx, by, 1.0f);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        
        GLPrimitives::getInstance()->renderSquare();
        
        glPopMatrix();
        
        BrowserControls::render();
    }
}