#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp" 
using namespace std;
using namespace cv;

Mat contours_img;//����ͼ��
int main()
{
	contours_img = imread("E:\\Program Files\\Opencv\\01.png",-1);
	
	Mat threshimg;
	threshold(contours_img,threshimg,30,200.0, CV_THRESH_BINARY);
	namedWindow("��ֵͼ",CV_WINDOW_AUTOSIZE);
	imshow("��ֵͼ", threshimg);
	Mat cannyimg;
	vector<vector<Point> > f_contours;
	Mat drawimg;
	Mat findimg;
     findContours(findimg, f_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	 vector<vector<Point> >::iterator itc = f_contours.begin();
	 while (itc != f_contours.end())
	 {
		 if (itc->size() < 100)
		 {
			 itc = f_contours.erase(itc);
		 }
		 else
		 {
			 ++itc;
		 }
	 }
	 Mat eraseimg;
	 Mat result_erase(cannyimg.size(), CV_8U, Scalar(255));
	 drawContours(result_erase, f_contours,
		 -1, //������������  
		 Scalar(0),  //��ɫΪ��ɫ  
		 2); //�����ߵĻ��ƿ��Ϊ2  
	 imshow("���ͼ", cannyimg);
	waitKey();
	return 0;
}