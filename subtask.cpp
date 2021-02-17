#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    
}

int main(int argc, char** argv){

    Mat imsrc = imread("empty.jpg");
    if (imsrc.empty()){
        cout<<"Image not loaded"<<endl;
        return -1;
    }

    namedWindow("Win", 1);



    vector<Point2f> src_points;
    vector<Point2f> dst_points;
    dst_points.pushback(472,52);
    dst_points.pushback(472,832);
    dst_points.pushback(800,830);
    dst_points.pushback(800,52);

}