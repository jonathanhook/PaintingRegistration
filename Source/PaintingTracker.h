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
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "MultiTouchEvents/FastDelegate.h"
#include "JDHUtility/Point3f.h"
#include "JDHUtility/Point2i.h"

using namespace fastdelegate;

namespace JDHUtility
{
    class Point2f;
    class Matrixf;
}
using namespace JDHUtility;

namespace PaintingRegistration
{
    class PaintingTracker
    {
    public:
        static const unsigned int VERTEX_COUNT = 4;
        
        struct AsyncData
        {
            const unsigned char *fData;
            unsigned int fWidth;
            unsigned int fHeight;
        };
        AsyncData threadData;
        
        typedef FastDelegate1<bool> CompletedCallback;
        
        PaintingTracker(void);
        ~PaintingTracker(void);
        
        bool compute(const unsigned char *fData, unsigned int fWidth, unsigned int fHeight);
        void computeAsync(const unsigned char *fData, unsigned int fWidth, unsigned int fHeight);
        bool getHasVertices(void) const;
        const Matrixf *getGlMatrix(void) const;
        const Matrixf *getGlMatrixInverse(void) const;
        const Point2i &getTargetDimensions(void) const;
        const Point2f *getVertices(void) const;
        void loadFeatures(void);
        void saveFeatures(void) const;
        void setCompletedCallback(CompletedCallback completed);
        void train(const std::string &image);
        void throwCompletedEvent(bool result);
        
    private:
        static const float AREA_THRESHOLD;
        static const std::string SAVE_FILE;
        static const std::string DESCRIPTOR_LABEL;
        static const std::string KEY_POINT_LABEL;
        
        bool hasTarget;
        double maxDist;
        double minDist;
        Point2f *vertices;
        unsigned char *loadedData;
        Matrixf *glMatrix;
        Matrixf *glMatrixInverse;
        Point2i targetDimensions;
        
        cv::Mat cameraDescriptors;
        cv::Mat greyImage;
        cv::Mat homography;
        cv::Mat targetDescriptors;
        cv::Mat targetImage;
        
        std::vector<cv::Point2f> cameraCorners;
        std::vector<cv::KeyPoint> cameraKeyPoints;
        std::vector<cv::DMatch> goodMatches;
        std::vector<cv::DMatch> matches;
        std::vector<cv::Point2f> targetCorners;
        std::vector<cv::KeyPoint> targetKeyPoints;
        std::vector<cv::Point2f> t;
        std::vector<cv::Point2f> f;
        
        cv::VideoCapture *cap;
        cv::FeatureDetector *detector;
        cv::DescriptorExtractor *extractor;
        cv::BFMatcher *matcher;
        
        Point3f rotation;
        Point3f translation;
        
        CompletedCallback completed;
        
        float getArea(void) const;
        Matrixf getOpenGLMatrix(const cv::Mat &m);
    };
}
