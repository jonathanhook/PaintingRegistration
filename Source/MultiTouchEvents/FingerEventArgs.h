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
#ifndef FingerEventArgs_H
#define FingerEventArgs_H

#include <JDHUtility/Point2f.h>

using namespace JDHUtility;

namespace MultiTouchEvents
{
	class FingerEventArgs
	{
	public:
		typedef enum
		{
			FINGER_ADDED,
			FINGER_REMOVED,
			FINGER_UPDATED
		} EventType;

		FingerEventArgs(unsigned int id, const Point2f &position, EventType type);
		~FingerEventArgs(void);

		unsigned int getId(void) const;
		const Point2f &getPosition(void) const;
		EventType getType(void) const;
		float getX(void) const;
		float getY(void) const;

	protected:
		unsigned int id;
		Point2f position;
		EventType type;
	};
}
#endif
