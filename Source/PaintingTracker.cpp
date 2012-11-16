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
#include "PaintingTracker.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <JDHUtility/FileLocationUtility.h>
#include <JDHUtility/Ndelete.h>

using namespace JDHUtility;

namespace PaintingRegistration
{
    PaintingTracker::PaintingTracker(const std::string image)
    {
        cap = new cv::VideoCapture(0);
        detector = new cv::SiftFeatureDetector();
        extractor = new cv::SiftDescriptorExtractor();
        hasTarget = false;
        matcher = new cv::BFMatcher(cv::NORM_L2, false);
        
        if(image != "")
        {
            train(image);
        }
    }
    
    PaintingTracker::~PaintingTracker(void)
    {
        NDELETE(cap);
        NDELETE(detector);
        NDELETE(extractor);
        NDELETE(matcher);
    }
    
    void PaintingTracker::computeHomography(void)
    {
        cap->read(cameraImage);
        cv::cvtColor(cameraImage, cameraImage, CV_RGB2GRAY);
        
        detector->detect(cameraImage, cameraKeyPoints);
        extractor->compute(cameraImage, cameraKeyPoints, cameraDescriptors);
        matcher->match(targetDescriptors, cameraDescriptors, matches);
        
        maxDist = 0;
        minDist = 75;
        for(int i = 0; i < targetDescriptors.rows; i++)
        {
            double dist = matches[i].distance;
            if(dist < minDist)
            {
                minDist = dist;
            }
            
            if(dist > maxDist)
            {
                maxDist = dist;
            }
        }
        
        for(int i = 0; i < targetDescriptors.rows; i++)
        {
            if(matches[i].distance < 3 * minDist)
            {
                goodMatches.push_back(matches[i]);
            }
        }
    
        for(int i = 0; i < goodMatches.size(); i++)
        {
            t.push_back(targetKeyPoints[goodMatches[i].queryIdx].pt);
            f.push_back(cameraKeyPoints[goodMatches[i].trainIdx].pt);
        }
        
        if(t.size() >= 4 && f.size() >= 4)
        {
            homography = cv::findHomography(t, f, CV_RANSAC);
            hasTarget = true;
        }
        else
        {
            hasTarget = false;
        }
        
        cameraKeyPoints.clear();
        f.clear();
        goodMatches.clear();
        matches.clear();
        t.clear();
    }
    
    bool PaintingTracker::getHasTarget(void) const
    {
        return hasTarget;
    }
    
    const cv::Mat &PaintingTracker::getHomography(void) const
    {
        return homography;
    }
    
    void PaintingTracker::train(const std::string image)
    {
        std::string localPath = FileLocationUtility::getFileInResourcePath(image);
        targetImage = cv::imread(localPath, CV_LOAD_IMAGE_GRAYSCALE);
        
        detector->detect(targetImage, targetKeyPoints);
        extractor->compute(targetImage, targetKeyPoints, targetDescriptors);
    }

}