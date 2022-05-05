#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

#define MAX_PADDING_VALUE 0
#define MIN_PADDING_VALUE 255

Mat set_padding(Mat img, int padding_value, int dim_kernel) {

    int rows = img.rows;
    int cols = img.cols;
    int dist_center = (dim_kernel-1)/2;
    Mat padded_img = Mat::ones(rows + dist_center*2, cols + dist_center*2, img.type()) * padding_value;

    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            padded_img.at<unsigned char> (i + dist_center, j + dist_center) = img.at<unsigned char> (i, j);
        }
    }

    return padded_img;

}


Mat max_filter(Mat src, int dim_kernel)
{
    Mat src_padded = set_padding(src, MAX_PADDING_VALUE, dim_kernel);

    Mat src_max = src_padded.clone();
    //check if the size of the kernel is odd
    if (dim_kernel % 2 == 1)
    {

        int height = src_padded.rows;
        int width = src_padded.cols;
        int dist_center = (dim_kernel-1)/2;
        for (int i = dist_center; i < height - dist_center; i++)
        {
            for (int j = dist_center; j < width - dist_center; j++)
            {
                int max_value = 0;
                for (int x = - dist_center; x <= dist_center; x++)
                {
                    for (int y = - dist_center; y <= dist_center; y++)
                    {
                        unsigned char pixel_value = src_padded.at<unsigned char>(i+x, j+y);
                        if (pixel_value > max_value)
                        {
                            max_value = pixel_value;
                        }
                    }
                }

                src_max.at<unsigned char>(i, j) = max_value;
            }
        }
    }
    else
    {
        std::cout << "Erorr: kernel dimension even!\n";
    }

    return src_max;
}

Mat min_filter(Mat src, int dim_kernel)
{
    Mat src_padded = set_padding(src, MIN_PADDING_VALUE, dim_kernel);

    Mat src_min = src_padded.clone();

    //check if the size of the kernel is odd
    if (dim_kernel % 2 == 1)
    {

        int height = src_padded.rows;
        int width = src_padded.cols;
        int dist_center = (dim_kernel-1)/2;
        for (int i = dist_center; i < height - dist_center; i++)
        {
            for (int j = dist_center; j < width - dist_center; j++)
            {
                int min_value = 255;
                for (int x = - dist_center; x <= dist_center; x++)
                {
                    for (int y = - dist_center; y <= dist_center; y++)
                    {
                        unsigned char pixel_value = src_padded.at<unsigned char>(i+x, j+y);
                        if (pixel_value <  min_value)
                        {
                            min_value = pixel_value;
                        }
                    }
                }

                src_min.at<unsigned char>(i, j) = min_value;
            }
        }
    }
    else
    {
        std::cout << "Erorr: kernel dimension even!\n";
    }

    return src_min;
}
