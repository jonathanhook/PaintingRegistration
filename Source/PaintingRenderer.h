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
#include <opencv2/core/core.hpp>
#include "JDHUtility/OpenGL.h"
#include "UIElement.h"

namespace JDHUtility
{
    class GLVbo;
    class TextureBlock;
}

using namespace JDHUtility;

namespace PaintingRegistration
{
    class PaintingRenderer :
        public UIElement
    {
    public:
        PaintingRenderer(const Point2i &position, const Point2i &dimensions, const Point2i &frameDimensions, const Point2i &textureDimensions);
        ~PaintingRenderer(void);
        
        void render(void) const;
        void setCameraTexture(GLuint handle);
        void setMatrix(const float *matrix);
        void setPosition(float position);
        
    private:
        static const std::string TEXTURE_FILENAME_FORMAT;
        
        GLuint cameraTexture;
        GLVbo *camVbo;
        const Point2f *vertices;
        TextureBlock *textureBlock;
        const float *matrix;
        Point2i frameDimensions;
        
        void renderCameraImage(void) const;
        void renderPerspective(void) const;
    };
}
