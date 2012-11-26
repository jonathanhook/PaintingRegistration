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
#include <iostream>
#include <math.h>
#include "JDHUtility/FileLocationUtility.h"
#include "TextureBlock.h"

namespace PaintingRegistration
{
    /* Public */
    TextureBlock::TextureBlock(std::string format, unsigned int start, unsigned int end, float position)
    {
        this->format = format;
        this->start = start;
        this->end = end;
        this->position = position;
        
        setPosition(position);
        loadAll();
    }
    
    TextureBlock::~TextureBlock(void)
    {
    }
    
    void TextureBlock::bind(void) const
    {
        const GLTexture &t = textures[currentTexture];
        t.bind(GL_REPLACE);
    }
    
    void TextureBlock::setPosition(float position)
    {
        this->position = position;
        currentTexture = (unsigned int)(position * (float)(end - start));
    }
    
    void TextureBlock::unbind(void) const
    {
        const GLTexture &t = textures[currentTexture];
        t.unbind();
    }
    
    /* Private */
    void TextureBlock::loadAll(void)
    {
        textures.clear();
        
        for(unsigned int i = start; i <= end; i++)
        {
            loadTexture(i);
        }
    }
    
    void TextureBlock::loadTexture(unsigned int i)
    {
        char buffer[1024];
        sprintf(buffer, format.c_str(), i);
        GLTexture t(buffer);
        textures.push_back(t);
    }
}