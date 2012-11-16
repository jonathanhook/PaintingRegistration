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
#pragma once
#if defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <glut.h>
#include <GL/glu.h>
#define GLUT_WINDOWING
#elif defined __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#define IOS_WINDOWING
#elif TARGET_OS_MAC
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#define GLUT_WINDOWING
#else
#endif
#endif
