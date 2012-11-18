/**
 * This file is part of PhysicsSynth.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2010-2012 Jonathan Hook. All rights reserved.
 *
 * PhysicsSynth is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PhysicsSynth is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PhysicsSynth.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <exception>
#include "Matrixf.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Matrixf::Matrixf(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;

		unsigned int size = width * height;
		matrix = new float[size];
	}

	Matrixf::~Matrixf(void)
	{
		delete[] matrix;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Matrixf *Matrixf::add(Matrixf &m) const
	{
		Matrixf *result = new Matrixf(width, height);
		
		for(unsigned int i = 0; i < (width * height); i++)
		{
			result->getMatrix()[i] = (matrix[i] + m.getMatrix()[i]);
		}

		return result;
	}

	unsigned int Matrixf::getHeight(void) const
	{
		return height;
	}

	float *Matrixf::getMatrix(void)
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

	Matrixf *Matrixf::multiply(Matrixf &m) const
	{
		Matrixf *result = new Matrixf(m.getWidth(), getHeight());
		
		float val;
		for(unsigned int i = 0; i < result->getWidth(); i++)
		{
			for(unsigned int j = 0; j < height; j++)
			{
				val = 0;
				for(unsigned int k = 0; k < width; k++)
				{
					val += (getValue(k, j) * m.getValue(i, k));
				}

				result->setValue(val, i, j);
			}
		}

		return result;
	}

	void Matrixf::setValue(float value, unsigned int x, unsigned int y)
	{
		matrix[(height * x) + y] = value;
	}

}