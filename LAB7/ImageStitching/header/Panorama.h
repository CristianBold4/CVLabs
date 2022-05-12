//
// Created by Cristian on 06/05/2022.
//

#ifndef PANORAMA_H
#define PANORAMA_H

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <vector>
#include "panoramic_utils.h"


class Panorama {

    private:
        // -- Variables
        std::vector<cv::Mat> images;
        std::vector<cv::Mat> projected_images;
        double FOV;
        double threshold_matching;

        // - Functions

        void load_images(std::string dataset_path);

        void cylindrical_projection();

        void SIFT_extractor(std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<cv::Mat> &descriptors);

        void BF_matcher(std::vector<cv::Mat> &descriptors, std::vector<std::vector<cv::DMatch>> &total_matches);

        static bool sort_matching(const cv::DMatch &m1, const cv::DMatch &m2);

        void refine_match(std::vector<std::vector<cv::DMatch>> &total_matches, std::vector<std::vector<cv::DMatch>> &refined_matches);

        void find_homography(std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &refined_matches, std::vector<std::vector<cv::DMatch>> &final_matches);

        void find_distances(std::vector<std::vector<float>> &total_distances_x, std::vector<std::vector<float>> &total_distances_y, std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &final_matches);

        void stitch_images (cv::Mat &panorama, std::vector<float> &total_mean_distances_x, std::vector<float> &total_mean_distances_y, std::vector<std::vector<float>> &total_distances_x, std::vector<std::vector<float>> &total_distances_y, std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<std::vector<cv::DMatch>> &final_matches);

    public:
        // -- Constructor
        Panorama(std::string dataset_path, double FOV, double threshold_matching);


        // -- create panorama
        void create_panorama(cv::Mat &panorama);

};


#endif //PANORAMA_H
