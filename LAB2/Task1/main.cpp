#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

const char *source_path = "image.jpg";
const char *dest_path = "image_grayscale.jpg";

int main(int argc, char** argv)
{
    Mat src, gray_src ;
    src = imread(source_path);
    namedWindow("Img source", WINDOW_NORMAL);
    //display source image
    imshow("Img source", src);
    waitKey();

    //converting source image to gray scale
    cvtColor(src, gray_src , COLOR_BGR2GRAY);
    //save the image
    imwrite(dest_path, gray_src);

    return 0;
}
