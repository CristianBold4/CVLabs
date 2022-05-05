#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include "header.h"

using namespace cv;
using namespace std;

const char *source_path = "image.jpg";
const char *dest_path = "image_grayscale.jpg";

int main(int argc, char** argv)
{
    Mat src, gray_src ;
    src = imread(source_path);
    //namedWindow("Img source", WINDOW_NORMAL);
    //display source image
    //imshow("Img source", src);
    //waitKey();

    //converting source image to gray scale
    cvtColor(src, gray_src , COLOR_BGR2GRAY);
    //save the image
    imwrite(dest_path, gray_src);

    Mat max_src = max_filter(gray_src, 3);

    namedWindow("Img max filter", WINDOW_NORMAL);
    //display image post max filtering application
    imshow("Img max filter", max_src);
    waitKey();

    Mat min_src = min_filter(gray_src, 5);

    namedWindow("Img min filter", WINDOW_NORMAL);
    //display image post max filtering application
    imshow("Img min filter", min_src);
    waitKey();

    return 0;
}
