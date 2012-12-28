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
#include <stdio.h>
#include "JDHUtility/Point2f.h"
#include "FingerEventArgs.h"

using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	FingerEventArgs::FingerEventArgs(unsigned int id, const Point2f &position, EventType type)
	{
		this->id = id;
		this->position = position;
		this->type = type;
	}

	FingerEventArgs::~FingerEventArgs(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	unsigned int FingerEventArgs::getId(void) const
	{
		return id;
	}

	const Point2f &FingerEventArgs::getPosition(void) const
	{
		return position;
	}

	FingerEventArgs::EventType FingerEventArgs::getType(void) const
	{
		return type;
	}

	float FingerEventArgs::getX(void) const
	{
		return position.getX();
	}

	float FingerEventArgs::getY(void) const
	{
		return position.getY();
	}
}
