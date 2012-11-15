#include <stdio.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>

int main(int argc, const char *argv[])
{
    cv::namedWindow("window");
    
    cv::VideoCapture cap(0);
    cv::Mat frame;
    cv::Mat gray;
    cv::Mat target;
    
    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> frameKeyPoints;
    std::vector<cv::KeyPoint> targetKeyPoints;
    
    cv::SiftDescriptorExtractor extractor;
    cv::Mat frameDescriptors;
    cv::Mat targetDescriptors;
    
    cv::BFMatcher matcher(cv::NORM_L2, false);
    std::vector<cv::DMatch> matches;
    
    target = cv::imread("/Users/Jon/Desktop/target.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    detector.detect(target, targetKeyPoints);
    extractor.compute(target, targetKeyPoints, targetDescriptors);
    
    while(true)
    {
        cap.read(frame);
        cv::cvtColor(frame, gray, CV_RGB2GRAY);
       
        detector.detect(gray, frameKeyPoints);
        extractor.compute(gray, frameKeyPoints, frameDescriptors);
        matcher.match(targetDescriptors, frameDescriptors, matches);

        double max_dist = 0; double min_dist = 100;
        for(int i = 0; i < targetDescriptors.rows; i++)
        {
            double dist = matches[i].distance;
            if(dist < min_dist) min_dist = dist;
            if(dist > max_dist) max_dist = dist;
        }
        
        std::vector<cv::DMatch> goodMatches;
        for(int i = 0; i < targetDescriptors.rows; i++)
        {
            if(matches[i].distance < 3 * min_dist)
            {
                goodMatches.push_back( matches[i]);
            }
        }
        
        std::vector<cv::Point2f> t;
        std::vector<cv::Point2f> f;
        
        for(int i = 0; i < goodMatches.size(); i++)
        {
            t.push_back(targetKeyPoints[goodMatches[i].queryIdx].pt);
            f.push_back(frameKeyPoints[goodMatches[i].trainIdx].pt);
        }
        
        if(t.size() > 0 && f.size() > 0)
        {
            cv::Mat H = cv::findHomography(t, f, CV_RANSAC);
        
            std::vector<cv::Point2f> corners(4);
            corners[0] = cvPoint(0,0);
            corners[1] = cvPoint(target.cols, 0);
            corners[2] = cvPoint(target.cols, target.rows);
            corners[3] = cvPoint(0, target.rows);
            std::vector<cv::Point2f> sceneCorners(4);
        
            cv::perspectiveTransform(corners, sceneCorners, H);

            cv::line(frame, sceneCorners[0], sceneCorners[1], cv::Scalar(0, 255, 0), 4);
            cv::line(frame, sceneCorners[1], sceneCorners[2], cv::Scalar(0, 255, 0), 4);
            cv::line(frame, sceneCorners[2], sceneCorners[3], cv::Scalar(0, 255, 0), 4);
            cv::line(frame, sceneCorners[3], sceneCorners[0], cv::Scalar(0, 255, 0), 4);
        }
        
        cv::imshow("window", frame);
    }

    
}

