#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "Panorama.h"

using namespace std;
using namespace cv;

int main(int argc, char ** argv) {

    const string dataset[] = {"dolomites", "kitchen", "data", "lab_19_automatic", "lab_19_manual"};

    cout << "Select the dataset by pressing the number: \n";
    cout << "0 -- dolomites\n1 -- kitchen\n2 -- data\n3 -- lab_19_automatic\n4 -- lab_19_manual\n";

    int i;
    cin >> i;

    cout << "Creating panorama for " << dataset[i] << "...\n";

    const std::string path = "../dataset/" + dataset[i] + "/";
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
    string filename = "..//output//" + dataset[i] + ".png";
    imwrite(filename, panorama);

    return 0;

}