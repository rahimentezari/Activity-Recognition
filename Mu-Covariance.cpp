
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <string.h>
using namespace std;
using namespace cv;

Mat mu, cov,;
Mat_ <float> total(11000000, 3);
long int total_counter;
const double PI  =3.141592653589793238463;
const float  PI_F=3.14159265358979f;

double likelihood(Mat x)
{
	double alpha = 1/(pow(2 * PI, 3)*determinant(cov));
	cov.inv();
	Mat trans;
	transpose((x-mu), trans);
	Mat e = trans*cov.inv()*(x-mu);

	return alpha * exp(-0.5 * e.ptr<double>(0)[0]);
}

double* segment_avg(Mat segment)
{
	long int sum_b = 0, sum_g = 0, sum_r = 0;
	for(int i = 0; i < 20; i++)
		for(int j = 0; j < 20; j++)
		{
			sum_b += segment.ptr<cv::Vec3b>(j)[i][0];
			sum_g += segment.ptr<cv::Vec3b>(j)[i][1];
			sum_r += segment.ptr<cv::Vec3b>(j)[i][2];
		}
	double ret[3];
	ret[0] = sum_b / 400;
	ret[1] = sum_g / 400;
	ret[2] = sum_r / 400;
//cout<<"main func:\t"<<ret[0]<<"\t"<<ret[1]<<"\t"<<ret[2]<<endl;
	return ret;
}

//rectangular segmentation

long double* avg_calc(int r, int g, int b, char* img_name, char* img_addr, char* img_ext)
{
	char image[200], image_gt[200];

	strcpy(image, img_addr);

	strcpy(image_gt, img_addr);
	strcat(image, img_name);
	strcat(image_gt, img_name);
	strcat(image, img_ext);
	strcat(image_gt, "_GT");
	strcat(image_gt, img_ext);

	Mat main_img = imread(image_gt, 1);
	Mat img_gt = imread(image_gt, 1);

	//avg for r
	long double avg_r = 0, avg_g = 0, avg_b = 0;
	int counter = 0;
	for(int j = 0; j < main_img.rows; j++) {
	    	   for(int i = 0; i < main_img.cols; i++) {

	    		   uchar bb = img_gt.ptr<cv::Vec3b>(j)[i][0];
	    		   uchar gg = img_gt.ptr<cv::Vec3b>(j)[i][1];
	    		   uchar rr = img_gt.ptr<cv::Vec3b>(j)[i][2];

	    		   if(b == bb && g == gg && r == rr)
	    		   {
	    			   counter++;
	    			   avg_b += main_img.ptr<cv::Vec3b>(j)[i][0];
	    			   avg_g += main_img.ptr<cv::Vec3b>(j)[i][1];
	    			   avg_r += main_img.ptr<cv::Vec3b>(j)[i][2];
	    		   }
	    	    }
	    	 }
	avg_b /= counter;
	avg_g /= counter;
	avg_r /= counter;
	long double ret[]={avg_r,avg_g,avg_b};
	return ret;
}

