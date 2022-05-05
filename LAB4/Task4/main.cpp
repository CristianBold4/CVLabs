#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void designCircles(const Mat &src, Mat &final) {
    vector<Vec3f> circles;
    HoughCircles(src, circles, HOUGH_GRADIENT, 1,
                 src.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, 1, 30 // change the last two parameters
    );

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle( final, center, 1, Scalar(0,100,100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle( final, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }
}

int main(int argc, char** argv) {

    const char *source_path = "../src/street_scene.png";

    // Declare the Mat variables
    Mat src, src_gray, blurred_gray, masked_img, final;

    // Loads an image
    src = imread( source_path, IMREAD_COLOR);
    // Check if image is not empty
    if(src.empty()){
        cerr << "Error opening image!\n";
        return EXIT_FAILURE;
    }

    // convert to gray scale
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    // blur image
    int kernel_size = 5;
    GaussianBlur(src_gray, blurred_gray, Size(kernel_size, kernel_size), 0);


    // detect and design circles
    designCircles(blurred_gray, src);

    // show final results
    imshow("Detected circles", src);
    waitKey();


    return 0;
}
