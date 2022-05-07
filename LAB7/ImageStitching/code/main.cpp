#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "panoramic_utils.h"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/core.hpp"
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <opencv2/calib3d.hpp>
#include "Panorama.h"

using namespace std;
using namespace cv;


bool sort_pred ( const DMatch& m1_, const DMatch& m2_ )
{
    return m1_.distance < m2_.distance;
}


void compute_matching(const vector<Mat> input_images, float threshold_matching) {

    vector<Mat> cylindrical_proj;
    Mat c_proj1, c_proj2, img1, img2;
    float min_distance;

    Ptr<SIFT> ptr = SIFT::create();
    BFMatcher matcher;


    for (int i = 0; i < input_images.size() - 1; i++) {
        img1 = input_images[i];
        img2 = input_images[i+1];
        c_proj1 = cylindricalProj(img1, 54);
        c_proj2 = cylindricalProj(img2, 54);

        // -- extract SIFT features

        vector<KeyPoint> k1, k2;
        ptr->detect(c_proj1, k1);
        ptr->detect(c_proj2, k2);

        Mat d1, d2;
        ptr->compute(c_proj1, k1, d1);
        ptr->compute(c_proj2, k2, d2);

        vector<DMatch> matches, closest;
        matcher.match(d1, d2, matches);

        sort(matches.begin(), matches.end(), sort_pred);

        min_distance = matches[0].distance;

        for( int i=0; i < matches.size(); i++) {
            if (matches[i].distance <= min_distance * threshold_matching) {
                closest.push_back(matches[i]);
            }
        }

        //-- Localize the object
        std::vector<Point2f> vec_img1;
        std::vector<Point2f> vec_img2;
        for( size_t i = 0; i < closest.size(); i++ )
        {
            // -- Get the keypoints from the closest matches
            vec_img1.push_back( k1[closest[i].queryIdx].pt );
            vec_img2.push_back( k2[closest[i].trainIdx].pt );
        }

        // -- drawing the results

        Mat cvmImgMatches;

        cv::drawMatches( input_images[i], k1, input_images[i+1], k2, closest, cvmImgMatches);

        namedWindow("matches", 0);
        imshow("matches", cvmImgMatches);
        waitKey();

        Mat H, masked;
        H = findHomography( vec_img1, vec_img2, RANSAC, 5,  masked);

        // -- Use the Homography Matrix to warp the images
        cv::Mat result;
        warpPerspective(img1,result,H,cv::Size(img1.cols+img2.cols,img1.rows));

        imshow( "Result", result );
        waitKey();

        cv::Mat half(result,cv::Rect(0,0,img2.cols,img2.rows));
        img2.copyTo(half);

        imshow( "Result", result );
        waitKey();

        break;


    }
}

void read_filesystem(string path, vector<Mat> &vec_images) {
    Mat src;
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        src = imread(entry.path().string(), IMREAD_UNCHANGED);

        // Check if image is loaded fine
        if (src.empty()) {
            cerr << ("Error opening image!\n");
        } else {
            vec_images.push_back(src); // Load an image
        }
    }
}


int main(int argc, char ** argv) {

    const string dataset[5] = {"../dataset/dolomites/", "../dataset/kitchen/", "../dataset/data/", "../dataset/lab_19_automatic", "../dataset/lab_19_manual"};
    // read src image
    const std::string path = dataset[0];
    double FOV = 66;
    double threshold_matching = 2.5;

    // -- vector where to store the input images
    std::vector<Mat> vec_images;

    // read the input dataset
    read_filesystem(path, vec_images);

    // -- create the panorama
    Mat panorama;
    Panorama p (vec_images, FOV, threshold_matching);
    p.create_panorama(panorama);

    // -- show the final panorama
    imshow("final", panorama);
    waitKey();

    return 0;

}