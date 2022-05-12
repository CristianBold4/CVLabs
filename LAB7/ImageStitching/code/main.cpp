#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "Panorama.h"

using namespace std;
using namespace cv;

int main(int argc, char ** argv) {

    const string dataset[] = {"../dataset/dolomites/", "../dataset/kitchen/", "../dataset/data/", "../dataset/lab_19_automatic", "../dataset/lab_19_manual"};
    const std::string path = dataset[1];
    double FOV = 66;
    double threshold_matching = 2.5;

    // -- create the panorama
    Mat panorama;
    Panorama p (path, FOV, threshold_matching);
    p.create_panorama(panorama);

    // -- show the final panorama
    imshow("final", panorama);
    waitKey();

    // -- write the final image
    imwrite("..//output//kitchen.png", panorama);

    return 0;

}