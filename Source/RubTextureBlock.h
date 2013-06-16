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
        
        void reset(void);
        void update(float x, float y, float cursorSize, bool mode, float fingerSpeed);
        
    private:
        static const float RUB_DECREMENT;
        static const float SPEED_MULTIPLIER;
        static const float MAX_SPEED;
        
        float *mask;

        void initMask(void); 
        bool isWithinPainting(int x, int y) const;
        void updatePixels(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    };
}