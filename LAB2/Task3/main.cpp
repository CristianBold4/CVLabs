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


    //converting source image to gray scale
    cvtColor(src, gray_src, COLOR_BGR2GRAY);
    //save the image
    imwrite(dest_path, gray_src);

    namedWindow("Img source", WINDOW_NORMAL);
    //display grayscale image
    imshow("Img source", gray_src);
    waitKey();
    //MEDIAN FILTERING
    Mat median_dst;
    medianBlur(gray_src, median_dst, 9);

    namedWindow("Img after median filtering", WINDOW_NORMAL);
    imshow("Img after median filtering", median_dst);
    waitKey();

    //GAUSSIAN FILTERING
	Mat gaussian_dst;
    GaussianBlur(gray_src, gaussian_dst, Size(9, 9), 0, 0);

    namedWindow("Img after Gaussian filtering", WINDOW_NORMAL);
    imshow("Img after Gaussian filtering", gaussian_dst);
    waitKey();


    return 0;
}
