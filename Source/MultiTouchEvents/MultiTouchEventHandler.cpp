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
#include <assert.h>
#include "JDHUtility/Vector2f.h"
#include "MultiTouchEventHandler.h"

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	MultiTouchEventHandler::MultiTouchEventHandler(void)
	{
		captured = false;
		capturer = NULL;
		enabled = true;
		markedForRelease = false;
	}

	MultiTouchEventHandler::~MultiTouchEventHandler(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void MultiTouchEventHandler::capture(MultiTouchEventManager *capturer)
	{
		this->capturer = capturer;
		captured = true;
	}

	MultiTouchEventManager *MultiTouchEventHandler::getCapturer(void) const
	{
		return capturer;
	}

	bool MultiTouchEventHandler::isCaptured(void) const
	{
		return captured;
	}

	bool MultiTouchEventHandler::isEnabled(void) const
	{
		return enabled;
	}

	bool MultiTouchEventHandler::isMarkedForRelease(void) const
	{
		return markedForRelease;
	}

	void MultiTouchEventHandler::keyPressed(unsigned char key)
	{

	}
	
	void MultiTouchEventHandler::keyReleased(unsigned char key)
	{

	}

	void MultiTouchEventHandler::processKeyEvent(unsigned char key, bool pressed)
	{
		if(pressed)
		{
			keyPressed(key);
		}
		else
		{
			keyReleased(key);
		}

		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *child = (*it);
			assert(child != NULL);
			
			child->processKeyEvent(key, pressed);
		}
	}

	void MultiTouchEventHandler::registerEventHandler(MultiTouchEventHandler *eventHandler)
	{
		handlers.push_back(eventHandler);
	}

	void MultiTouchEventHandler::release(void)
	{
		capturer = NULL;
		captured = false;
	}

	MultiTouchEventHandler *MultiTouchEventHandler::searchEventTree(const FingerEventArgs &e)
	{
		if(enabled)
		{
			// search children
			for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				MultiTouchEventHandler *handled = (*it)->searchEventTree(e);

				if(handled != NULL) 
				{
					return handled;
				}
			}

			if(contains(e)) return this;		
		}
		
		return NULL;	
	}

	void MultiTouchEventHandler::setEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

	void MultiTouchEventHandler::setMarkedForRelease(bool markedForRelease)
	{
		this->markedForRelease = markedForRelease;
	}

	void MultiTouchEventHandler::unregisterEventHandler(MultiTouchEventHandler *eventHandler)
	{
		for(std::vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			if(*it == eventHandler)
			{
				it = handlers.erase(it);
				break;
			}
		}
	}
}
