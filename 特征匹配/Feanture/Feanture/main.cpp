#include <iostream>  
#include <stdio.h>  
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;
int main()
{
	Mat img1 = imread("1.png");
	Mat img2 = imread("2.png");


	Ptr<Feature2D> surf = xfeatures2d::SURF::create(1000);
	//BFMatcher matcher;
	//实例化匹配器
	FlannBasedMatcher matcher;
	Mat c, d;
	vector<KeyPoint>key1, key2;
	vector<DMatch> matches;
	vector<DMatch> Goodmatches;
	//DMatch是用来描述匹配好的一对特征点的类，包含这两个点之间的相关信息
	                       //比如左图有个特征m，它和右图的特征点n最匹配，这个DMatch就记录它俩最匹配，并且还记录m和n的
	                       //特征向量的距离和其他信息，这个距离在后面用来做筛选
	Mat imageDesc1, imageDesc2;
	//输入图像，输入掩码，输入特征点，输出Mat，存放所有特征点的描述向量
	surf->detectAndCompute(img1,Mat(),key1,imageDesc1);
	surf->detectAndCompute(img2, Mat(), key2, imageDesc2);
	//匹配，数据来源是特征向量，结果存放在DMatch类型里面
	matcher.match(imageDesc1,imageDesc2,matches);
	cout << "total match points:" << matches.size() << endl;

	double max_dist = 0;
	double min_dist = 100;
	for (int i = 0; i < imageDesc1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}
	for (int i = 0; i < imageDesc1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			Goodmatches.push_back(matches[i]);
		}
	}
	Mat img_match;
	drawMatches(img1,key1,img2,key2,Goodmatches,img_match,Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("Good Matches", 0);
	resizeWindow("Good Matches", 800, 800);
	imshow("Good Matches", img_match);
}
