#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;


// empirical default values
void detectWhiteMarking(const Mat &src, Mat &final, int threshold_white, int = 1, int = 2, int = 0, int = CV_32F);


int main(int argc, char **argv) {

    const char *img_path = "../src/street_scene.png";
    const String window_name = "Street Lines Edge Detector";

    Mat image, final;

    // read src image
    image = imread(img_path, IMREAD_COLOR); // Load an image
    // Check if image is loaded fine
    if (image.empty()) {
        printf("Error opening image!\n");
        return EXIT_FAILURE;
    }

    int threshold_white = 240;
    detectWhiteMarking(image, final, threshold_white);

    //show final result
    imshow(window_name, final);
    waitKey(0);

    return 0;
}

void detectWhiteMarking(const Mat &src, Mat &final, int threshold_white, int ksize, int scale, int delta, int ddepth) {

    Mat src_gray, src_blurred;

    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(src, src_blurred, Size(3, 3), 0, 0, BORDER_DEFAULT);
    // Convert the image to grayscale
    cvtColor(src_blurred, src_gray, COLOR_BGR2GRAY);

    // filter the image keeping only white pixels (above threshold)
    for (int i = 0; i < src_gray.rows; i++) {
        for (int j=0; j < src_gray.cols; j++) {
            if (src_gray.at<unsigned char>(i, j) < threshold_white) {
                src_gray.at<unsigned char>(i, j) = 0;
            }
        }
    }

    // use Sobel filters, compute gradients
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, final);

}
