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
    at dictionary; 
   int main(int argc, char*argv[])
{
    FileStorage fs("dictionary.yml", FileStorage::READ);
    fs["vocabulary"] >> dictionary;
    fs.release();    
    
    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);    
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor,matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);
    char * filename = new char[100];
    char * imageTag = new char[10];
    FileStorage fs1("descriptor.yml", FileStorage::WRITE);    
    sprintf(filename,"G:\\testimages\\image\\1.jpg");        
    //read the image
    Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);        
    vector<KeyPoint> keypoints;        
    Detector->detect(img,keypoints);
    Mat bowDescriptor;        
    bowDE.compute(img,keypoints,bowDescriptor);
    printf(imageTag,"img1");            
    fs1 << imageTag << bowDescriptor;        
    fs1.release();
#endif   
return 0;
}
