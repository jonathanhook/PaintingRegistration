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
#include "SlideTextureBlock.h"

namespace PaintingRegistration
{
    /* Public */
    SlideTextureBlock::SlideTextureBlock(std::string format, unsigned int start, unsigned int end, float position) :
        TextureBlock(format, start, end)
    {
        this->position = position;
        
        setPosition(position);
        loadAll();
    }
    
    SlideTextureBlock::~SlideTextureBlock(void)
    {
        
    }
    
    void SlideTextureBlock::bind(void) const
    {
        const GLTexture &t = textures[currentTexture];
        t.bind(GL_REPLACE);
    }
    
    void SlideTextureBlock::setPosition(float position)
    {
        this->position = position;
        currentTexture = (unsigned int)(position * (float)(end - start));
    }
    
    void SlideTextureBlock::unbind(void) const
    {
        const GLTexture &t = textures[currentTexture];
        t.unbind();
    }
    
    /* Private */
    void SlideTextureBlock::loadTexture(unsigned int i)
    {
        char buffer[1024];
        sprintf(buffer, format.c_str(), i);
        
        GLTexture t(buffer);
        dimensions.setX(t.getWidth());
        dimensions.setY(t.getHeight());
        textures.push_back(t);
    }
}