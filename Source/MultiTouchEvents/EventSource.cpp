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
#include "JDHUtility/WindowingUtils.h"
#include "EventSource.h"

using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	EventSource::EventSource(void)
	{
		this->eventCallback = NULL;
	}

	EventSource::~EventSource(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void EventSource::setEventCallback(const EventCallback &eventCallback)
	{
		this->eventCallback = eventCallback;
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void EventSource::processPointForWindowTarget(Point2f &p) const
	{
		const Point2f &dims = WindowingUtils::getWindowDimensions();
		const Point2f &pos	= WindowingUtils::getWindowPosition();

		float x = p.getX() - pos.getX();
		float y = p.getY() - pos.getY();
		x /= dims.getX();
		y /= dims.getY();

		//if(squarePixel) // HACK: NOT SURE IF THIS SHOULD BE COMMENTED OR NOT AS DEV-ING WITHOUT A WINDOWS TOUCH DEVICE
		//{
		float ratio = (float)dims.getX() / (float)dims.getY();
		y /= ratio;
		//}

		p.setX(x);
		p.setY(y);
	}
}