#include <opencv2/opencv.hpp>  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  

using namespace cv;
using namespace std;



Mat SrcImage,  GrayImage;
int MaxCorners = 23;
int MaxTrackbar = 100;
RNG rng(12345);//随机数产生器

void goodFeaturesToTrack_Demo(int, void*);
int main()
{
	SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg",1);
	//灰度变换
	cvtColor(SrcImage,GrayImage,CV_BGR2GRAY);

	namedWindow("图像",CV_WINDOW_AUTOSIZE);
	//创建trackbar
	createTrackbar("MaxCorners","图像",&MaxCorners,MaxTrackbar,goodFeaturesToTrack_Demo);

	goodFeaturesToTrack_Demo(0,0);
	waitKey();
	return 0;
}
void goodFeaturesToTrack_Demo(int,void*)
{
	//初始化 Shi-Tomasi algorithm的一些参数
	vector<Point2f>corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	
	//给原图做一次备份
	Mat  copy;
	copy = SrcImage.clone();

	//角点检测
	goodFeaturesToTrack(GrayImage, corners, MaxCorners, qualityLevel, minDistance, Mat(), 
		blockSize, useHarrisDetector, k);
	//画出检测到的角点
	cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		circle(copy,corners[i],r,Scalar(rng.uniform(0,255)),-1,8,0);
	}
	namedWindow("角点检测",CV_WINDOW_AUTOSIZE);
	imshow("角点检测",copy);
}












/*Harris 角点检测*/
//Mat SrcImage,  GrayImage,Srcimage1;
//int thresh = 30;//当前阈值
//int max_Thresh = 175;// 最大阈值
//void on_CornerHarris(int, void*);//回调函数
//int main(int argc,char** argv)
//{
//	SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg",1);
//	if (!SrcImage.data)
//	{
//		return -1;
//	}
//	imshow("原图",SrcImage);
//	Srcimage1 = SrcImage.clone();
//	cvtColor(Srcimage1, GrayImage, CV_BGR2GRAY);//灰度化
//	namedWindow("角点检测",CV_WINDOW_AUTOSIZE);
//	createTrackbar("阈值:","角点检测",&thresh,max_Thresh,on_CornerHarris);
//	on_CornerHarris(0, 0);
//	waitKey();
//	return(0);
//}
//void on_CornerHarris(int, void*)
//{
//	Mat dstImage;//目标图
//	Mat normImage;//归一化图;
//	Mat scaledImage;//线性变换后八位无符号整型图
//	//置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值  
//	dstImage = Mat::zeros(SrcImage.size(), CV_32FC1);
//	Srcimage1 = SrcImage.clone();
//	//进行角点检测  
//  //第三个参数表示邻域大小，第四个参数表示Sobel算子孔径大小
//	//第五个参数表示Harris参数
//	cornerHarris(GrayImage,dstImage,2,3,0.04,BORDER_DEFAULT);
//	//归一化
//	normalize(dstImage,normImage,0,255,NORM_MINMAX,CV_32FC1,Mat());
//	convertScaleAbs(normImage, scaledImage);//将归一化后的图形变换成8位无符号整型
//	for (int j = 0; j< normImage.rows; j++)
//	{
//		for (int i = 0; i < normImage.cols; i++)
//		{
//			if ((int)normImage.at<float>(j, i) > thresh + 80)
//			{
//				circle(Srcimage1,Point(i,j),5,Scalar(10,10,255),2,8,0);
//				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
//			}
//		}
//
//	}
//	imshow("角点检测",Srcimage1);
//	imshow("角点检测2",scaledImage);
//}
