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
#include "TextureBlock.h"

namespace PaintingRegistration
{
    class RubTextureBlock :
        public TextureBlock
    {
    public:
        RubTextureBlock(std::string format, unsigned int start, unsigned int end);
        ~RubTextureBlock(void);
        
        void bind(void) const;
        void unbind(void) const;
        void update(float x, float y, float cursorSize);
        
    private:
        static const float RUB_DECREMENT;
        
        float *mask;
        GLuint texture;
        std::vector<unsigned char *> textures;
        unsigned int bpp;

        void initMask(void);
        void initTexture(void);
        void loadTexture(unsigned int i);
        bool isWithinPainting(int x, int y) const;
        void updatePixels(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    };
}