#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;


vector<Point2f> src_points; // Global Variable

void onMouseClick(int event, int x, int y, int flags, void* userdata)
{
    //capture points if left button pressed wiht control key pressed
    if( flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON) ){
        src_points.push_back(Point2f(x,y));
        cout<< "coordinate- ("<< x <<","<< y << ")"<< endl;
    }
}

int main(int argc, char** argv){
    
    //read the Image
    Mat imsrc = imread("empty.jpg");
    
    //check if the Image has loaded or not
    if (imsrc.empty()){
        cout<<"Image not loaded"<<endl;
        return -1;
    }
    
    // converting the colored image to grayscale 
    Mat gray_img;
    cvtColor(imsrc,gray_img,COLOR_BGR2GRAY);
    
    namedWindow("Win", 1);
    imshow("Win",gray_img);

    //projecting the grayscaled image to the required image 
    //vector<Point2f> src_points;
    
    //give the instructions to click the points
    cout<<"click 4 points on Win in the following order and holding the CNTRL key"<<endl;
    cout<<"top-left"<<endl;
    cout<<"bottom-left"<<endl;
    cout<<"bottom-right"<<endl;
    cout<<"top-right"<<endl;
    
    while(src_points.size()<4){
        setMouseCallback("Win",onMouseClick,0);
    }
    
    vector<Point2f> dst_points;
    dst_points.push_back(Point2f(472,52));
    dst_points.push_back(Point2f(472,832));
    dst_points.push_back(Point2f(800,830));
    dst_points.push_back(Point2f(800,52));
    
    Mat homography = findHomography(src_points,dst_points);
    Mat im_out;
    warpPerspective(gray_img,im_out,homography,gray_img.size());
    
    //show the image projected
    imshow("projected image",im_out);
    //save the projected image
    bool check1 = imwrite("projected.jpg",im_out);
    if(check1 == false){
        cout<<"Image could not be saved"<<endl;
    }

    //croping the image
    int top_left_x = 472;
    int top_left_y = 52;
    int width = 800 - 472;
    int height = 830 - 52;
    Rect cropped_img(top_left_x,top_left_y,width,height);
    Mat im_crop = im_out(cropped_img);

    //show the image cropped 
    imshow("cropped image",im_crop);
    bool check2 = imwrite("cropped.jpg",im_crop);
    if(check2 == false){
        cout<<"Image could not be saved"<<endl;
    }

    waitKey(0);

    destroyAllWindows();    
    return 0;

}
