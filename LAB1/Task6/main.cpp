#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    Point2f srcTri[3];
    Point2f dstTri[3];

    Mat rot_mat (2, 3, CV_32FC1);
    Mat warp_mat (2, 3, CV_32FC1);

    Mat src, warp_dst, rotate_dst;

    src = imread("Lena_color.jpg", IMREAD_COLOR);

    warp_dst = Mat::zeros(src.rows, src.cols, src.type());

    //set the source point
    srcTri[0] = Point2f (0, 0);
    srcTri[1] = Point2f (src.cols - 1.f, 0);
    srcTri[2] = Point2f (0, src.rows - 1.f);

    dstTri[0] = Point2f (src.cols*0.0f, src.rows*0.0f);
    dstTri[1] = Point2f (src.cols*0.85f, src.rows*0.25f);
    dstTri[2] = Point2f (src.cols*0.15f, src.rows*0.7f);

    warp_mat = getAffineTransform(srcTri, dstTri);

    warpAffine(src, warp_dst, warp_mat, warp_dst.size());

    Point center = Point (warp_dst.cols/2, warp_dst.rows/2);
    double angle = 90.0; //90 degree rotation counterclock-wise
    double scale = 1;

    rot_mat = getRotationMatrix2D(center, angle, scale);

    warpAffine (src, rotate_dst, rot_mat, warp_dst.size());

    namedWindow("Source image", WINDOW_NORMAL);
    resizeWindow("Source image", 900, 600);
    imshow("Source image", src);

    namedWindow("Warp source", WINDOW_NORMAL);
    resizeWindow("Warp source", 900, 600);
    imshow("Warp source", warp_dst);

    namedWindow("Rotated source", WINDOW_NORMAL);
    resizeWindow("Rotated source", 900, 600);
    imshow("Rotated source", rotate_dst);

    waitKey(0);

    return 0;
}
