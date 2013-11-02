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
        TextureBlock(const std::string &format, unsigned int start, unsigned int end);
        ~TextureBlock(void);
        
        void bind(void) const;
        const Point2i &getDimensions(void) const;
        void unbind(void) const;
        
    protected:
        static std::vector<unsigned char *> textures;
        static bool texturesLoaded;
        static int start;
        static int end;
        static Point2i dimensions;
        static unsigned int bpp;
        static GLuint texture;
        
        static void loadAll(const std::string &format);
        static void loadTexture(unsigned int i, const std::string &format);
        static void initTexture(void);
    };
}