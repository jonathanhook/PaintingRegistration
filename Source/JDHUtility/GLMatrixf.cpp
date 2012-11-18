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
#include "GLMatrixf.h"

namespace JDHUtility
{
	/* Constructors */
	GLMatrixf::GLMatrixf(void) :
		Matrixf(OPEN_GL_MATRIX_DIM, OPEN_GL_MATRIX_DIM)
	{

	}

	GLMatrixf::~GLMatrixf(void)
	{

	}

	/* Public Member Functions */
	// only works with no rotation
	float GLMatrixf::getXScale(void) const
	{
		return matrix[0];
	}
	
	// only works with no rotation
	float GLMatrixf::getXTranslation(void) const
	{
		return matrix[12];
	}

	// only works with no rotation
	float GLMatrixf::getYScale(void) const
	{
		return matrix[5];
	}

	// only works with no rotation
	float GLMatrixf::getYTranslation(void) const
	{
		return matrix[13];
	}

	// only works with no rotation
	float GLMatrixf::getZScale(void) const
	{
		return matrix[10];
	}
	
	// only works with no rotation
	float GLMatrixf::getZTranslation(void) const
	{
		return matrix[14];
	}
	
	// un-tested
	void GLMatrixf::transform(Point3f &p) const
	{
		float a = p.getX();
		float b = p.getY();
		float c = p.getZ();
		float d = 1.0f;

		float vx = matrix[0] * a + matrix[4] * b + matrix[8]  *	c + matrix[12] * d;
		float vy = matrix[1] * a + matrix[5] * b + matrix[9]  *	c + matrix[13] * d;
		float vz = matrix[3] * a + matrix[6] * b + matrix[10] * c + matrix[14] * d;
	  //float vd = matrix[4] * a + matrix[7] * b + matrix[11] * c + matrix[15] * d;

		p.setX(vx);
		p.setY(vy);
		p.setZ(vz);
	}

	// only works with no rotation
	void GLMatrixf::unScalePoint(Point2f &p) const
	{
		p.scale(1.0f / getXScale(), 1.0f / getYScale());
	}

	// only works with no rotation
	void GLMatrixf::unTranslatePoint(Point2f &p) const
	{
		p.translateX(-getXTranslation());
		p.translateY(-getYTranslation());
	}
}
