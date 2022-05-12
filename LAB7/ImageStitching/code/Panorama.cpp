//
// Created by Cristian on 06/05/2022.
//

#include "Panorama.h"
#include "panoramic_utils.h"
#include <vector>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <filesystem>


using namespace std;
using namespace cv;

// - Constructor
Panorama::Panorama(std::string dataset_path, const double FOV, const double threshold_matching) {
    load_images(dataset_path);
    this->FOV = FOV;
    this->threshold_matching = threshold_matching;
}

void Panorama::load_images(std::string dataset_path) {
    Mat src;
    for (const auto &entry: std::filesystem::directory_iterator(dataset_path)) {
        src = imread(entry.path().string(), IMREAD_UNCHANGED);

        // Check if image is loaded fine
        if (src.empty()) {
            cerr << "Error opening image: " << entry.path()  << "\n";
        } else {
            this->images.push_back(src); // Load an image
        }
    }
}

void Panorama::cylindrical_projection() {
    for (int i = 0; i < images.size(); i++) {
        projected_images.push_back(cylindricalProj(images[i], FOV / 2));
    }

    cout << "Cylindrical projection done..." << endl;
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

    cout << "Features extracted..." << endl;

}

void Panorama::BF_matcher(std::vector<cv::Mat> &descriptors, std::vector<std::vector<cv::DMatch>> &total_matches) {

    vector<DMatch> matches;
    Ptr<BFMatcher> BF_matcher = BFMatcher::create(NORM_L2, false);

    for (int i = 0; i < projected_images.size() - 1; i++) {

        BF_matcher->match(descriptors[i], descriptors[i+1], matches);
        total_matches.push_back(matches);
    }

    matches.clear();

    cout << "Matching done..." << endl;

}


bool Panorama::sort_matching(const cv::DMatch &m1, const cv::DMatch &m2) {

    return m1.distance < m2.distance;
}


void Panorama::refine_match(std::vector<std::vector<cv::DMatch>> &total_matches,
                            std::vector<std::vector<cv::DMatch>> &refined_matches) {



    vector<DMatch> distances, refined;
    double min_dist;

    for (int i = 0; i < total_matches.size(); i++) {
        distances = total_matches[i];
        sort(distances.begin(), distances.end(), sort_matching);
        min_dist = distances[0].distance;

        for (int i = 0; i < distances.size(); i++) {
            if (distances[i].distance > threshold_matching * min_dist) {
                break;
            }
            refined.push_back(distances[i]);
        }

        refined_matches.push_back(refined);
        refined.clear();
        distances.clear();
    }

    cout << "Matching refined..." << endl;

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

            // -- filter inliers
            if (total_h_mask[i].at<unsigned char>(j,0)) {
                good_matches.push_back(refined_matches[i][j]);
            }

        }

        final_matches.push_back(good_matches);
        good_matches.clear();

    }

    cout << "RANSAC applied..." << endl;

}

void Panorama::find_distances(std::vector<std::vector<float>> &total_distances_x,
                              std::vector<std::vector<float>> &total_distances_y,
                              std::vector<std::vector<cv::KeyPoint>> &keypoints,
                              std::vector<std::vector<cv::DMatch>> &final_matches) {

    Point2f p1, p2;
    float dist_x, dist_y;
    vector<float> distances_x, distances_y;

    for (int i = 0; i < keypoints.size() - 1; i++) {

        for (int j = 0; j < final_matches[i].size(); j++) {
            p1 = keypoints[i][final_matches[i][j].queryIdx].pt;
            p2 = keypoints[i+1][final_matches[i][j].trainIdx].pt;
            dist_x = (float)projected_images[i].cols - p1.x + p2.x;
            dist_y = abs(p1.y - p2.y);
            distances_x.push_back(dist_x);
            distances_y.push_back(dist_y);
        }

        total_distances_x.push_back(distances_x);
        total_distances_y.push_back(distances_y);
        distances_x.clear();
        distances_y.clear();

    }

    cout << "Distances computed..." << endl;


}

void Panorama::stitch_images(cv::Mat &panorama, std::vector<float> &total_mean_distances_x,
                             std::vector<float> &total_mean_distances_y,
                             std::vector<std::vector<float>> &total_distances_x,
                             std::vector<std::vector<float>> &total_distances_y,
                             std::vector<std::vector<cv::KeyPoint>> &keypoints,
                             std::vector<std::vector<cv::DMatch>> &final_matches) {


    float dist_x, dist_y, n_inliers;

    for (int i = 0; i < final_matches.size(); i++) {
        dist_x = 0, dist_y = 0;
        n_inliers = final_matches[i].size();

        for (int j = 0; j < n_inliers; j++) {
            dist_x += total_distances_x[i][j];
            dist_y += total_distances_y[i][j];
        }

        total_mean_distances_x.push_back(dist_x/n_inliers);
        total_mean_distances_y.push_back(dist_y/n_inliers);

    }

    // -- Apply translation
    Mat shiftMat = Mat::zeros(Size(3, 2), CV_64F);
    shiftMat.at<double>(0, 0) = 1;
    shiftMat.at<double>(1, 1) = 1;


    Mat dst;
    panorama = images[0];

    for (int i = 0; i < images.size() - 1; i++) {
        shiftMat.at<double>(0, 2) = -total_mean_distances_x[i];
        //shiftMat.at<double>(1, 2) = total_mean_distances_y[i];
        warpAffine(images[i + 1], dst, shiftMat, Size(images[i + 1].cols - total_mean_distances_x[i], images[i + 1].rows), INTER_CUBIC, BORDER_CONSTANT, Scalar());
        hconcat(panorama, dst, panorama);
    }

    cout << "Images stitched..." << endl;

}

void Panorama::create_panorama(cv::Mat &panorama) {

    vector<vector<KeyPoint>> k;
    vector<Mat> d, H;
    vector<vector<DMatch>> total_matches, refined_matches, final_matches;

    vector<vector<float>> total_distances_x, total_distances_y;
    vector<float> mean_distances_x, mean_distances_y;

    cylindrical_projection();
    SIFT_extractor(k, d);
    BF_matcher(d, total_matches);
    refine_match(total_matches, refined_matches);
    find_homography(k, refined_matches, final_matches);
    find_distances(total_distances_x, total_distances_y, k, final_matches);
    stitch_images(panorama, mean_distances_x, mean_distances_y, total_distances_x, total_distances_y, k, final_matches);

}

