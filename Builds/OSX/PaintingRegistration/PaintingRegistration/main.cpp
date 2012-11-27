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
#include "JDHUtility/CrossPlatformTime.h"
#include "App.h"

const unsigned int DEFAULT_HEIGHT = 480;
const unsigned int DEFAULT_WIDTH = 320;
const char *WINDOW_TITLE = "PaintingTracker";
const unsigned int UPDATE_RATE = 30;
const unsigned int BUFFER_SIZE = 640 * 480 * 4;
const unsigned int VIDEO_WIDTH = 480;
const unsigned int VIDEO_HEIGHT = 640;

void captureFakeiPhoneImage(void);
void idle(void);
void mouse(int button, int state, int x, int y);
void render(void);
void update(void);

unsigned int lastUpdated= 0;
int now	= 0;
int elapsed	= 0;
bool initialised = false;
bool mouseDown = false;

PaintingRegistration::App *app;
cv::VideoCapture *capture;
unsigned char *frameData = new unsigned char[BUFFER_SIZE];

void captureFakeiPhoneImage(void)
{
    cv::Mat img;
    capture->read(img);
    cv::flip(img, img, 1);
    cv::transpose(img, img);
    cv::cvtColor(img, img, CV_BGR2BGRA);

    memcpy(frameData, img.data, sizeof(unsigned char) * BUFFER_SIZE);
    app->setLatestFrame(frameData, img.cols, img.rows);
    
    cv::imshow("Hello", img);
}

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

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		int width = glutGet(GLUT_WINDOW_WIDTH);
		float fx = (float)x / (float)width;
		float fy = (float)y / (float)width;
        
		mouseDown = (state == GLUT_DOWN);
		FingerEventArgs::EventType type = mouseDown ? FingerEventArgs::FINGER_ADDED : FingerEventArgs::FINGER_REMOVED;
        
		app->raiseEvent(0, fx, fy, type);
	}
}

void motion(int x, int y)
{
	if(mouseDown)
	{
		int width = glutGet(GLUT_WINDOW_WIDTH);
		float fx = (float)x / (float)width;
		float fy = (float)y / (float)width;
        
		app->raiseEvent(0, fx, fy, FingerEventArgs::FINGER_UPDATED);
	}
}
void render(void)
{
    if(initialised)
    {
        app->render();

        glutSwapBuffers();
    
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
        {
            printf("OpenGL error: %i\n", error);
        }
    }
}

void update(void)
{
    captureFakeiPhoneImage();
    app->update();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutInitWindowPosition(0, 0);
    glutCreateWindow(WINDOW_TITLE);
    glutDisplayFunc(render);
	glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    unsigned int width = glutGet(GLUT_WINDOW_WIDTH);
    unsigned int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    app = new PaintingRegistration::App(width, height, "/Users/Jon/github/local/PaintingRegistration/Resources", "/Users/Jon/github/local/PaintingRegistration/Resources");
    app->train();
    initialised = true;
    
    capture = new cv::VideoCapture(0);
    capture->set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);
    
	glutMainLoop();
    return 0;
}
