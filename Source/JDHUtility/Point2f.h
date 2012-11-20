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
#ifndef Point2f_H
#define Point2f_H

namespace JDHUtility
{
	class Vector2f;

	class Point2f
	{
	public:
		Point2f(float x, float y);
		Point2f(const Point2f &position);
		Point2f(void);
		~Point2f(void);

		float	getX		(void) const;
		float	getY		(void) const;
		void	rotate		(float theta);
		void	setPosition	(const Point2f &position);
		void	setPosition	(float x, float y);
		void	scale		(float xs, float ys);
		void	scaleX		(float s);
		void	scaleY		(float s);
		void	setX		(float x);
		void	setY		(float y);
		void	translate	(float xt, float yt);
		void	translate	(const JDHUtility::Vector2f &tv);
		void	translateX	(float t);
		void	translateY	(float t);

	protected:
		float x, y;

	};
}
#endif