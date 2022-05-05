#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    //colored image
    Mat img = imread("Lena_color.jpg");
    int nChannels = img.channels();
    cout << "Image colored channels: " << nChannels << "\n";
    // "channels" is a vector of 3 Mat arrays:
    vector<Mat> channels(3);
    char ch;

    if (nChannels == 3)
    {
        // split img:
        split(img, channels);
        // get the channels (follow BGR order in OpenCV)
        Mat imgout0(img.rows, img.cols, CV_8UC1);
        Mat imgout1(img.rows, img.cols, CV_8UC1);
        Mat imgout2(img.rows, img.cols, CV_8UC1);

        imgout0 = channels[0];
        imgout1 = channels[1];
        imgout2 = channels[2];

        //channels[0] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set blue channel to 0
        //channels[1] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set green channel to 0
        //channels[2] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set red channel to 0
        //merge the image

        //merge(channels, imgout1);


        //show image

        namedWindow("Image out 0", 0);
        imshow("Image out 0", imgout0);
        waitKey(0);

        namedWindow("Image out 1", 0);
        imshow("Image out 1", imgout1);
        waitKey(0);

        namedWindow("Image out 2", 0);
        imshow("Image out 2", imgout2);
        waitKey(0);

    }



    return 0;
}
