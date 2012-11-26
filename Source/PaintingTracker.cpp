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
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "JDHUtility/CrossPlatformTime.h"
#include "JDHUtility/FileLocationUtility.h"
#include "JDHUtility/stb_image.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/OpenGL.h"
#include "JDHUtility/Point2f.h"
#include "PaintingTracker.h"

using namespace JDHUtility;

namespace PaintingRegistration
{
    /* Static */
    const float PaintingTracker::AREA_THRESHOLD = 0.1f;
    const std::string PaintingTracker::SAVE_FILE = "model.xml";
    const std::string PaintingTracker::DESCRIPTOR_LABEL = "descriptors";
    const std::string PaintingTracker::KEY_POINT_LABEL = "keypoints";
    
    /* Public */
    PaintingTracker::PaintingTracker(void)
    {
        hasTarget = false;
        matcher = new cv::BFMatcher(cv::NORM_L2, false);
        vertices = new Point2f[VERTEX_COUNT];
        loadedData = NULL;
        glMatrix = new double[16];

        detector = new cv::SiftFeatureDetector();
        extractor = new cv::SiftDescriptorExtractor();

        targetCorners.resize(4);
    }
    
    PaintingTracker::~PaintingTracker(void)
    {
        NDELETE(cap);
        NDELETE(detector);
        NDELETE(extractor);
        NDELETE(matcher);
        NDELETE_ARRAY(vertices);
        NDELETE_ARRAY(glMatrix);
    }
    
