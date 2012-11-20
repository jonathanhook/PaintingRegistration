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
#include <JDHUtility/OpenGL.h>
#include "Colour3f.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Colour3f::Colour3f(float r, float g, float b)
	{
		Colour3f:: r = r;
		Colour3f:: g = g;
		Colour3f:: b = b;
	}

	Colour3f::Colour3f(void) 
	{
		Colour3f(0.0f, 0.0f, 0.0);
	}

	Colour3f::~Colour3f(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Colour3f::getR(void) const
	{
		return r;
	}

	float Colour3f::getG(void) const
	{
		return g;
	}

	float Colour3f::getB(void) const
	{
		return b;
	}

	void Colour3f::setR(float r)
	{
		this->r = r;
	}

	void Colour3f::setG(float g)
	{
		this->g = g;
	}

	void Colour3f::setB(float b)
	{
		this->b = b;
	}

	void Colour3f::use(void) const
	{
		glColor4f(r, g, b, 1.0f);
	}
}
