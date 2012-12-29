/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2010-2012 Jonathan Hook. All rights reserved.
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
#include <assert.h>
#include <exception>
#include "Ndelete.h"
#include "Matrixf.h"

namespace JDHUtility
{
    /* Static */
    Matrixf Matrixf::add(const Matrixf &a, const Matrixf &b)
	{
        assert(a.width == b.width && a.height == b.height);

        unsigned int width = a.width;
        unsigned int height = a.height;
        Matrixf result(width, height);
        
		for(unsigned int i = 0; i < (width * height); i++)
		{
			result.getPtr()[i] = (a.getPtr()[i] + b.getPtr()[i]);
		}
        
		return result;
	}
    
    Matrixf Matrixf::multiply(const Matrixf &a, const Matrixf &b)
	{
        assert(a.width == b.height);
        
        Matrixf result(a.height, b.width);
		float val;
        
		for(unsigned int i = 0; i < b.getWidth(); i++)
		{
			for(unsigned int j = 0; j < a.getHeight(); j++)
			{
				val = 0.0f;
				for(unsigned int k = 0; k < a.getWidth(); k++)
				{
					val += (a.getValue(k, j) * b.getValue(i, k));
				}
                
				result.setValue(val, i, j);
			}
		}
        
		return result;
	}
    
    Matrixf Matrixf::scale(const Matrixf &a, float s)
    {
        unsigned int width = a.width;
        unsigned int height = a.height;
        Matrixf result(width, height);
        
		for(unsigned int i = 0; i < (width * height); i++)
		{
			result.getPtr()[i] = a.getPtr()[i] * s;
		}
        
        return result;
    }
    
    Matrixf Matrixf::subtract(const Matrixf &a, const Matrixf &b)
    {
        assert(a.width == b.width && a.height == b.height);
        
        unsigned int width = a.width;
        unsigned int height = a.height;
        Matrixf result(width, height);
        
		for(unsigned int i = 0; i < (width * height); i++)
		{
			result.getPtr()[i] = (a.getPtr()[i] - b.getPtr()[i]);
		}
        
		return result;
    }
    
	/* Public */
	Matrixf::Matrixf(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;

		unsigned int size = width * height;
		matrix = new float[size];
	}

	Matrixf::~Matrixf(void)
	{
		NDELETE_ARRAY(matrix);
	}
    
    void Matrixf::add(Matrixf &m)
    {
        Matrixf result = add(*this, m);
        setValues(result);
    }

	unsigned int Matrixf::getHeight(void) const
	{
		return height;
	}
    
    /* Adapted from the MESA implementation of gluInvertMatrix */
    Matrixf Matrixf::getInverse(void) const
    {
        // NOTE: only words for OpenGL matrices
        assert(width == 4 && height == 4);
        
        Matrixf result(width, height);
        float *m = matrix;
        float *inv = result.getPtr();
        
        inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];
            
        inv[4] = -m[4]  * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];
            
        inv[8] = m[4]  * m[9] * m[15] -
        m[4]  * m[11] * m[13] -
        m[8]  * m[5] * m[15] +
        m[8]  * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];
            
        inv[12] = -m[4]  * m[9] * m[14] +
        m[4]  * m[10] * m[13] +
        m[8]  * m[5] * m[14] -
        m[8]  * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];
            
        inv[1] = -m[1]  * m[10] * m[15] +
        m[1]  * m[11] * m[14] +
        m[9]  * m[2] * m[15] -
        m[9]  * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];
            
        inv[5] = m[0]  * m[10] * m[15] -
        m[0]  * m[11] * m[14] -
        m[8]  * m[2] * m[15] +
        m[8]  * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];
            
        inv[9] = -m[0]  * m[9] * m[15] +
        m[0]  * m[11] * m[13] +
        m[8]  * m[1] * m[15] -
        m[8]  * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];
            
        inv[13] = m[0]  * m[9] * m[14] -
        m[0]  * m[10] * m[13] -
        m[8]  * m[1] * m[14] +
        m[8]  * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];
            
        inv[2] = m[1]  * m[6] * m[15] -
        m[1]  * m[7] * m[14] -
        m[5]  * m[2] * m[15] +
        m[5]  * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];
            
        inv[6] = -m[0]  * m[6] * m[15] +
        m[0]  * m[7] * m[14] +
        m[4]  * m[2] * m[15] -
        m[4]  * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];
            
        inv[10] = m[0]  * m[5] * m[15] -
        m[0]  * m[7] * m[13] -
        m[4]  * m[1] * m[15] +
        m[4]  * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];
            
        inv[14] = -m[0]  * m[5] * m[14] +
        m[0]  * m[6] * m[13] +
        m[4]  * m[1] * m[14] -
        m[4]  * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];
            
        inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];
            
        inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];
            
        inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];
            
        inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];
            
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
            
        if (det != 0)
        {
            det = 1.0 / det;
            
            for (unsigned int i = 0; i < 16; i++)
            {
                inv[i] = inv[i] * det;
            }
        }
        
        return result;
    }

	float *Matrixf::getPtr(void)
	{
		return matrix;
	}
    
    const float *Matrixf::getPtr(void) const
    {
        return matrix;
    }

	float Matrixf::getValue(unsigned int x, unsigned int y) const
	{
		return matrix[(height * x) + y];
	}

	unsigned int Matrixf::getWidth(void) const
	{
		return width;
	}
    
    void Matrixf::multiply(Matrixf &m)
    {
        Matrixf result = multiply(*this, m);
        setValues(result);
    }
    
    void Matrixf::scale(float s)
    {
        Matrixf result = scale(*this, s);
        setValues(result);
    }

	void Matrixf::setValue(float value, unsigned int x, unsigned int y)
	{
		matrix[(height * x) + y] = value;
	}
    
    void Matrixf::setValues(const Matrixf &m)
    {
        assert(width == m.width && height == m.height);
        
        for(unsigned int i = 0; i < (width * height); i++)
		{
			matrix[i] = m.getPtr()[i];
		}
    }
    
    void Matrixf::subtract(Matrixf &m)
    {
        Matrixf result = subtract(*this, m);
        setValues(result);
    }
    
    Point2f Matrixf::transform(const Point2f &p) const
    {
        Point3f p3d(p.getX(), p.getY(), 0.0f);
        p3d = transform(p3d);
        
        Point2f result(p3d.getX(), p3d.getY());
        return result;
    }
    
    Point3f Matrixf::transform(const Point3f &p) const
    {
        float a = p.getX();
        float b = p.getY();
        float c = p.getZ();
        float d = 1.0f;
            
        float vx = matrix[0] * a + matrix[4] * b + matrix[8] * c + matrix[12] * d;
        float vy = matrix[1] * a + matrix[5] * b + matrix[9] * c + matrix[13] * d;
        float vz = matrix[3] * a + matrix[6] * b + matrix[10] * c + matrix[14] * d;

        Point3f result(vx, vy, vz);
        return result;
    }

}