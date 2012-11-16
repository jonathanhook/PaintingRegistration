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

namespace PaintingRegistration
{
    class PaintingTracker
    {
    public:
        PaintingTracker(const std::string image = "");
        ~PaintingTracker(void);
        
        void computeHomography(void);
        bool getHasTarget(void) const;
        const cv::Mat &getHomography(void) const;
        void train(const std::string image);
        
    private:
        bool hasTarget;
        
        double maxDist;
        double minDist;
        
        cv::Mat cameraDescriptors;
        cv::Mat cameraImage;
        cv::Mat homography;
        cv::Mat targetDescriptors;
        cv::Mat targetImage;
        
        std::vector<cv::KeyPoint> cameraKeyPoints;
        std::vector<cv::DMatch> goodMatches;
        std::vector<cv::DMatch> matches;
        std::vector<cv::KeyPoint> targetKeyPoints;
        std::vector<cv::Point2f> t;
        std::vector<cv::Point2f> f;
        
        cv::VideoCapture *cap;
        cv::SiftFeatureDetector *detector;
        cv::SiftDescriptorExtractor *extractor;
        cv::BFMatcher *matcher;

        

        
    };
}