    bool PaintingTracker::compute(const unsigned char *fData, unsigned int fWidth, unsigned int fHeight)
    {
        cv::Mat camImage(fHeight, fWidth, CV_8UC4, (unsigned char *)fData, 0);
        cv::cvtColor(camImage, greyImage, CV_BGRA2GRAY);
        cv::transpose(greyImage, greyImage);

#ifdef IOS_WINDOWING
        cv::flip(greyImage, greyImage, 2);
#endif
        
        detector->detect(greyImage, cameraKeyPoints);
        extractor->compute(greyImage, cameraKeyPoints, cameraDescriptors);
        matcher->match(targetDescriptors, cameraDescriptors, matches);
    
        maxDist = 0;
        minDist = 1000;
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

            targetCorners[0] = cvPoint(0,0);
            targetCorners[1] = cvPoint(targetImage.cols, 0);
            targetCorners[2] = cvPoint(targetImage.cols, targetImage.rows);
            targetCorners[3] = cvPoint(0, targetImage.rows);
            
            cv::perspectiveTransform(targetCorners, cameraCorners, homography);
            
            vertices[0].setPosition(cameraCorners[0].x / (float)greyImage.cols, cameraCorners[0].y / (float)greyImage.rows);
            vertices[1].setPosition(cameraCorners[1].x / (float)greyImage.cols, cameraCorners[1].y / (float)greyImage.rows);
            vertices[2].setPosition(cameraCorners[2].x / (float)greyImage.cols, cameraCorners[2].y / (float)greyImage.rows);
            vertices[3].setPosition(cameraCorners[3].x / (float)greyImage.cols, cameraCorners[3].y / (float)greyImage.rows);
            
            float area = getArea();
            if(area > AREA_THRESHOLD)
            {
                glMatrix[0] = homography.at<double>(0);
                glMatrix[4] = homography.at<double>(1);
                glMatrix[12] = homography.at<double>(2);
                glMatrix[1] = homography.at<double>(3);
                glMatrix[5] = homography.at<double>(4);
                glMatrix[13] = homography.at<double>(5);
                glMatrix[3] = homography.at<double>(6);
                glMatrix[7] = homography.at<double>(7);
                glMatrix[15] = homography.at<double>(8);
                glMatrix[2] = 0.0;
                glMatrix[6] = 0.0;
                glMatrix[8] = 0.0;
                glMatrix[9] = 0.0;
                glMatrix[10] = 0.0;
                glMatrix[11] = 0.0;
                glMatrix[14] = 0.0;
                
                hasTarget = true;
            }
            else
            {
                hasTarget = false;
            }

#if defined(DEBUG) && defined(GLUT_WINDOWING)
            cv::Mat imgMatches;
            
            cv::drawMatches(targetImage, targetKeyPoints, greyImage, cameraKeyPoints, goodMatches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                            cv::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            
            cv::line(imgMatches, cameraCorners[0] + cv::Point2f(targetImage.cols, 0), cameraCorners[1] + cv::Point2f(targetImage.cols, 0), cv::Scalar(0, 255, 0), 4);
            cv::line(imgMatches, cameraCorners[1] + cv::Point2f(targetImage.cols, 0), cameraCorners[2] + cv::Point2f(targetImage.cols, 0), cv::Scalar(0, 255, 0), 4);
            cv::line(imgMatches, cameraCorners[2] + cv::Point2f(targetImage.cols, 0), cameraCorners[3] + cv::Point2f(targetImage.cols, 0), cv::Scalar(0, 255, 0), 4);
            cv::line(imgMatches, cameraCorners[3] + cv::Point2f(targetImage.cols, 0), cameraCorners[0] + cv::Point2f(targetImage.cols, 0), cv::Scalar(0, 255, 0), 4);
            
            cv::resize(imgMatches, imgMatches, cv::Size(imgMatches.cols / 4, imgMatches.rows / 4));
            cv::imshow( "Good Matches & Object detection", imgMatches);
#endif
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
        
        return hasTarget;
    }
    
    const double *PaintingTracker::getGlMatrix(void) const
    {
        return glMatrix;
    }
    
    bool PaintingTracker::getHasVertices(void) const
    {
        return hasTarget;
    }
    
    const Point2f *PaintingTracker::getVertices(void) const
    {
        return vertices;
    }
    
    void PaintingTracker::loadFeatures(void)
    {
        std::string path = FileLocationUtility::getFileInDocumentsPath(SAVE_FILE);
        cv::FileStorage fs(path, cv::FileStorage::READ);
        cv::FileNode node = fs[KEY_POINT_LABEL];
        cv::read(node, targetKeyPoints);
        fs[DESCRIPTOR_LABEL] >> targetDescriptors;
        fs.release();
    }
    
    void PaintingTracker::saveFeatures(void) const
    {
        std::string path = FileLocationUtility::getFileInDocumentsPath(SAVE_FILE);
        cv::FileStorage fs(path, cv::FileStorage::WRITE);
        fs << KEY_POINT_LABEL << targetKeyPoints;
        fs << DESCRIPTOR_LABEL << targetDescriptors;
        fs.release();
    }
    
    void PaintingTracker::train(const std::string &image)
    {
        // TODO: make saving and loading model work on iOS
        
        //std::string sPath = FileLocationUtility::getFileInDocumentsPath(SAVE_FILE);
        //if(fopen(sPath.c_str(), "r") == NULL)
        //{
            std::string path = FileLocationUtility::getFileInResourcePath(image);
        
            int x, y, n;
            unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, 1);
            targetImage = cv::Mat(y, x, CV_8UC1, (unsigned char *)data, 0);
        
            detector->detect(targetImage, targetKeyPoints);
            extractor->compute(targetImage, targetKeyPoints, targetDescriptors);
            
        //    saveFeatures();
        //}
        //else
        //{
        //    loadFeatures();
        //}
    }
    
    /* Private */
    float PaintingTracker::getArea(void) const
    {
        float result = 0.0f;
        for(int i = 0; i < VERTEX_COUNT; i++)
        {
            float x0 = vertices[i].getX();
            float y0 = vertices[i].getY();
            float x1 = vertices[(i + 1) % VERTEX_COUNT].getX();
            float y1 = vertices[(i + 1) % VERTEX_COUNT].getY();
            
            float ay = (y0 + y1) / 2.0f;
            float dx = (x0 - x1);
            float area = dx * ay;
            
            result += area;
        }
        
        return result;
    }
}