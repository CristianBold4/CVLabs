//
// Created by Cristian on 06/05/2022.
//

#include "Panorama.h"
#include "panoramic_utils.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>


using namespace std;
using namespace cv;

// - Constructor
Panorama::Panorama(const std::vector<cv::Mat> images, const double FOV, const double threshold_matching) {
    this->images = images;
    this->FOV = FOV;
    this->threshold_matching = threshold_matching;
}

void Panorama::cylindrical_projection() {
    for (int i = 0; i < images.size(); i++) {
        projected_images.push_back(cylindricalProj(images[i], FOV / 2));
    }
}

void Panorama::SIFT_extractor(std::vector<std::vector<cv::KeyPoint>> &keypoints, std::vector<cv::Mat> &descriptors) {

    vector<KeyPoint> k;
    Mat d;

    Ptr<SIFT> extractor = SIFT::create();

    for (int i = 0; i < projected_images.size(); i++) {

        extractor->detect(projected_images[i], k);
        extractor->compute(projected_images[i], k, d);

        keypoints.push_back(k);
        descriptors.push_back(d);

        // -- Drawing and showing keypoints
        /*
        Mat output;
        drawKeypoints(images[i], k, output);
        imshow( "Keypoints found with SIFT", output );
        waitKey(0);
        */

        k.clear();

    }

}

void Panorama::BF_matcher(std::vector<cv::Mat> &descriptors, std::vector<std::vector<cv::DMatch>> &total_matches) {

    vector<DMatch> matches;
    Ptr<BFMatcher> BF_matcher = BFMatcher::create(NORM_L2, false);

    for (int i = 0; i < projected_images.size() - 1; i++) {

        BF_matcher->match(descriptors[i], descriptors[i+1], matches);
        total_matches.push_back(matches);
    }

    matches.clear();


}


void Panorama::refine_match(std::vector<std::vector<cv::DMatch>> &total_matches,
                            std::vector<std::vector<cv::DMatch>> &refined_matches) {

    float dist, min_dist;
    int ii = 0, ij = 0;

    vector<DMatch> refined;
    vector<float> min_distances;

    for (int i = 0; i < total_matches.size(); i++) {

        min_dist = total_matches[i][0].distance;

        for (int j = 0; j < total_matches[i].size(); j++) {
            dist = total_matches[i][j].distance;

            if (dist < min_dist) {
                min_dist = dist;
                ii = i;
                ij = j;
            }
        }

        min_distances.push_back(min_dist);
    }

    // -- Refine using ratio
    for (int i = 0; i < total_matches.size(); i++) {

        for (int j = 0; j < total_matches[i].size(); j++) {

            dist = total_matches[i][j].distance;

            if (dist <= min_distances[i] * threshold_matching) {

                refined.push_back(total_matches[i][j]);

            }

        }

        refined_matches.push_back(refined);
        refined.clear();


    }


}

void Panorama::find_homography(std::vector<std::vector<cv::KeyPoint>> &keypoints,
                               std::vector<std::vector<cv::DMatch>> &refined_matches,
                               std::vector<std::vector<cv::DMatch>> &final_matches) {

    vector<DMatch> good_matches;

    vector<Point2f> pts1, pts2;
    Mat h_mask;
    vector<Mat> total_h_mask;


    for (int i = 0; i < keypoints.size() - 1; i++) {


        for (int j = 0; j < refined_matches[i].size(); j++) {

            pts1.push_back(keypoints[i][refined_matches[i][j].queryIdx].pt);
            pts2.push_back(keypoints[i + 1][refined_matches[i][j].trainIdx].pt);

        }


        findHomography(pts1, pts2, RANSAC, 5, h_mask);
        total_h_mask.push_back(h_mask);

        pts1.clear();
        pts2.clear();

    }


    for (int i = 0; i < refined_matches.size(); i++) {

        for (int j = 0; j < refined_matches[i].size(); j++){

            if ((int)total_h_mask[i].Mat::at<uchar>(j,0)) {
                good_matches.push_back(refined_matches[i][j]);
            }

        }

        final_matches.push_back(good_matches);
        good_matches.clear();

    }

}

void Panorama::find_distances(std::vector<std::vector<float>> &total_distances,
                              std::vector<std::vector<cv::KeyPoint>> &keypoints,
                              std::vector<std::vector<cv::DMatch>> &final_matches) {

    Point2f p1, p2;
    float dist;
    vector<float> distances;

    for (int i = 0; i < keypoints.size() - 1; i++) {

        for (int j = 0; j < final_matches[i].size(); j++) {
            p1 = keypoints[i][final_matches[i][j].queryIdx].pt;
            p2 = keypoints[i+1][final_matches[i][j].trainIdx].pt;
            dist = (float)projected_images[i].cols - p1.x + p2.x;
            distances.push_back(dist);
        }

        total_distances.push_back(distances);
        distances.clear();

    }


}

void Panorama::merge_images(cv::Mat &panorama, std::vector<float> &total_mean_distances,
                            std::vector<std::vector<float>> &total_distances,
                            std::vector<std::vector<cv::KeyPoint>> &keypoints,
                            std::vector<std::vector<cv::DMatch>> &final_matches) {
    float dist, n_inliers;

    for (int i = 0; i < final_matches.size(); i++) {
        dist = 0;
        n_inliers = final_matches[i].size();

        for (int j = 0; j < n_inliers; j++) {
            dist += total_distances[i][j];
        }

        total_mean_distances.push_back(dist/n_inliers);

    }

    // -- Apply translation
    Mat shiftMat = Mat::zeros(Size(3, 2), CV_64F);
    shiftMat.at<double>(0, 0) = 1;
    shiftMat.at<double>(1, 1) = 1;

    Mat dst;
    panorama = images[0];

    for (int i = 0; i < images.size() - 1; i++) {
        shiftMat.at<double>(0, 2) = -total_mean_distances[i];
        warpAffine(images[i + 1], dst, shiftMat, Size(images[i + 1].cols - total_mean_distances[i], images[i + 1].rows), INTER_CUBIC, BORDER_CONSTANT, Scalar());
        hconcat(panorama, dst, panorama);
    }

}

void Panorama::create_panorama(cv::Mat &panorama) {

    vector<vector<KeyPoint>> k;
    vector<Mat> d;
    vector<vector<DMatch>> total_matches, refined_matches, final_matches;

    vector<vector<float>> total_distances;
    vector<float> mean_distances;

    cylindrical_projection();
    SIFT_extractor(k, d);
    BF_matcher(d, total_matches);
    refine_match(total_matches, refined_matches);
    find_homography(k, refined_matches, final_matches);
    find_distances(total_distances, k, final_matches);
    merge_images(panorama, mean_distances, total_distances, k, final_matches);

}

