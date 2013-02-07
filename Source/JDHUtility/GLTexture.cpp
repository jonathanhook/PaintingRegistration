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
#include "JDHUtility/FileLocationUtility.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/stb_image.h"
#include "GLTexture.h"

namespace JDHUtility
{
	/* Private Constants */
	const unsigned int GLTexture::INVALID_TEXTURE = -1;

	/* Constructors */
	GLTexture::GLTexture(std::string path)
	{
        std::string resPath = FileLocationUtility::getFileInResourcePath(path);

#ifdef IOS_WINDOWING
        stbi_convert_iphone_png_to_rgb(1);
#endif
        
        int x, y, n;
        unsigned char *data = stbi_load(resPath.c_str(), &x, &y, &n, 0);
        width = x;
        height = y;
        
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, n == 4 ? GL_RGBA : GL_RGB, x, y, 0, n == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glDisable(GL_TEXTURE_2D);
        
        //free(data);
        delete data;
	}

	GLTexture::~GLTexture(void)
	{
    
	}

	/* Public Member Functions */
	void GLTexture::bind(GLenum mode, GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT) const
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
	}
    
    void GLTexture::unbind(void) const
    {
        glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
    }

	unsigned int GLTexture::getId(void) const
	{
		return id;
	}

    unsigned int GLTexture::getWidth(void) const
    {
        return width;
    }
    
    unsigned int GLTexture::getHeight(void) const
    {
        return height;
    }
    
	bool GLTexture::isTexture(void) const
	{
		return id != INVALID_TEXTURE;
	}
}
