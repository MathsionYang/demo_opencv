#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  

using namespace cv;
using namespace std;
int main()
{
	//��ȡԭͼ
	Mat srcimg1, srcimg2;
	srcimg1 = imread("p2.png");
	srcimg2 = imread("p1.png");

	//�Ҷ�ת��
	Mat grayimg1, grayimg2;
	cvtColor(srcimg1,grayimg1,CV_RGB2GRAY);
	cvtColor(srcimg2, grayimg2, CV_RGB2GRAY);

	//��ȡ������
	SiftFeatureDetector siftDetector(2000); // ����������ֵ��������������ȣ�ֵԽ���Խ�٣�Խ��׼
	vector<KeyPoint>keyPoint1, keyPoint2;
	siftDetector.detect(grayimg1,keyPoint1);
	siftDetector.detect(grayimg2, keyPoint2);

	//����������
	SiftDescriptorExtractor siftDescriptor;
	Mat imgDesc1,imgDesc2;
	siftDescriptor.compute(grayimg1,keyPoint1,imgDesc1);
	siftDescriptor.compute(grayimg2, keyPoint2, imgDesc2);

	//��ȡ������
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(imgDesc1,imgDesc2,matchePoints,Mat());
	cout << "total match points:" << matchePoints.size() << endl;

	Mat img_match;
	drawMatches(srcimg1,keyPoint1,srcimg2,keyPoint2,matchePoints,img_match);
	imshow("ƥ��",img_match);
	imwrite("match.jpg",img_match);
	waitKey();
	return 0;
}