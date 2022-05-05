#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat img1(256, 256, CV_8U);
    Mat img2(256, 256, CV_8U);

    for (int i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            img1.at<unsigned char> (i, j) = i;
            img2.at<unsigned char> (i, j) = j;
        }
    }

    namedWindow("Vertical gradient");
    imshow ("Vertical gradient", img1);
    waitKey(0);


    namedWindow("Horizontal gradient");
    imshow ("Horizontal gradient", img2);
    waitKey(0);

    Mat chess1(300, 300, CV_8U);
    Mat chess2(300, 300, CV_8U);

    int intI, intJ;

    for (int i = 0; i < 300; ++i)
    {
        for (int j = 0; j < 300; ++j)
        {
            intI = static_cast<int> (i/20);
            intJ = static_cast<int> (j/20);
            if ((intI + intJ) % 2 == 0)
            {
                chess1.at<unsigned char> (i, j) = 0;
            }
            else
            {
                chess1.at<unsigned char> (i, j) = 255;
            }
            //same for chessboard 2
            intI = static_cast<int> (i/50);
            intJ = static_cast<int> (j/50);
            if ((intI + intJ) % 2 == 0)
            {
                chess2.at<unsigned char> (i, j) = 0;
            }
            else
            {
                chess2.at<unsigned char> (i, j) = 255;
            }
        }
    }

    namedWindow("Chessboard 1");
    imshow ("Chessboard 1", chess1);
    waitKey(0);


    namedWindow("Chessboard 2");
    imshow ("Chessboard 2", chess2);
    waitKey(0);



    return 0;
}
