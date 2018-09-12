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
	//ʵ����ƥ����
	FlannBasedMatcher matcher;
	Mat c, d;
	vector<KeyPoint>key1, key2;
	vector<DMatch> matches;
	vector<DMatch> Goodmatches;
	//DMatch����������ƥ��õ�һ����������࣬������������֮��������Ϣ
	                       //������ͼ�и�����m��������ͼ��������n��ƥ�䣬���DMatch�ͼ�¼������ƥ�䣬���һ���¼m��n��
	                       //���������ľ����������Ϣ����������ں���������ɸѡ
	Mat imageDesc1, imageDesc2;
	//����ͼ���������룬���������㣬���Mat������������������������
	surf->detectAndCompute(img1,Mat(),key1,imageDesc1);
	surf->detectAndCompute(img2, Mat(), key2, imageDesc2);
	//ƥ�䣬������Դ��������������������DMatch��������
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
