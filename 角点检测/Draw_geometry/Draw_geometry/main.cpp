#include <opencv2/opencv.hpp>  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  

using namespace cv;
using namespace std;



Mat SrcImage,  GrayImage;
int MaxCorners = 23;
int MaxTrackbar = 100;
RNG rng(12345);//�����������

void goodFeaturesToTrack_Demo(int, void*);
int main()
{
	SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg",1);
	//�Ҷȱ任
	cvtColor(SrcImage,GrayImage,CV_BGR2GRAY);

	namedWindow("ͼ��",CV_WINDOW_AUTOSIZE);
	//����trackbar
	createTrackbar("MaxCorners","ͼ��",&MaxCorners,MaxTrackbar,goodFeaturesToTrack_Demo);

	goodFeaturesToTrack_Demo(0,0);
	waitKey();
	return 0;
}
void goodFeaturesToTrack_Demo(int,void*)
{
	//��ʼ�� Shi-Tomasi algorithm��һЩ����
	vector<Point2f>corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	
	//��ԭͼ��һ�α���
	Mat  copy;
	copy = SrcImage.clone();

	//�ǵ���
	goodFeaturesToTrack(GrayImage, corners, MaxCorners, qualityLevel, minDistance, Mat(), 
		blockSize, useHarrisDetector, k);
	//������⵽�Ľǵ�
	cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		circle(copy,corners[i],r,Scalar(rng.uniform(0,255)),-1,8,0);
	}
	namedWindow("�ǵ���",CV_WINDOW_AUTOSIZE);
	imshow("�ǵ���",copy);
}












/*Harris �ǵ���*/
//Mat SrcImage,  GrayImage,Srcimage1;
//int thresh = 30;//��ǰ��ֵ
//int max_Thresh = 175;// �����ֵ
//void on_CornerHarris(int, void*);//�ص�����
//int main(int argc,char** argv)
//{
//	SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg",1);
//	if (!SrcImage.data)
//	{
//		return -1;
//	}
//	imshow("ԭͼ",SrcImage);
//	Srcimage1 = SrcImage.clone();
//	cvtColor(Srcimage1, GrayImage, CV_BGR2GRAY);//�ҶȻ�
//	namedWindow("�ǵ���",CV_WINDOW_AUTOSIZE);
//	createTrackbar("��ֵ:","�ǵ���",&thresh,max_Thresh,on_CornerHarris);
//	on_CornerHarris(0, 0);
//	waitKey();
//	return(0);
//}
//void on_CornerHarris(int, void*)
//{
//	Mat dstImage;//Ŀ��ͼ
//	Mat normImage;//��һ��ͼ;
//	Mat scaledImage;//���Ա任���λ�޷�������ͼ
//	//���㵱ǰ��Ҫ��ʾ������ͼ���������һ�ε��ô˺���ʱ���ǵ�ֵ  
//	dstImage = Mat::zeros(SrcImage.size(), CV_32FC1);
//	Srcimage1 = SrcImage.clone();
//	//���нǵ���  
//  //������������ʾ�����С�����ĸ�������ʾSobel���ӿ׾���С
//	//�����������ʾHarris����
//	cornerHarris(GrayImage,dstImage,2,3,0.04,BORDER_DEFAULT);
//	//��һ��
//	normalize(dstImage,normImage,0,255,NORM_MINMAX,CV_32FC1,Mat());
//	convertScaleAbs(normImage, scaledImage);//����һ�����ͼ�α任��8λ�޷�������
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
//	imshow("�ǵ���",Srcimage1);
//	imshow("�ǵ���2",scaledImage);
//}
