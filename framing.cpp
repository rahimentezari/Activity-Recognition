#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "iostream"

using namespace cv;
using namespace std;
int main(int argc, char*argv[])
{

    char *my_file = "/home/rahim/Desktop/Pattern/running/person21_running_d1_uncomp.avi";
    std::cout<<"Video File "<<my_file<<std::endl;
    cv::VideoCapture input_video;
   // double frnb ( cap.get ( CV_CAP_PROP_FRAME_COUNT ) );
    //std::cout << "frame count = " << frnb << endl;

    if(input_video.open(my_file))
    {
         std::cout<<"Video file open "<<std::endl;

    }
    else
    {
        std::cout<<"Not able to Video file open "<<std::endl;

    }
   // namedWindow("My_Win",1);
    namedWindow("Segemented", 1);
    Mat cap_img;
    for(;;)
    {
         input_video >> cap_img;
         imshow("My_Win", cap_img);
          waitKey(0);
    }
   return 0;
 }
