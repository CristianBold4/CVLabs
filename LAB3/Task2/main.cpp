#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

const char *source_path = "../src/robocup.jpg";



int calc_mean (int ir, int ic, int channel, Mat img, int dim_filter) {
    int a = (dim_filter-1)/2;
    int sum = 0;
    for (int i = max(0, ir-a); i <= min(img.rows - 1, ir+a); i++) {
        for (int j = max(0, ic-a); j <= min(img.cols - 1, ic+a); j++) {
            sum += (int) img.at<Vec3b>(i, j)[channel];
        }
    }

    return (int) sum/(dim_filter*dim_filter);
}

void create_new_image(Mat img, Mat mask) {
    Mat new_img = img.clone();
    for (int i = 0; i < new_img.rows; i++) {
        for (int j = 0; j < new_img.cols; j++) {
            if (mask.at<unsigned char>(i, j) == 255) {
                new_img.at<Vec3b>(i, j)[0] = 92;
                new_img.at<Vec3b>(i, j)[1] = 37;
                new_img.at<Vec3b>(i, j)[2] = 201;
            }
        }
    }

    //show new img
    imshow("New image", new_img);
    waitKey();

}

void select_areas(Mat img, int b_mean, int g_mean, int r_mean) {
    Mat mask (img.rows, img.cols, CV_8U, 0.0);

    const int T = 60;

    int current_b, current_g, current_r;

    for (int i=0; i<img.rows; i++) {
        for (int j=0; j<img.cols; j++) {

            current_b = (int) img.at<Vec3b>(i, j) [0];
            current_g = (int) img.at<Vec3b>(i, j) [1];
            current_r = (int) img.at<Vec3b>(i, j) [2];

            if (abs(current_b - b_mean) < T && abs(current_g - g_mean) < T && abs(current_r - r_mean) < T) {
                mask.at<unsigned char>(i, j) = 255; // white pixel setted
            }
        }
    }

    //show mask img
    //namedWindow("Mask", WINDOW_NORMAL);
    imshow("Mask", mask);
    waitKey();
    create_new_image(img, mask);
}




void call_back(int event, int x, int y, int flags, void* param) {
    Mat* img = (Mat*) param;
    if (event == EVENT_LBUTTONDOWN) {
        //cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        int b, g, r;
        b = (int) img->at<Vec3b>(y, x)[0];
        g = (int) img->at<Vec3b>(y, x)[1];
        r = (int) img->at<Vec3b>(y, x)[2];
        printf("Position (%d, %d), B: %d G: %d R: %d\n", y, x, b, g, r);
        int b_neigh_mean = calc_mean(y, x, 0, *img, 9);
        int g_neigh_mean = calc_mean(y, x, 1, *img, 9);
        int r_neigh_mean = calc_mean(y, x, 2, *img, 9);
        printf("Mean of neighborhood 9x9 (%d, %d), B: %d G: %d R: %d\n", y, x, b_neigh_mean, g_neigh_mean, r_neigh_mean);
        select_areas(*img, b_neigh_mean, g_neigh_mean, r_neigh_mean);
    }
}



int main() {
    Mat src = imread(source_path);
    //if fail to read the image
    if ( src.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }

    namedWindow("Img src", WINDOW_NORMAL);

    setMouseCallback("Img src", call_back, &src);

    imshow("Img src", src);
    waitKey();

    return 0;
}
