#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace cv;



int main( int argc, char** argv )
{

 cv::initModule_nonfree();
 const cv::Mat input = cv::imread("/home/rahim/Desktop/1.jpg", 0); //Load as grayscale             cv::SiftFeatureDetector detector;
 //const Mat input = cv::imread("/home/rahim/Desktop/1.jpg");
 Ptr<FeatureDetector> featureDetector = FeatureDetector::create("SIFT");
 std::vector<cv::KeyPoint> keypoints;

 featureDetector->detect(input, keypoints);     // Add results to image and save.

cv::Mat output;

cv::drawKeypoints(input, keypoints, output);

cv::imwrite("sift_result.jpg", output);

return 0;

 }

