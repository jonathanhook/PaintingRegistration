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
#include "Point2f.h"

namespace JDHUtility
{
	class Vector2f :
		public Point2f
	{
	public:
		Vector2f(float x, float y);
		Vector2f(void);
		~Vector2f(void);

		float		getAngle		(const Vector2f &v) const;
		float		getDotProduct	(const Vector2f &v) const;
		Point2f		getCentre		(void) const;
		float		getCrossProduct	(const Vector2f &v) const;
		Vector2f	getCrossProduct	(void) const;
		float		getMagnitude	(void) const;
		Vector2f	getNormalised	(void) const;
		float		getScaleFactor	(const Vector2f &v) const;
		void		normalise		(void);
	};
}
