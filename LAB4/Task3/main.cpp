#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

struct Line {
    double m;
    double q;
    Point p1, p2;
};

void createRectMask(const Mat &src, Mat &masked_img, Point vertices[1][4]) {

    // init mask
    Mat mask = Mat::zeros(Size(src.cols, src.rows), CV_8U);
    const Point *ppt[1] = {vertices[0]};
    int npt[] = {4};
    fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255), LINE_8);

    bitwise_and(src, mask, masked_img);

}

void colorArea(const Mat &src, vector<Line> lines, int n_lines) {
    // if we want to select the right roadway, it is reasonable to consider the rightest lines
    int c = 0;
    bool flag1 = false, flag2 = false;
    Line l;
    Point vertices[1][4];
    for (size_t i = 0; i < n_lines && c < 4; i++) {
        l = lines[i];
        if (l.m >= 0 && l.p1.x > src.rows/3 && !flag1) { // \ line
            vertices[0][c++] = l.p1;
            vertices[0][c++] = l.p2;
            flag1 = true;
        }
        if (l.m < 0 && !flag2) {
            vertices[0][c++] = l.p2;
            vertices[0][c++] = l.p1;
            flag2 = true;
        }
    }

    const Point *ppt[1] = {vertices[0]};
    int npt[] = {4 };
    fillPoly(src, ppt, npt, 1, Scalar(0, 0, 255), LINE_8);

}

void designLines(const Mat &src, const Mat &edge_src, Mat &final, Point vertices[1][4]) {

    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(edge_src, lines, 2, CV_PI/180, 150, 0, 0 ); // runs the actual detection

    double m, q, a, b;
    float rho, theta;
    Point pd1, pd2;

    Mat line_src = Mat::zeros(Size(src.cols, src.rows), src.type());

    vector<Line> lines_pool;
    Line l;

    // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ ) {
        rho = lines[i][0], theta = lines[i][1];
        a = cos(theta), b = sin(theta);

        // compute design point
        m = (- a / b); // slope = - cos(theta) / sin(theta)
        q = rho / b; // intercept = rho / sin(theta)

        Point bottom_left = vertices[0][0];
        Point top_left = vertices[0][1];
        Point top_right = vertices[0][2];
        Point bottom_right = vertices[0][3];


        if (m <= 0) { // start line from the bottom left
            pd1 = Point(bottom_left.x, m * (bottom_left.x) + q);
            pd2 = Point((top_left.y - q) / m, top_left.y);
        } else { // start line from the bottom right
            pd1 = Point(bottom_right.x, m * (bottom_right.x) + q);
            pd2 = Point((top_right.y - q) / m, top_right.y);
        }

        l.m = m, l.q = q, l.p1 = pd1, l.p2 = pd2;
        lines_pool.push_back(l);


        line(line_src, pd1, pd2, Scalar(0, 0, 255), 3, LINE_AA);
    }

    addWeighted(src, 1, line_src, 0.8, 0, final);

    // color the area between lines
    colorArea(final, lines_pool, lines.size());

}

int main(int argc, char** argv)
{

    const char *source_path = "../src/street_scene.png";

    // Declare the Mat variables
    Mat src, src_gray, blurred_gray, canny_img, masked_img, final;

    // Loads an image
    src = imread( source_path, IMREAD_COLOR);
    // Check if image is not empty
    if(src.empty()){
        cerr << "Error opening image!\n";
        return EXIT_FAILURE;
    }

    // convert image to grayscale
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    // blur image
    int kernel_size = 5;
    GaussianBlur(src_gray, blurred_gray, Size(kernel_size, kernel_size), 0);

    // Edge detection
    Canny(blurred_gray, canny_img, 50, 200, 3);


    // computing polygonal mask, defining vertices
    Point vertices[1][4];
    vertices[0][0] = Point(0, src.rows);
    vertices[0][1] = Point(570, 190);
    vertices[0][2] = Point(620, 190);
    vertices[0][3] = Point(860, 375);

    // create a polygonal mask that detects the piece of roads
    createRectMask(canny_img, masked_img, vertices);

    designLines(src, masked_img, final, vertices);

    imshow("Lines detection", final);
    waitKey();


    return 0;
}
