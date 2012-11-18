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
#pragma once

namespace JDHUtility
{
	class Matrixf
	{
	public:
		Matrixf(unsigned int width, unsigned int height);
		~Matrixf(void);

		Matrixf *add(Matrixf &m) const;
		unsigned int getHeight(void) const;
		float *getMatrix(void);
		float getValue(unsigned int x, unsigned int y) const;
		unsigned int getWidth(void) const;
		Matrixf *multiply(Matrixf &m) const;
		void setValue(float value, unsigned int x, unsigned int y);

	protected:
		Matrixf(void);

		unsigned int width;
		float *matrix;
		unsigned int height;
	};
}

