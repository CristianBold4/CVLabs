#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    //colored image
    Mat img_color = imread("Lena_color.jpg");
    namedWindow("Lena color", WINDOW_NORMAL);
    imshow("Lena color", img_color);
    cout << "Image colored channels: " << img_color.channels() << "\n";
    char waitKeyCol = static_cast<char> (waitKey(0));
    cout << "Wait key colored image: " << waitKeyCol << "\n";

    //grayscale image
    Mat img_gray = imread("Lena_grayscale.jpg");
    namedWindow("Lena gray", WINDOW_NORMAL);
    imshow("Lena gray", img_gray);
    cout << "Image grayscaled channels: " << img_gray.channels() << "\n";
    char waitKeyGray = static_cast<char> (waitKey(0));
    cout << "Wait key colored image: " << waitKeyGray << "\n";
    return 0;
}
