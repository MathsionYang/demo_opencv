#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{

	Mat SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\logo.jpg");
	if (!SrcImage.data)
	{
		return -1;
	}
	imshow("‘≠Õº",SrcImage);

	Mat GrayImage;//ª“∂»ªØ
	cvtColor(SrcImage,GrayImage,CV_RGB2GRAY);

	Mat AtImage;
	int thresh_size = (100 / 4) * 2 + 1;//◊‘  ”¶∂˛÷µªØ„–÷µ
	adaptiveThreshold(GrayImage,AtImage,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY_INV, thresh_size,thresh_size/3);
	imshow("„–÷µÕº", AtImage);

	vector<vector<Point>> contours;
	findContours(AtImage,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);//’“¬÷¿™
	vector<vector<Point>> contours1;
	for (int i = 0; i < contours.size(); i++)
	{
		contours1.push_back(contours[i]);
	}
	Mat hole(AtImage.size(),CV_8U,Scalar(0));//’⁄’÷Õº≤„
	drawContours(hole,contours1,-1,Scalar(255),CV_FILLED);
	imshow("’⁄’÷Õº", hole);
	Mat crop(SrcImage.rows,SrcImage.cols,CV_8UC3);
	SrcImage.copyTo(crop,hole);//‘≠ÕºœÒøΩ±¥µΩ’⁄’÷Õº
	imshow("¬÷¿™Õº",crop);
	waitKey();
	return 0;
}