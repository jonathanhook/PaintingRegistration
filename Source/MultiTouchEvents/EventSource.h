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
#ifndef EventSource_H
#define EventSource_H

#include "FastDelegate.h"
#include "FingerEventArgs.h"

using namespace fastdelegate;

namespace MultiTouchEvents
{
	class EventSource
	{
	public:
		typedef FastDelegate4<unsigned int, float, float, FingerEventArgs::EventType> EventCallback;

		EventSource(void);
		~EventSource(void);

		void setEventCallback(const EventCallback &eventCallback);

	protected:
		EventCallback eventCallback;

		void processPointForWindowTarget(Point2f &p) const;
	};
}

#endif

