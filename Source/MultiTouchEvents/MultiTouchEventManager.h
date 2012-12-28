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
#include <map>
#include <queue>
#include "JDHUtility/Vector2f.h"
#include "JDHUtility/Point2f.h"
#include "FingerEventArgs.h"

using namespace JDHUtility;

namespace JDHUtility 
{ 
	class XPlatformMutex;
}

namespace MultiTouchEvents
{
	class EventSource;
	class MultiTouchEventHandler;

	class MultiTouchEventManager
	{
	public:
		MultiTouchEventManager(bool dispatchEvents = true);
		~MultiTouchEventManager(void);

		void dispatchAllEvents(void);
		bool isPeekEnabled(void);
		void raiseEvent(unsigned int id, float x, float y, FingerEventArgs::EventType type);
		void raiseKeyEvent(unsigned char key, bool pressed);
		void registerEventHandler(MultiTouchEventHandler *eventHandler);
		void registerEventSource(EventSource *source);
		void release(unsigned int id);
		void setPeekEnabled(bool peekEnabled);
		void unregisterEventHandler(MultiTouchEventHandler *eventHandler);

	private:
		std::map<int, MultiTouchEventHandler *>	captured;
		bool dispatchEvents;
		std::queue<FingerEventArgs *> eventQueue;
		std::vector<MultiTouchEventHandler *> handlers;
		bool peekEnabled;
		
		void dispatchAddedEvent(FingerEventArgs *e);
		void dispatchEvent(FingerEventArgs *e);
		void dispatchRemovedEvent(FingerEventArgs *e);
		void dispatchUpdatedEvent(FingerEventArgs *e);
		virtual void peek(const FingerEventArgs &e);

		XPlatformMutex *mutex;
	};
}
