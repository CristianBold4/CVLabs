#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

const char *source_path = "../src/robocup.jpg";


int main() {
    Mat src = imread(source_path);
    namedWindow("Img src", WINDOW_NORMAL);
    imshow("Img src", src);
    waitKey();
    return 0;
}