long double* sd_calc(int r, int g, int b, char* img_name, char* img_addr, char* img_ext)
{
		char image[200], image_gt[200];

		strcpy(image, img_addr);
		strcpy(image_gt, img_addr);
		strcat(image, img_name);
		strcat(image_gt, img_name);
		strcat(image, img_ext);
		strcat(image_gt, "_GT");
		strcat(image_gt, img_ext);

		Mat main_img = imread(image, 1);
		Mat img_gt = imread(image_gt, 1);

		long double *avg = avg_calc(r, g, b, img_name, img_addr, img_ext);

		//avg for r
		long double sd_r = 0, sd_g = 0, sd_b = 0;
		int counter = 0, counter_temp = 0;
		for(int j = 0; j < main_img.rows; j++) {
		    	   for(int i = 0; i < main_img.cols; i++) {

		    		   uchar bb = img_gt.ptr<cv::Vec3b>(j)[i][0];
		    		   uchar gg = img_gt.ptr<cv::Vec3b>(j)[i][1];
		    		   uchar rr = img_gt.ptr<cv::Vec3b>(j)[i][2];


		    		   if(b == bb && g == gg && r == rr)
		    		   {
		    			   counter++;
		    			   sd_r += pow(avg[0] - main_img.ptr<cv::Vec3b>(j)[i][0], 2);
		    			   sd_g += pow(avg[1] - main_img.ptr<cv::Vec3b>(j)[i][1], 2);
		    			   sd_b += pow(avg[2] - main_img.ptr<cv::Vec3b>(j)[i][2], 2);
		    		   }
		    	    }
		    	 }

		Mat_<float> smp(counter,3);
		int temp_counter = 0;

		for(int j = 0; j < main_img.rows; j++)
		{
				    	   for(int i = 0; i < main_img.cols; i++)
				    	   {
				    		   uchar bb = img_gt.ptr<cv::Vec3b>(j)[i][0];
				    		   uchar gg = img_gt.ptr<cv::Vec3b>(j)[i][1];
				    		   uchar rr = img_gt.ptr<cv::Vec3b>(j)[i][2];

				    		   if(b == bb && g == gg && r == rr)
				    		   {
				    			   //smp.push_back(main_img.ptr<cv::Vec3b>(j)[i]);

				    			   smp(temp_counter,0) = main_img.ptr<cv::Vec3b>(j)[i][0];
				    			   //smp.push_back(main_img.ptr<cv::Vec3b>(j)[i][1]);
				    			   smp(temp_counter,1) = main_img.ptr<cv::Vec3b>(j)[i][1];
				    			   //smp.push_back(main_img.ptr<cv::Vec3b>(j)[i][2]);
				    			   smp(temp_counter,2) = main_img.ptr<cv::Vec3b>(j)[i][2];
				    			   temp_counter++;
				    		   }
				    	   }
		}

		//cout<<"ok"<<smp(1,1)<<endl;
		for(int i = 0; i < temp_counter; i++)
		{
			total(total_counter + i, 0) = smp(i, 0);
			total(total_counter + i, 1) = smp(i, 1);
			total(total_counter + i, 2) = smp(i, 2);
			//cout<<smp[i][0]<<"\t"<<smp[i][1]<<"\t"<<smp[i][2]<<endl;
		}

		total_counter += temp_counter;

		sd_b /= counter;
		sd_b = sqrt(sd_b);
		sd_g /= counter;
		sd_g = sqrt(sd_g);
		sd_r /= counter;
		sd_r = sqrt(sd_r);

		long double ret[]={sd_r,sd_g,sd_b};
		///////////////////////////////
		if(temp_counter != 0)
		{
		    //cv::calcCovarMatrix(smp, cov, mu, CV_COVAR_NORMAL | CV_COVAR_ROWS);

		    //cout << "cov: " << endl;
		    //cout << cov << endl;

		    //cout << "mu: " << endl;
		    //cout << mu << endl;
		}
		//else cout<<"Not available!\n";
		///////////////////////////////

		return ret;
}


long double* sd_calc(int r, int g, int b, char* img_name, char* img_addr, char* img_ext,double* pre_avg)
{
	char image[200], image_gt[200];

		strcpy(image, img_addr);
		strcpy(image_gt, img_addr);
		strcat(image, img_name);
		strcat(image_gt, img_name);
		strcat(image, img_ext);
		strcat(image_gt, "_GT");
		strcat(image_gt, img_ext);

		Mat main_img = imread(image, 1);
		Mat img_gt = imread(image_gt, 1);

		double *avg = pre_avg;

		//avg for r
		long double sd_r = 0, sd_g = 0, sd_b = 0;
		int counter = 0;
		for(int j = 0; j < main_img.rows; j++) {
		    	   for(int i = 0; i < main_img.cols; i++) {

		    		   uchar bb = img_gt.ptr<cv::Vec3b>(j)[i][0];
		    		   uchar gg = img_gt.ptr<cv::Vec3b>(j)[i][1];
		    		   uchar rr = img_gt.ptr<cv::Vec3b>(j)[i][2];

		    		   if(b == bb && g == gg && r == rr)
		    		   {
		    			   counter++;
		    			   sd_r += pow(avg[0] - main_img.ptr<cv::Vec3b>(j)[i][0], 2);
		    			   sd_g += pow(avg[1] - main_img.ptr<cv::Vec3b>(j)[i][1], 2);
		    			   sd_b += pow(avg[2] - main_img.ptr<cv::Vec3b>(j)[i][2], 2);
		    		   }
		    	    }
		    	 }
		sd_b /= counter;
		sd_b = sqrt(sd_b);
		sd_g /= counter;
		sd_g = sqrt(sd_g);
		sd_r /= counter;
		sd_r = sqrt(sd_r);

		long double ret[]={sd_r,sd_g,sd_b};
		return ret;
}


