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
#include "Point3f.h"

namespace JDHUtility
{
	Point3f::Point3f(float x, float y, float z) :
		Point2f(x, y)
	{
		this->z = z;
	}

	Point3f::Point3f(void)
	{
		Point3f(0.0f, 0.0f, 0.0f);
	}

	Point3f::~Point3f(void)
	{

	}

	float Point3f::getZ(void) const
	{
		return z;
	}

	void Point3f::scaleZ(float s)
	{
		z *= s;
	}

	void Point3f::setZ(float z)
	{
		this->z = z;
	}

	void Point3f::translate(float xt, float yt, float zt)	
	{
		Point2f::translate(xt, yt);
		translateZ(zt);
	}

	void Point3f::translateZ(float t)
	{
		z += t;
	}
}