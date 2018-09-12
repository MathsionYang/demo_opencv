#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp" 
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>  
#include <iostream>
using namespace std;
using namespace cv;

vector<Point2f> imagePoints1, imagePoints2;
Mat ref_win, src_win;
int pcount = 0;
//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
void on_mouse1(int event, int x, int y, int flag, void *ustc)
{
	if (event==CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ����,����ͼ��ô����
	{
		Point p = Point(x,y);
		circle(ref_win,p,1,Scalar(0,0,255),-1);
		imshow("��׼ͼ",ref_win);
		imagePoints1.push_back(p);//���ѡ�еĵ�
		cout << "��׼ͼ:" << p << endl;
		pcount++;
		cout << "point num:" << pcount << endl;
	}
}
void on_mouse2(int event, int x, int y, int flags, void *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
	Point p = Point(x, y);
	circle(src_win, p, 1, Scalar(0, 0, 255), -1);
	imshow("����׼ͼ", src_win);
	imagePoints2.push_back(p);//���ѡ�еĵ�
	cout << "����׼ͼ:" << p << endl;
	}
}
int main()
{
	Mat ref = imread("1.png");
	Mat src = imread("2.png");
	ref_win = ref.clone();
	src_win = src.clone();

	namedWindow("����׼ͼ");
	namedWindow("��׼ͼ");
	imshow("����׼ͼ", src_win);
	imshow("��׼ͼ", ref_win);
	setMouseCallback("����׼ͼ", on_mouse2);
	setMouseCallback("��׼ͼ", on_mouse1);
	
	waitKey();
	string str;
	printf("����ִ�У�\n");
	cin >> str;
	//��任����
	Mat homo = findHomography(imagePoints2, imagePoints1, CV_RANSAC);
	Mat imageTransform1;
	warpPerspective(src,imageTransform1,homo,Size(ref.cols,ref.rows));
	imshow("transform", imageTransform1);
	/*imshow("��׼ͼ���", ref_win);
	imshow("����׼ͼ���", src_win);*/
	imshow("�任ͼ", imageTransform1);
	waitKey();
	return 0;
	

}