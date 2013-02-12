/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2012 Jonathan Hook. All rights reserved.
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
#include "WindowingUtils.h"

namespace JDHUtility
{
    float WindowingUtils::DEVICE_WINDOW_WIDTH = 0.0f;
    float WindowingUtils::DEVICE_WINDOW_HEIGHT = 0.0f;
    
#ifdef GLUT_WINDOWING    
	const Point2f &WindowingUtils::getWindowPosition(void)
	{
		int w = glutGet(GLUT_WINDOW_X);
		int h = glutGet(GLUT_WINDOW_Y);

		return Point2f(w, h);
	}

	const Vector2f &WindowingUtils::getWindowDimensions(void)
	{
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);

		return Vector2f(w, h);
	}
}
#elif defined(IOS_WINDOWING)

	Point2f WindowingUtils::getWindowPosition(void)
	{
        return Point2f(0.0f, 0.0f);
	}
    
	Vector2f WindowingUtils::getWindowDimensions(void)
	{
		return Vector2f(DEVICE_WINDOW_WIDTH, DEVICE_WINDOW_HEIGHT);
	}
}
#endif 