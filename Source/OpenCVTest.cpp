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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "OpenCVTest.h"

namespace PaintingRegistration
{
    OpenCVTest::OpenCVTest(void)
    {
        capture = cvCreateCameraCapture(0);
        
    }
    
    OpenCVTest::~OpenCVTest(void)
    {
        cvReleaseCapture(&capture);
    }
    
    void OpenCVTest::render(void) const
    {
        
    }
    
    void OpenCVTest::update(void)
    {
        IplImage* frame = cvQueryFrame(capture);
        cvCvtColor(frame, image, CV_BGR2GRAY);
        
    }
}