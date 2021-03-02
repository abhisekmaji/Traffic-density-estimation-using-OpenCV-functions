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
    if( event == EVENT_LBUTTONDOWN ){
        src_points.push_back(Point2f(x,y));
        cout<< "coordinate- ("<< x <<","<< y << ")"<< endl;
    }
    if(src_points.size() ==4){
        destroyWindow("Win");
        return;
    }
}

void setPoints(){
    
    //set the area of interest for the frame to be proojected and cropped

    Mat imsrc = imread(final_path);
    
    //check if the Image has loaded or not
    if (imsrc.empty()){
        cout<<"Image not loaded"<<endl;
        return -1;
    }

    // converting the colored image to grayscale 
    Mat gray_img;
    cvtColor(imsrc,gray_img,COLOR_BGR2GRAY);
    //resize(gray_img,gray_img,Size(gray_img.cols/3,gray_img.rows/3));
    namedWindow("Win",0);
    resizeWindow("Win",1000,1000);
    imshow("Win", gray_img);
    
    
    //projecting the grayscaled image to the required image 
    
    
    //give the instructions to click the points
    
    cout<<"click 4 points on Win "<<endl;
    cout<<"top-left"<<endl;
    cout<<"bottom-left"<<endl;
    cout<<"bottom-right"<<endl;
    cout<<"top-right"<<endl;
    

    setMouseCallback("Win",onMouseClick,0);
}

Mat project_crop(Mat imsrc){
    
    Mat gray_img;
    cvtColor(imsrc,gray_img,COLOR_BGR2GRAY);
    
    //projecting the grayscaled image to the required image     
    vector<Point2f> dst_points;
    dst_points.push_back(Point2f(472,52));
    dst_points.push_back(Point2f(472,832));
    dst_points.push_back(Point2f(800,830));
    dst_points.push_back(Point2f(800,52));
    
    Mat homography = findHomography(src_points,dst_points);
    Mat im_out;
    warpPerspective(gray_img,im_out,homography,gray_img.size());
    
    //croping the image
    int top_left_x = 472;
    int top_left_y = 52;
    int width = 800 - 472;
    int height = 830 - 52;
    Rect cropped_img(top_left_x,top_left_y,width,height);
    Mat im_crop = im_out(cropped_img);
    return im_crop;
}

int main(int argc, char** argv){
    if(argv[1]==NULL){
        cout<<"enter the image to be processed"<<endl;
        return 0;
    }
    string vid_path = argv[1];
    string final_path = vid_path + ".mp4";
    
    VideoCapture cap(final_path);
    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Could not open the video file" << endl;
        return -1;
    }

    while(1){
        Mat frame;
        cap >> frame;
        if(frame.empty()){
            break;
        }
        Mat croped = project_crop(frame);
        imshow("frame", frame);
        char c = (char)waitkey(25);
        if(c==q||c=27){
            break;
        }
    }
    return 0;
}
