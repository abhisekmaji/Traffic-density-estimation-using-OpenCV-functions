#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <chrono>

using namespace cv;
using namespace std;

#define ul unsigned long
#define ARRAY_SIZE 10000

// Global Variables
float* a;       				//to store the queue density
unsigned long mag_g = 0;		//to store   
int NUM_THREADS;				//variable to store number of threads taken as command input
vector<Point2f> src_points; 	//to store 4-coordinates for density analysis

//data-structure to store arguments and result of function passed to a thread
struct arg_struct {
    Mat img;
    int idx;
    ul result;
};

/*---------------------------------------------------
| Initializing the array for queue density storage	|
---------------------------------------------------*/
void init(){ 
    a = (float *)malloc(sizeof(float)*ARRAY_SIZE);
}

/*-----------------------------------------------------------
| Calculate the magnitude of traffic to calcultae density	|
-----------------------------------------------------------*/
ul cal_magnitude(Mat diff){
    float threshold = 30.0f;
    float dist;

    ul mag = 0;

    for(int i = 0; i < diff.rows; ++i)
        for(int j = 0; j < diff.cols; ++j)
        {
            Vec3b pixelMatrix = diff.at<Vec3b>(i,j);
            dist = sqrt( pixelMatrix[0]*pixelMatrix[0]
                        +pixelMatrix[1]*pixelMatrix[1]
                        +pixelMatrix[2]*pixelMatrix[2]
                        );
            if(dist>threshold) mag ++;
        }
    return mag;
}

/*---------------------------------------------------------------------------
| Helper Function to pass arguments and store results of the function       | 
| (cal_magnitude) called by the thread										|
---------------------------------------------------------------------------*/
void* process(void* arg){
    struct arg_struct *args = (struct arg_struct *)arg;
    Mat img = args->img;
    args->result = cal_magnitude(img);
    pthread_exit(NULL);
    //return (void *)cal_magnitude(img, p, q);
}

/*--------------------------------------------------
| helper function to detect the mouse click events |
--------------------------------------------------*/
void onMouseClick(int event, int x, int y, int flags, void* userdata){
    //capture points if left button pressed wiht control key pressed
    if( event == EVENT_LBUTTONDOWN ){
        src_points.push_back(Point2f(x,y));
        /*cout<< "coordinate- ("<< x <<","<< y << ")"<< endl;*/
    }
    if(src_points.size() == 4){
        destroyWindow("Win");
        return;
    }
}

/*--------------------------------------------------
| Function to take 4 points from the user in       |
| order to apply projection and cropping to that   |
| particular region                                |
--------------------------------------------------*/
void setPoints(Mat imgsrc){    
    //set the area of interest for the frame to be proojected and cropped
    
    //converting the colored image to grayscale 
    Mat gray_img;
    cvtColor(imgsrc,gray_img,COLOR_BGR2GRAY);
    
    //resize(gray_img,gray_img,Size(gray_img.cols/3,gray_img.rows/3));
    namedWindow("Win",0);
    resizeWindow("Win",1000,1000);
    imshow("Win", gray_img);
         
    //give the instructions to click the points    
    cout<<"click 4 points on Win"<<endl;
    cout<<"top-left"<<endl;
    cout<<"bottom-left"<<endl;
    cout<<"bottom-right"<<endl;
    cout<<"top-right"<<endl;
    setMouseCallback("Win",onMouseClick,0);
    waitKey(0);
    return;
}

/*--------------------------------------------------
| Function to correct the camera angle and crop    |
| the image.                                       |
--------------------------------------------------*/
Mat project_crop(Mat imsrc){
    Mat gray_img;
    cvtColor(imsrc,gray_img,COLOR_BGR2GRAY);
    
    //projecting the grayscaled image to the required image     
    vector<Point2f> dst_points;
    dst_points.push_back(Point2f(472,52));
    dst_points.push_back(Point2f(472,832));
    dst_points.push_back(Point2f(800,830));
    dst_points.push_back(Point2f(800,52));
    
    //calculating homograph 
    Mat homography = findHomography(src_points, dst_points);
    Mat im_out;
    warpPerspective(gray_img, im_out, homography, gray_img.size());

    //croping the image
    int top_left_x = 472;
    int top_left_y = 52;
    int width = 800 - 472;
    int height = 830 - 52;
    Rect cropped_img(top_left_x, top_left_y, width, height);
    Mat im_crop = im_out(cropped_img);
    return im_crop;
}

