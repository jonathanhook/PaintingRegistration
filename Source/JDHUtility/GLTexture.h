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
#include <string>
#include "OpenGL.h"

namespace JDHUtility
{
	class GLTexture
	{
	public:
		GLTexture(std::string path);
		~GLTexture(void);

		void			bind(	GLenum mode			= GL_MODULATE, 
								GLenum minFilter	= GL_LINEAR,
								GLenum magFilter	= GL_LINEAR, 
								GLenum wrapS		= GL_CLAMP_TO_EDGE,
								GLenum wrapT		= GL_CLAMP_TO_EDGE) const;
        void            unbind(void) const;
		unsigned int	getId(void) const;
        unsigned int    getWidth(void) const;
        unsigned int    getHeight(void) const;
		bool			isTexture(void) const;

	private:
		static const unsigned int INVALID_TEXTURE;

		unsigned int id;
        unsigned int width;
        unsigned int height;
	};
}

