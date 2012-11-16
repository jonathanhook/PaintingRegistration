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
#include <assert.h>
#include <stdio.h>
#include <GLUT/glut.h>
#include <JDHUtility/CrossPlatformTime.h>
#include <JDHUtility/FileLocationUtility.h>
#include <JDHUtility/GLTexture.h>
#include <JDHUtility/OpenGL.h>
#include "PaintingTracker.h"
#include "Renderer.h"

using namespace PaintingRegistration;
using namespace JDHUtility;

const unsigned int DEFAULT_HEIGHT = 768;
const unsigned int DEFAULT_WIDTH = 1024;
const char *WINDOW_TITLE = "PaintingTracker";
const unsigned int UPDATE_RATE = 16;

void idle(void);
void init(void);
void render(void);
void resize(int width, int height);
void setupProjection(unsigned int width, unsigned int height);
void update(void);

PaintingTracker *p;
Renderer *r;
GLTexture *texture;
unsigned int lastUpdated= 0;
int now	= 0;
int elapsed	= 0;

void idle(void)
{
	update();
    
	now	= CrossPlatformTime::getTimeMillis();
	elapsed	= now - lastUpdated;
    
	if(elapsed >= UPDATE_RATE)
	{
		glutPostRedisplay();
		lastUpdated = now;
	}
}

void init(void)
{
    FileLocationUtility::setResourcePath("/Users/Jon/github/local/PaintingRegistration/Resources");
    
    texture = new GLTexture("target.jpg");
    p = new PaintingTracker();
    r = new Renderer();
    
    p->train("target.jpg");
    r->initScene();
    r->setCurrentImage(texture);
}

void render(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    r->render();
    
    glutSwapBuffers();
    
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf("OpenGL error: %i\n", error);
    }
}

void resize(int width, int height)
{
	setupProjection(width, height);
}

void setupProjection(unsigned int width, unsigned int height)
{
	if(height == 0)
	{
		height = 1;
	}
    
	float ratio = (float)width / (float)height;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0.0f, 1.0f, 1.0f / ratio, 0.0f, -1.0f, 100.0f);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update(void)
{
    p->computeHomography();
    if(p->getHasTarget())
    {
        const cv::Mat &homography = p->getHomography();
        r->setHomography(homography);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutInitWindowPosition(0, 0);
    glutCreateWindow(WINDOW_TITLE);
    glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
    
    init();
    
	glutMainLoop();
    return 0;
}