/*--------------------------------------------------
| +++++++++++++++++ MAIN ++++++++++++++++++++++++++|
--------------------------------------------------*/
int main(int argc, char** argv){
    
    init();	//initialize array to store density values
    
    //set clock to measure video processing time
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
	
	//take video name from command line arguments
    if(argv[1]==NULL){
        cout<<"enter the video to be processed"<<endl;
        return 0;
    }
    string vid_path = argv[1];
    string final_path = vid_path + ".mp4";
    cout<<"Video path received"<<endl;
    
    //take number of threads from command line arguments
    char *arg2 = argv[2];
    NUM_THREADS = atoi(arg2);
    cout<<"Number of threads received"<<endl;
    
    VideoCapture cap(final_path);
    cout<<"Video capture created"<<endl;
    
    //check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Could not open the video file" << endl;
        return -1;
    }
    
    //created frames for processing pictures
    Mat frame, cropped, empty;
    cap >> empty;
    if(empty.empty()){
    	cout<<"video capture failed"<<endl;
    	return -1;
    }
    
    /*set the 4 coordinates*/
    setPoints(empty);
    /*
    src_points.push_back(Point2f(854,270));
    src_points.push_back(Point2f(579,1072));
    src_points.push_back(Point2f(1574,1068));
    src_points.push_back(Point2f(1328,274));
    */

    //project empty frame using homography
    empty = project_crop(empty);
    
    //create window to show frames being proceesed 
    //(if required, but not used while calculating time for video processing)
    namedWindow("frame",0);
    resizeWindow("frame",300,500);
    
    //paratmeters for video processing
    pthread_t threads[NUM_THREADS];			//array to store thread references
    struct arg_struct arg[NUM_THREADS]; 	//array to contain arguments of threads
    int j = 0;								//thread identifier
    int k = 0;								//frame number
    int cnt = 0;							//helper parameter

	//start timer
    auto t1 = high_resolution_clock::now();
    
    cout<<"Video processing started"<<endl;
    while(k < ARRAY_SIZE){
        //capture new frame
        cap >> frame;
        if(frame.empty()) break;

        if(cnt == 15){
			cropped = project_crop(frame);		//correction of camera anlge and cropping
            Mat dst;							//matrix to store subtracted image
            absdiff(cropped,empty,dst);			//subtract
            
            //argument setting for thread function
            arg[j].img = dst;		//difference image
            arg[j].idx = k;			//frame number
            
            //check thread creation
            int check  = pthread_create(&threads[j], NULL, process, (void *)&arg[j]);
            if (check != 0) {
                cout<<"oh no!! thread could not be created\n";
                return -1;
            }
            
      		//loop parameter update
            j++;
            k++;
            cnt = 0;
            
            //calculate and store density calculation after all threads complete work 
            if(j == NUM_THREADS){
                j = 0;
                for(int z = 0; z < NUM_THREADS; z++)
                    pthread_join(threads[z], NULL);
                for(int z = 0; z < NUM_THREADS; z++){
                    ul mag = arg[z].result;
                    mag_g = arg[z].img.rows*arg[z].img.cols;
                    float mag1 = (mag*1.0)/(mag_g*1.0);
                    a[arg[z].idx] = mag1;
                }
            }
        }
        else cnt++;

        /*
        //display new cropped frame
        imshow("frame", cropped);
        char c = (char)waitKey(25);
        if(c=='q'||c==27){
            break;
        }
        */
    }
    for(int z = 0; z < j; z++)
        pthread_join(threads[z], NULL);
    for(int z = 0; z < j; z++){
        ul mag = arg[z].result;
        mag_g = arg[z].img.rows*arg[z].img.cols;
        float mag1 = (mag*1.0)/(mag_g*1.0);
        a[arg[z].idx] = mag1;
    }
    cout<<"Video proceesed"<<endl;

	//mark stop time and display run time
    auto t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  	std::cout << "Time: " << time_span.count() << " sec\n";

	//store density calculation in .txt file
    ofstream myfile("m4.txt");
    if (myfile.is_open())
    {
        for(int count = 0; count < k; count++){
            myfile << a[count] << "\n";
        }
        myfile.close();
    }
    else cout << "Unable to open file";
	
	//free dynamic array and return
    free(a);
    return 0;
}


