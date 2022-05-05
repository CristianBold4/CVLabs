#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "include/thinning.h"

using namespace std;
using namespace cv;


void findSeedPoints(const Mat &src, Mat &dst, int threshold) {
    Mat src_gray;
    dst = Mat::zeros(Size(src.cols, src.rows), CV_8UC1);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    GaussianBlur(src_gray, src_gray, Size(3, 3), 0);
    for (int i = 0; i < src_gray.rows; i++) {
        for (int j = 0; j < src_gray.cols; j++) {
            if (src_gray.at<unsigned char>(i, j) <= threshold) {
                dst.at<unsigned char>(i, j) = 255;
            }
        }
    }
}

bool isKConnected (const Mat &src, int i, int j, int ksize) {
    int a = (ksize-1)/2;
    for (int x = max(0, i - a); x < min(src.rows, i + a); x++) {
        for (int y = max(0, j - a); y < min(src.cols, j + a); y++) {
            if (src.at<unsigned char>(x, y) != 255) {
                return false;
            }
        }
    }

    return true;
}

void growSeedPoints(const Mat &src, const Mat& input, Mat& out) {

    out = Mat::zeros(Size(src.cols, src.rows), CV_8UC1);
    int threshold = 40;
    int kernel_size = 5;
    int a = (kernel_size - 1)/2;
    int current_b, current_r, current_g;
    int seed_b, seed_r, seed_g;
    int num_seeds_neigh;
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            if (input.at<unsigned char>(i, j) == 255) {
                // find seed point
                num_seeds_neigh = 0;
                out.at<unsigned char>(i, j) = 255;
                seed_b = (int) src.at<Vec3b>(i, j) [0];
                seed_g = (int) src.at<Vec3b>(i, j) [1];
                seed_r = (int) src.at<Vec3b>(i, j) [2];
                for (int x = max(0, i - a); x < min(src.rows, i + a); x++) {
                    for (int y = max(0, j - a); y < min(src.cols, j + a); y++) {
                        current_b = (int) src.at<Vec3b>(x, y) [0];
                        current_g = (int) src.at<Vec3b>(x, y) [1];
                        current_r = (int) src.at<Vec3b>(x, y) [2];
                        if (x != i && y != j && input.at<unsigned char>(x, y) == 255) {
                            // neighboring seed point
                            num_seeds_neigh++;
                        }
                        if (num_seeds_neigh >= 2 && abs(current_b - seed_b) < threshold && abs(current_g - seed_g) < threshold && abs(current_r - seed_r) < threshold) {
                            // grow point
                            out.at<unsigned char>(x, y) = 255;
                        }
                    }
                }
            }
        }
    }

}

void erodeFinal(const Mat &src, Mat &out, int ksize) {
    out = Mat::zeros(Size(src.cols, src.rows), CV_8UC1);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if (src.at<unsigned char>(i, j) == 255) {
                // seed point
                if (isKConnected(src, i, j, ksize)) {
                    out.at<unsigned char>(i, j) = 255;
                }
            }
        }
    }
}

int main(int argc, char ** argv) {

    const char* img_path = "../src/Asphalt-2.png";

    Mat src;

    // read src image
    src = imread(img_path, IMREAD_COLOR); // Load an image
    // Check if image is loaded fine
    if (src.empty()) {
        printf("Error opening image!\n");
        return EXIT_FAILURE;
    }

    imshow("Img", src);
    waitKey();


    Mat seed_array (src.rows, src.cols, CV_8UC1);
    //(src.rows, src.cols, CV_8UC1);
    findSeedPoints(src, seed_array, 30);


    imshow("Seed points", seed_array);
    waitKey();

    Mat thinned_seeds;
    skeletonization(seed_array, thinned_seeds);

    imshow("Thin", thinned_seeds);
    waitKey();

    Mat seeds_grown;
    growSeedPoints(src, thinned_seeds, seeds_grown);

    imshow("Seeds grown", seeds_grown);
    waitKey();

    Mat eroded;

    //erode(seeds_grown, eroded, getStructuringElement(MORPH_CROSS, Size(3, 3)));
    erodeFinal(seeds_grown, eroded, 3);
    imshow("eroded", eroded);
    waitKey();

    return 0;

}
