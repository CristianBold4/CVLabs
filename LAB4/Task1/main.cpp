#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Struct that holds the passing of parameters through callback functions
struct CannyThresholdParam {
    Mat src;
    int* low_t;
    int* max_t;
    const char* win_name;
    const int kernel_size = 3;
};

// callback that regulates first trackbar (low threshold)
static void CannyThreshold_1(int, void* param)
{

    CannyThresholdParam* p = (CannyThresholdParam*) param;
    Mat src = p->src;

    Mat detected_edges, dst, src_gray;

    dst.create( src.size(), src.type() );
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, *p->low_t, *p->max_t, p->kernel_size );

    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);

    imshow( p->win_name, dst );

}

// callback that regulates second trackbar (max threshold)
static void CannyThreshold_2(int, void* param)
{

    CannyThresholdParam *p = (CannyThresholdParam*) param;
    Mat src = p->src;

    Mat detected_edges, dst, src_gray;

    dst.create( src.size(), src.type() );
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, *p->low_t, *p->max_t, p->kernel_size );

    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);

    imshow(p->win_name, dst );

}

int main(int argc, char ** argv) {

    const char *source_path = "../src/street_scene.png";
    const char* window_name = "Canny Edge Detector";


    // maximum values of trackbars
    const int max_lowThreshold = 300;
    const int max_maxThreshold = 300;

    // variables that regulate the values of thresholding
    int lowThreshold = 0;
    int maxThreshold = 0;

    Mat src, src_gray, detected_edges;

    // reading src image
    src = imread(source_path, IMREAD_COLOR);
    if( src.empty() ) {
        cerr << "Error loading image!\n";
        return -1;
    }

    // setting up the passing struct
    CannyThresholdParam param;
    param.src = src;
    param.low_t = &lowThreshold;
    param.max_t = &maxThreshold;
    param.win_name = window_name;

    // create window and trackbars
    namedWindow( window_name, WINDOW_NORMAL);
    createTrackbar( "MIN_T:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold_1, (void*)&param);
    CannyThreshold_1(0, (void*) &param);
    createTrackbar( "MAX_T:", window_name, &maxThreshold, max_maxThreshold, CannyThreshold_2, (void*) &param);
    CannyThreshold_2(0, (void*)&param);

    waitKey();

    return 0;

}
