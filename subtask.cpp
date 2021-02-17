#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    
}

int main(int argc, char** argv){
    
    \\read the Image
    Mat imsrc = imread("empty.jpg");
    
    \\check if the Image has loaded or not
    if (imsrc.empty()){
        cout<<"Image not loaded"<<endl;
        return -1;
    }
    
    \\ converting the colored image to grayscale 
    Mat gray_img;
    cvtcolor(imsrc,gray_img,COLOR_BGR2GRAY);
    
    namedWindow("Win", 1);
    imshow("Win",gray_img);

    \\projecting the grayscaled image to the required image 
    vector<Point2f> src_points;
    vector<Point2f> dst_points;
    dst_points.pushback(472,52);
    dst_points.pushback(472,832);
    dst_points.pushback(800,830);
    dst_points.pushback(800,52);
    
    return 0;

}
