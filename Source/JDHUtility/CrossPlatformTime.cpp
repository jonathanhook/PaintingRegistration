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
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#pragma comment(lib, "winmm")
#else
#include <sys/timeb.h>
#endif
#include "CrossPlatformTime.h"

namespace JDHUtility
{
    int CrossPlatformTime::getTimeMillis(void)
	{

#ifdef _WIN32
		return (long)timeGetTime();
#else
		struct timeb t;
        ftime(&t);
        
        int secs               = (int)t.time;
        unsigned short millis  = t.millitm;

        return (secs * 1000) + millis;
#endif
	}
}

