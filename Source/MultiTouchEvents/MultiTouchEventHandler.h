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
#include "JDHUtility/Vector2f.h"
#include "MultiTouchEventManager.h"

using namespace std;

namespace MultiTouchEvents
{
	class FingerEventArgs;

	class MultiTouchEventHandler
	{
	public:
		MultiTouchEventHandler	(void);
		~MultiTouchEventHandler	(void);

		void							capture(MultiTouchEventManager *capturer);
		virtual bool					contains(const FingerEventArgs &e) const = 0;
		virtual void					fingerAdded(const FingerEventArgs &e) = 0;
		virtual void					fingerUpdated(const FingerEventArgs &e) = 0;
		virtual void					fingerRemoved(const FingerEventArgs &e) = 0;
		MultiTouchEventManager			*getCapturer(void) const;
		bool							isCaptured(void) const;
		bool							isEnabled(void) const;
		bool							isMarkedForRelease(void) const;
		virtual void					keyReleased(unsigned char key);
		virtual void					keyPressed(unsigned char key);
		void							processKeyEvent(unsigned char key, bool pressed);
		void							registerEventHandler(MultiTouchEventHandler *eventHandler);
		void							release(void);
		virtual	MultiTouchEventHandler	*searchEventTree(const FingerEventArgs &e);
		void							setEnabled(bool enabled);
		void							setMarkedForRelease(bool markedForRelease);
		void							unregisterEventHandler(MultiTouchEventHandler *eventHandler);

	protected:
		MultiTouchEventManager				*capturer;
		vector<MultiTouchEventHandler *>	handlers;
		bool								captured;
		bool								enabled;
		bool								markedForRelease;
	};
}
