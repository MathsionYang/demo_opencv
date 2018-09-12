#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  
using namespace cv;
using namespace std;
int main()
{
	Mat image01 = imread("p2.png",1);//��ͼ
	Mat image02 = imread("p1.png",1);//��ͼ

	//�Ҷ�ת��
	Mat grayimg1, grayimg2;
	cvtColor(image01,grayimg1,CV_RGB2GRAY);
	cvtColor(image02, grayimg2, CV_RGB2GRAY);

	//��ȡ������
	SurfFeatureDetector surfDetector(800);// ����������ֵ��������������ȣ�ֵԽ���Խ�٣�Խ��׼ 
	vector<KeyPoint>keyPoint1, keyPoint2;
	surfDetector.detect(grayimg1,keyPoint1);
	surfDetector.detect(grayimg2, keyPoint2);

	//����������,Ϊ����ƥ��������
	SurfDescriptorExtractor surfDescriptor;
	Mat imgDesc1, imgDesc2;
	surfDescriptor.compute(grayimg1,keyPoint1,imgDesc1);
	surfDescriptor.compute(grayimg2, keyPoint2, imgDesc2);

	//��ȡƥ�������㣬����ȡ�������
	FlannBasedMatcher matcher;
	vector<DMatch>matchePoints;
	vector<DMatch> GoodMatchePoints;

	matcher.match(imgDesc1,imgDesc2,matchePoints,Mat());
	cout << "total match points:" << matchePoints.size() << endl;
	
	//// ��ȡ����ƥ���
	//for (int i = 0; i < matchePoints.size(); i++)
	//{
	//	if (matchePoints[i].distance < 0.6 * matchePoints[i].distance)
	//	{
	//		GoodMatchePoints.push_back(matchePoints[i]);
	//	}
	//}

	Mat img_Match;
	drawMatches(image01,keyPoint1,image02,keyPoint2, matchePoints,img_Match);
	imshow("ƥ��",img_Match);
	imwrite("match.jpg",img_Match);

	waitKey();
	return 0;

}