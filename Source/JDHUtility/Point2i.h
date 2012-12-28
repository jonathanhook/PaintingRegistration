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
#pragma once

namespace JDHUtility
{
	class Vector2f;

	class Point2i
	{
	public:
		Point2i(int x, int y);
		Point2i(const Point2i &position);
		Point2i(void);
		~Point2i(void);

		int		getX		(void) const;
		int		getY		(void) const;
		void	rotate		(float theta);
		void	setPosition	(const Point2i &position);
		void	scale		(int xs, int ys);
		void	scaleX		(int s);
		void	scaleY		(int s);
		void	setX		(int x);
		void	setY		(int y);
		void	translate	(int xt, int yt);
		void	translate	(const JDHUtility::Vector2f &tv);
		void	translateX	(int t);
		void	translateY	(int t);

	protected:
		int x, y;

	};
}

