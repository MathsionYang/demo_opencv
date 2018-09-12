#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  
using namespace cv;
using namespace std;
int main()
{
	Mat image01 = imread("p2.png",1);//右图
	Mat image02 = imread("p1.png",1);//左图

	//灰度转化
	Mat grayimg1, grayimg2;
	cvtColor(image01,grayimg1,CV_RGB2GRAY);
	cvtColor(image02, grayimg2, CV_RGB2GRAY);

	//提取特征点
	SurfFeatureDetector surfDetector(800);// 海塞矩阵阈值，在这里调整精度，值越大点越少，越精准 
	vector<KeyPoint>keyPoint1, keyPoint2;
	surfDetector.detect(grayimg1,keyPoint1);
	surfDetector.detect(grayimg2, keyPoint2);

	//特征点描述,为特征匹配做主备
	SurfDescriptorExtractor surfDescriptor;
	Mat imgDesc1, imgDesc2;
	surfDescriptor.compute(grayimg1,keyPoint1,imgDesc1);
	surfDescriptor.compute(grayimg2, keyPoint2, imgDesc2);

	//获取匹配特征点，并提取最优配对
	FlannBasedMatcher matcher;
	vector<DMatch>matchePoints;
	vector<DMatch> GoodMatchePoints;

	matcher.match(imgDesc1,imgDesc2,matchePoints,Mat());
	cout << "total match points:" << matchePoints.size() << endl;
	
	//// 获取优秀匹配点
	//for (int i = 0; i < matchePoints.size(); i++)
	//{
	//	if (matchePoints[i].distance < 0.6 * matchePoints[i].distance)
	//	{
	//		GoodMatchePoints.push_back(matchePoints[i]);
	//	}
	//}

	Mat img_Match;
	drawMatches(image01,keyPoint1,image02,keyPoint2, matchePoints,img_Match);
	imshow("匹配",img_Match);
	imwrite("match.jpg",img_Match);

	waitKey();
	return 0;

}