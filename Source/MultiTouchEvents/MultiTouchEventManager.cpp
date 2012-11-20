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
#include <assert.h>
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/XPlatformMutex.h"
#include "EventSource.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"
#include "FingerEventArgs.h"
#include "MultiTouchEventHandler.h"
#include "MultiTouchEventManager.h"

using namespace fastdelegate;
using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	MultiTouchEventManager::MultiTouchEventManager(bool dispatchEvents)
	{

		peekEnabled = true;
		this->dispatchEvents = dispatchEvents;

		mutex = new XPlatformMutex();
	}

	MultiTouchEventManager::~MultiTouchEventManager(void)
	{
		while(!eventQueue.empty())
		{
			FingerEventArgs *e = eventQueue.front();
			eventQueue.pop();
			NDELETE(e);
		}

		NDELETE(mutex);
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void MultiTouchEventManager::dispatchAllEvents(void)
	{
		mutex->wait();

		while(!eventQueue.empty())
		{
			FingerEventArgs *e = eventQueue.front();
			eventQueue.pop();
			dispatchEvent(e);
		}

		mutex->release();
	}

	bool MultiTouchEventManager::isPeekEnabled(void)
	{
		return peekEnabled;
	}

	void MultiTouchEventManager::raiseEvent(unsigned int id, float x, float y, FingerEventArgs::EventType type)
	{
		// HACK : better than it was before
		if(type == FingerEventArgs::FINGER_REMOVED || (x > 0.001f && y > 0.001f))
		{
			Point2f p(x, y);
			FingerEventArgs *e = new FingerEventArgs(id, p, type);

			if(dispatchEvents) 
			{
				dispatchEvent(e);
			}
			else 
			{
				mutex->wait();
				eventQueue.push(e);
				mutex->release();
			}
		}
	}

	void MultiTouchEventManager::raiseKeyEvent(unsigned char key, bool pressed)
	{
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *handler = (*it);
			assert(handler != NULL);

			handler->processKeyEvent(key,  pressed);
		}
	}

	void MultiTouchEventManager::registerEventHandler(MultiTouchEventHandler *eventHandler)
	{
		handlers.push_back(eventHandler);
	}

	void MultiTouchEventManager::registerEventSource(EventSource *source)
	{
		assert(source);
		source->setEventCallback(MakeDelegate(this, &MultiTouchEventManager::raiseEvent));
	}

	void MultiTouchEventManager::release(unsigned int id)
	{
		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->setMarkedForRelease(true);
		}
	}

	void MultiTouchEventManager::setPeekEnabled(bool peekEnabled)
	{
		this->peekEnabled = peekEnabled;
	}

	void MultiTouchEventManager::unregisterEventHandler(MultiTouchEventHandler *eventHandler)
	{
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			if(*it == eventHandler)
			{
				handlers.erase(it);
				break;
			}
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void MultiTouchEventManager::dispatchAddedEvent(FingerEventArgs *e)
	{
		unsigned int id = e->getId();
		
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *capturedHandler = (*it)->searchEventTree(*e);
			if(capturedHandler != NULL)
			{
				captured[id] = capturedHandler;
				captured[id]->capture(this);
				captured[id]->fingerAdded(*e);
				break;
			}
		}
	}

	void MultiTouchEventManager::dispatchEvent(FingerEventArgs *e)
	{
		assert(e != NULL);

		for(std::map<int, MultiTouchEventHandler *>::iterator it = captured.begin(); it != captured.end(); )
		{
			MultiTouchEventHandler *h = (*it).second;
			assert(h);

			if(h->isMarkedForRelease())
			{
				//it = captured.erase(it);
                captured.erase(it++);
                
                h->setMarkedForRelease(false);
			}
			else ++it;
		}

		// let any peekers peek (WARNING: has been known to send events out of order)
		if(peekEnabled)
		{
			peek(*e);
		}

		// dispatch the event
		switch(e->getType())
		{
			case FingerEventArgs::EventType::FINGER_ADDED:
				dispatchAddedEvent(e);
				break;
			case FingerEventArgs::EventType::FINGER_REMOVED:
				dispatchRemovedEvent(e);
				break;
			case FingerEventArgs::EventType::FINGER_UPDATED:
				if(captured.count(e->getId()) > 0)
				{
					dispatchUpdatedEvent(e);
				}
				else
				{
					dispatchAddedEvent(e);
				}
				break;
		}

		NDELETE(e);
	}

	void MultiTouchEventManager::dispatchRemovedEvent(FingerEventArgs *e)
	{
		assert(e);

		unsigned int id = e->getId();
		
		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->fingerRemoved(*e);
			cid->release();
			captured.erase(id);
		}
	}

	void MultiTouchEventManager::dispatchUpdatedEvent(FingerEventArgs *e)
	{
		assert(e);

		unsigned int id = e->getId();

		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->fingerUpdated(*e);
		}
	}

	void MultiTouchEventManager::peek(const FingerEventArgs &e)
	{

	}
}