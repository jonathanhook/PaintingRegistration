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
#include "JDHUtility/stb_image.h"
#include "TextureBlock.h"

namespace PaintingRegistration
{
    /* Static */
    std::vector<unsigned char *> TextureBlock::textures;
    bool TextureBlock::texturesLoaded = false;
    unsigned int TextureBlock::end = 0;
    unsigned int TextureBlock::start = 0;
    unsigned int TextureBlock::bpp = 0;
    Point2i TextureBlock::dimensions(0, 0);
    GLuint TextureBlock::texture = 0;
    
    void TextureBlock::loadAll(const std::string &format)
    {
        for(unsigned int i = end; i >= start; i--)
        {
            loadTexture(i, format);
        }
        
        texturesLoaded = true;
    }
    
    void TextureBlock::loadTexture(unsigned int i, const std::string &format)
    {
        char buffer[1024];
        sprintf(buffer, format.c_str(), i, i);
        std::string resPath = FileLocationUtility::getFileInResourcePath(buffer);
        
#ifdef IOS_WINDOWING
        stbi_convert_iphone_png_to_rgb(1);
#endif
        int x, y, n;
        unsigned char *data = stbi_load(resPath.c_str(), &x, &y, &n, 0);
        textures.push_back(data);
        
        dimensions.setX(x);
        dimensions.setY(y);
        bpp = n;
    }
    
    /* Public */ // NOTE: The static textures should be in an object passed to this, OK for now
    TextureBlock::TextureBlock(const std::string &format, unsigned int start, unsigned int end)
    {
        if(!TextureBlock::texturesLoaded)
        {
            this->start = start;
            this->end = end;
            TextureBlock::loadAll(format);
        }
        
        initTexture();
    }
    
    TextureBlock::~TextureBlock(void)
    {
        // NOTE: static texture memory never deleted
    }
    
    void TextureBlock::bind(void) const
    {
        glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
    
    void TextureBlock::unbind(void) const
    {
        glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
    }
    
    const Point2i &TextureBlock::getDimensions(void) const
    {
        return dimensions;
    }
    
    /* Protected */
    void TextureBlock::initTexture(void)
    {
        unsigned char *initialData = textures[textures.size() - 1];
        
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, bpp == 4 ? GL_RGBA : GL_RGB, dimensions.getX(), dimensions.getY(), 0, bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, initialData);
        glDisable(GL_TEXTURE_2D);
    }

}