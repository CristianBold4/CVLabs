#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

const char *source_path = "image.jpg";
const char *dest_path = "image_grayscale.jpg";




Mat calc_histogram(Mat src, int nbins)
{
    Mat hist;
    hist = Mat::zeros(nbins, 1, CV_32F);
    src.convertTo(src, CV_32F);
    double value = 0;
    for	(int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            value = src.at<float> (i, j);
            hist.at<float>(value) = hist.at<float>(value) + 1;
        }
    }

    return hist;
}

void plot_histogram(Mat src, int nbins)
{
    int h_width = 400;
    int h_height = 512;
    Mat hist = calc_histogram(src, nbins);
    Mat histogram_image(h_width, h_height, CV_8U, Scalar(0, 0, 0));
    Mat normalized_hist;
    normalize(hist, normalized_hist, 0, 400, NORM_MINMAX, -1, Mat());

    for (int i = 0; i < nbins; i++)
    {
        rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_hist.at<float>(i)),
                  Point(2 * (i+1), histogram_image.rows),
                  Scalar(255));
    }

    namedWindow("Histogram", WINDOW_NORMAL);
    imshow("Histogram", histogram_image);

}


int main(int argc, char** argv)
{
    Mat src, gray_src ;
    src = imread(source_path);
    if( src.empty() )
    {
        return EXIT_FAILURE;
    }

    //converting source image to gray scale
    cvtColor(src, gray_src, COLOR_BGR2GRAY);
    //save the image
    imwrite(dest_path, gray_src);





    namedWindow("Img source", WINDOW_NORMAL);
    //display grayscale image
    imshow("Img source", gray_src);
    waitKey();

    //CALCULATING HISTOGRAM
	hist = plot_histogram(gray_src, 256);
    waitKey();


    return 0;
}
