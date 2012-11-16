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
#include <exception>
#include <JDHUtility/FileLocationUtility.h>
#include "GLTexture.h"
#include "LoadImage.h"

namespace JDHUtility
{
	/* Private Constants */
	const unsigned int GLTexture::INVALID_TEXTURE = -1;

	/* Constructors */
	GLTexture::GLTexture(std::string path)
	{
        std::string resourcePath = FileLocationUtility::getFileInResourcePath(path);        
        id = createTexture(resourcePath.c_str());
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

	bool GLTexture::isTexture(void) const
	{
		return id != INVALID_TEXTURE;
	}
}
