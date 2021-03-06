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
#pragma once

#include "UIElement.h"

namespace JDHUtility
{
    class GLTexture;
}

namespace PaintingRegistration
{
    class BrowserControls;
    class PaintingRenderer;
    
    class Browser :
        public UIElement
    {
    public:
        Browser(const Point2i &position, const Point2i &dims, const Point2i &frameDims, const Point2i &textureDims, const Point2i &targetDims, int numTextures);
        ~Browser(void);
        
        virtual bool contains (const FingerEventArgs &e) const;
        PaintingRenderer *getPainting(void);
        void render(void) const;
        virtual void reset(void) = 0;

    protected:
        BrowserControls *controls;
        PaintingRenderer *painting;
        
        void controls_Clicked(UIElement *e);
    };
}