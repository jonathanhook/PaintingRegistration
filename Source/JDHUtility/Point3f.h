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
#ifndef Point3f_H
#define Point3f_H

#include <JDHUtility/Point2f.h>

namespace JDHUtility
{
	class Point3f : 
		public Point2f
	{
	public:
		Point3f(float x, float y, float z);
		Point3f(void);
		~Point3f(void);

		float getZ(void) const;
		void scaleZ(float s);
		void setZ(float z);
		void translate(float xt, float yt, float zt);
		void translateZ(float t);
	protected:
		float z;

	};
}
#endif
