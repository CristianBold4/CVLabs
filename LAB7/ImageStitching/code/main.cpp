#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "panoramic_utils.h"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/core.hpp"
#include <stdio.h>
#include <filesystem>

//using namespace std;
using namespace cv;


int main(int argc, char ** argv) {

    const std::string dataset[5]= {"../dataset/dolomites/", "kitchen", "data", "lab_19_automatic", "lab_19_manual"};

    Mat src, proj;

    // read src image
    const std::string path = "../dataset/dolomites/";
    char* num;
    int n_images = 23;
    std::vector<Mat> vec_images;
    std::vector<Mat> cylindrical_proj;
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        src = imread(entry.path().string(), IMREAD_UNCHANGED);

        // Check if image is loaded fine
        if (src.empty()) {
            printf("Error opening image!\n");
            return EXIT_FAILURE;
        } else {
            vec_images.push_back(src); // Load an image
            proj = cylindricalProj(src, 27);
            cylindrical_proj.push_back(proj);
        }
    }

    // extract SIFT features
    Mat gray_src;


    for (int i = 0; i < cylindrical_proj.size(); i++) {

        gray_src = (Size(cylindrical_proj[i].cols, cylindrical_proj[i].rows), CV_8UC1);
        cvtColor(cylindrical_proj[i], gray_src, COLOR_BGR2GRAY);



        Ptr<SIFT> siftPtr = SIFT::create();
        std::vector<cv::KeyPoint> keypoints;
        siftPtr->detect(gray_src, keypoints);

        // Add results to image and save.
        Mat output;
        drawKeypoints(gray_src, keypoints, output);
        imshow("sift_result.jpg", output);
    }



    return 0;

}
