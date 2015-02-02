#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

char * filename = new char[100];
Mat featuresUnclustered;
Mat descriptor;
Mat input;
SiftDescriptorExtractor detector;

int main( int argc, char** argv )
{

 cv::initModule_nonfree();
 cv::Mat output;

 for(int f=1;f<100;f+=50){

	 sprintf(filename,"/home/rahim/Desktop/Pattern/etc/Codes/Mine/Dataset/Train/Running/person01_running_d1_uncomp/image-%3d.jpeg",f);
	// const cv::Mat input_Mat = cv::imread( "%s/image%03d.png", file_directory, i );
	 // const cv::Mat input = cv::imread("/home/rahim/Desktop/Pattern/etc/Codes/Mine/Dataset/Train/Running/person01_running_d1_uncomp/image-017.jpeg", 0); //Load as grayscale             cv::SiftFeatureDetector detector;
 	 //const cv::Mat input = cv::imread("/home/rahim/Desktop/1.jpg", 0); //Load as grayscale
 	 printf("\n%s", filename);
	 input = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale
	// Ptr<FeatureDetector> featureDetector = FeatureDetector::create("SIFT");
	 std::vector<cv::KeyPoint> keypoints;
	 detector.detect(input, keypoints);
	 cv::drawKeypoints(input, keypoints, output);
	 cv::imwrite("sift_result.jpg", output);
	 detector.compute(input, keypoints,descriptor);
	 featuresUnclustered.push_back(descriptor);
	// printf("no_%i\n",f/50);
	 FileStorage fs("/home/rahim/Desktop/Keypoints.yml", FileStorage::WRITE);
	// write(fs, "keypoints_1", keypoints);
	 write(fs, "descriptors_1", descriptor);
	 fs.release();
 }
 //Construct BOWKMeansTrainer
 //the number of bags
 int dictionarySize=200;
  TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
  int retries=1;
  int flags=KMEANS_PP_CENTERS;
 //Create the BoW (or BoF) trainer
 BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
 //cluster the feature vectors
 Mat dictionary=bowTrainer.cluster(featuresUnclustered);
  FileStorage fs("dictionary.yml", FileStorage::WRITE);
 fs << "vocabulary" << dictionary;
 fs.release();
return 0;

 }

