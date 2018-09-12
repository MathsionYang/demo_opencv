#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  

using namespace cv;
using namespace std;
int main()
{
	//读取原图
	Mat srcimg1, srcimg2;
	srcimg1 = imread("p2.png");
	srcimg2 = imread("p1.png");

	//灰度转化
	Mat grayimg1, grayimg2;
	cvtColor(srcimg1,grayimg1,CV_RGB2GRAY);
	cvtColor(srcimg2, grayimg2, CV_RGB2GRAY);

	//提取特征点
	SiftFeatureDetector siftDetector(2000); // 海塞矩阵阈值，在这里调整精度，值越大点越少，越精准
	vector<KeyPoint>keyPoint1, keyPoint2;
	siftDetector.detect(grayimg1,keyPoint1);
	siftDetector.detect(grayimg2, keyPoint2);

	//特征点描述
	SiftDescriptorExtractor siftDescriptor;
	Mat imgDesc1,imgDesc2;
	siftDescriptor.compute(grayimg1,keyPoint1,imgDesc1);
	siftDescriptor.compute(grayimg2, keyPoint2, imgDesc2);

	//获取特征点
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(imgDesc1,imgDesc2,matchePoints,Mat());
	cout << "total match points:" << matchePoints.size() << endl;

	Mat img_match;
	drawMatches(srcimg1,keyPoint1,srcimg2,keyPoint2,matchePoints,img_match);
	imshow("匹配",img_match);
	imwrite("match.jpg",img_match);
	waitKey();
	return 0;
}