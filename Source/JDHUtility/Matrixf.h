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
#include "Point2f.h"
#include "Point3f.h"

namespace JDHUtility
{
	class Matrixf
	{
	public:
        static Matrixf add(const Matrixf &a, const Matrixf &b);
        static Matrixf multiply(const Matrixf &a, const Matrixf &b);
        static Matrixf scale(const Matrixf &a, float s);
        static Matrixf subtract(const Matrixf &a, const Matrixf &b);

		Matrixf(unsigned int width, unsigned int height);
		~Matrixf(void);

		void add(Matrixf &m);
		unsigned int getHeight(void) const;
		float *getPtr(void);
        const float *getPtr(void) const;
		float getValue(unsigned int x, unsigned int y) const;
		unsigned int getWidth(void) const;
		void multiply(Matrixf &m);
        void print(void) const;
        void scale(float s);
		void setValue(float value, unsigned int x, unsigned int y);
        void setValues(const Matrixf &m);
        void subtract(Matrixf &m);
        Point2f transform(const Point2f &p) const;
		Point3f transform(const Point3f &p) const;

	private:
		unsigned int width;
		float *matrix;
		unsigned int height;
	};
}
