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
#include "JDHUtility/GLTexture.h"
#include "ExitButton.h"
#include "Slider.h"
#include "SlideBrowserControls.h"

namespace PaintingRegistration
{
    /* Public */
    SlideBrowserControls::SlideBrowserControls(const Point2i &position, const Point2i &dimensions) :
        BrowserControls(position, dimensions)
    {
        int px = position.getX();
        int py = position.getY();
        int dx = dimensions.getX() - CONTROL_BAR_HEIGHT;
        int dy = dimensions.getY();
        
        slider = new Slider(1.0f, Point2i(px, py), Point2i(dx, dy));
        slider->setValueChangedCallback(MakeDelegate(this, &SlideBrowserControls::slider_ValueChanged));
        registerEventHandler(slider);
        
        px += slider->getDimensions().getX();
        dy = CONTROL_BAR_HEIGHT;
        
        exit = new ExitButton(Point2i(px, py), Point2i(dy, dy));
        exit->setClickedCallback(MakeDelegate(this, &SlideBrowserControls::exit_Clicked));
        registerEventHandler(exit);
    }
    
    SlideBrowserControls::~SlideBrowserControls(void)
    {
        NDELETE(slider);
    }
    
    void SlideBrowserControls::render(void) const
    {
        BrowserControls::render();
        slider->render();
    }
    
    void SlideBrowserControls::setPositionChangedCallback(PositionChangedCallback positionChanged)
    {
        this->positionChanged = positionChanged;
    }
    
    /* Private */    
    void SlideBrowserControls::slider_ValueChanged(float value)
    {
        if(positionChanged != NULL)
        {
            positionChanged(value);
        }
    }
}