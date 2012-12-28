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
#include <math.h>
#include "Point2f.h"
#include "Vector2f.h"

namespace JDHUtility
{
	/* CONSTRUCORS */
	Vector2f::Vector2f(float x, float y) :
		Point2f(x, y)
	{
	}

	Vector2f::Vector2f(void) :
		Point2f()
	{
	}

	Vector2f::~Vector2f(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Vector2f::getAngle(const Vector2f &v) const
	{
		// WARNING: this may not work

		float length = (getMagnitude() * v.getMagnitude());
        if (length == 0.0f)
        {
            return 0.0f;
        }
        
		return (float)acos(getDotProduct(v) / length);
	}
	
	float Vector2f::getDotProduct(const Vector2f &v) const
	{
		return (x * v.getX()) + (y * v.getY());
	}

	float Vector2f::getCrossProduct(const Vector2f &v) const
	{	
		float vx = v.getX();
		float vy = v.getY();

		return (x * vy) - (y * vx);
	}

	Vector2f Vector2f::getCrossProduct(void) const
	{
		return Vector2f(y, -x);
	}

	Point2f Vector2f::getCentre(void) const
	{
		return Point2f(x / 2.0f, y / 2.0f); 
	}

	float Vector2f::getMagnitude(void) const
	{
		return sqrt((x * x) + (y * y));
	}

	Vector2f Vector2f::getNormalised(void) const
	{
		float magnitude = getMagnitude();
		
		if(magnitude == 0.0f)
		{
			return Vector2f(0.0f, 0.0f);
		}

		return Vector2f(x / magnitude, y /magnitude);
	}

	float Vector2f::getScaleFactor(const Vector2f &v) const
	{
		return getMagnitude() / v.getMagnitude();
	}

	void Vector2f::normalise(void)
	{
		float magnitude = getMagnitude();
		x /= magnitude;
		y /= magnitude;
	}
}
