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

        // get the channels (follow BGR order in OpenCV)
        while (ch != 'Q')
        {
            cout << "Select the channel to be set to 0 (options: BGR). Press Q to exit" << "\n";
            cin >> ch;
            Mat img = imread("Lena_color.jpg");
            // split img:
            split(img, channels);
            switch (ch)
            {
            case 'B':
                channels[0] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set blue channel to 0
                break;
            case 'G':
                channels[1] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set green channel to 0
                break;
            case 'R':
                channels[2] = Mat::zeros(img.rows, img.cols, CV_8UC1); //Set red channel to 0
                break;


            }
            //merge the image
            merge(channels, img);
            //show image
            namedWindow("Lena color", WINDOW_NORMAL);
            imshow("Lena color", img);
            waitKey(0);
        }
    }


    return 0;
}
