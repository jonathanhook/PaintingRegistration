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

#include "JDHUtility/Matrixf.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Point3f.h"

namespace JDHUtility
{
	class GLMatrixf :
		public Matrixf
	{
	public:
		static const unsigned int OPEN_GL_MATRIX_DIM = 4;

		GLMatrixf	(void);
		~GLMatrixf	(void);

		float	getXScale		(void) const;
		float	getXTranslation	(void) const;
		float	getYScale		(void) const;
		float	getYTranslation	(void) const;
		float	getZScale		(void) const;
		float	getZTranslation	(void) const;
		void	transform		(Point2f &p) const;
		void	transform		(Point3f &p) const;
		void	unScalePoint	(Point2f &p) const;
		void	unTranslatePoint(Point2f &p) const;
	};
}

