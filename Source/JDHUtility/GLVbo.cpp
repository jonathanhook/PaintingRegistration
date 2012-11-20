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
#include "GLVbo.h"

namespace JDHUtility
{
    /* Constants */
    const GLuint GLVbo::COORDS_IN_VERTEX = 3;
    const GLuint GLVbo::COORDS_IN_UV = 2;
    
    /* Constructors */
    GLVbo::GLVbo(GLenum mode, GLenum usage, GLfloat *vertices, GLsizei count, GLfloat *textureCoords)
    {
        hasTexture = false;
        
        glGenBuffers(1, &id);
        glGenBuffers(1, &textureId);
        
        update(mode, usage, vertices, count, textureCoords);
    }
    
    GLVbo::~GLVbo(void)
    {
        glDeleteBuffers(1, &id);
        glDeleteBuffers(1, &textureId);
    }
    
    /* Public member functions */
    void GLVbo::render(void) const
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, id);
		glVertexPointer(COORDS_IN_VERTEX, GL_FLOAT, 0, 0);
        
        if(hasTexture)
        {
            glBindBuffer(GL_ARRAY_BUFFER, textureId);
            glTexCoordPointer(COORDS_IN_UV, GL_FLOAT, 0, 0);
        }
        
        glDrawArrays(mode, 0, count);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        if(hasTexture)
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    
    void GLVbo::update(GLenum mode, GLenum usage, GLfloat *vertices, GLsizei count, GLfloat *textureCoords)
    {
        this->mode  = mode;
        this->count = count;
        
        if(vertices != NULL)
        {
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * COORDS_IN_VERTEX * count, vertices, usage);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        if(textureCoords != NULL)
        {
            glBindBuffer(GL_ARRAY_BUFFER, textureId);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * COORDS_IN_UV * count, textureCoords, usage);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            hasTexture = true;
        }
    }
}