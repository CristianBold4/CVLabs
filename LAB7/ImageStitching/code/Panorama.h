//
// Created by Cristian on 06/05/2022.
//

#ifndef IMAGESTITCHING_PANORAMA_H
#define IMAGESTITCHING_PANORAMA_H

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <vector>
#include "panoramic_utils.h"


class Panorama {

    public:
        // -- Variables
        std::vector<cv::Mat> images;
        double FOV;

        // - Constructor
        Panorama(const std::vector<cv::Mat> images, const double FOV);

        // - Functions

        void cylindrical_projection();

        void SIFT_extractor(std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<cv::Mat> &descriptors);

        void BF_matcher(std::vector<cv::Mat> &descriptors, std::vector<std::vector<cv::DMatch>> &total_matches);

        static bool sort_pred ( const cv::DMatch& m1_, const cv::DMatch& m2_ );

        void refine_match(std::vector<std::vector<cv::DMatch>> &total_matches, std::vector<std::vector<cv::DMatch>> &refined_matches, double threshold_matching);

        void find_homography(std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &refined_matches, std::vector<std::vector<cv::DMatch>> &final_matches);

        void find_distances(std::vector<std::vector<float>> &total_distances, std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &final_matches);

        void merge_images (cv::Mat &panorama, std::vector<float> &total_mean_distances, std::vector<std::vector<float>> &total_distances, std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &final_matches);

};


#endif //IMAGESTITCHING_PANORAMA_H
