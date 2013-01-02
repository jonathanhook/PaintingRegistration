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
#include <vector>
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/Point2i.h"

using namespace JDHUtility;

namespace PaintingRegistration
{
    class TextureBlock
    {
    public:
        TextureBlock(std::string format, unsigned int start, unsigned int end);
        ~TextureBlock(void);
        
        virtual void bind(void) const = 0;
        const Point2i &getDimensions(void) const;
        virtual void unbind(void) const = 0;
        
    protected:
        Point2i dimensions;
        unsigned int start;
        unsigned int end;
        std::string format;
        
        void loadAll(void);
        virtual void loadTexture(unsigned int i) = 0;
    };
}