#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp" 



using namespace std;
using namespace cv;

Mat srcimg;//ԭͼ��
Mat grayimg;//�Ҷ�ͼ
Mat gaussimg;//��˹ͼ
Mat dilimg;//����ͼ
Mat cannyimg;//Cannyͼ

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1*y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3 * x4)) / d;
		pt.y = ((x1*y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3 * x4)) / d;
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

bool IsBadLine(int a, int b)
{
	if (a * a + b * b < 100)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool x_sort(const Point2f & m1, const Point2f & m2)
{
	return m1.x < m2.x;
}
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f>top, bot;
	vector<Point2f> backup = corners;
	sort(corners.begin(), corners.end(), x_sort);
	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y && top.size() < 2)    //�����С��2��Ϊ�˱����������㶼��top�����
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	corners.clear();
	if (top.size() == 2 && bot.size() == 2)
	{
		//cout << "log" << endl;
		cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];
		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
	}
	else
	{
		corners = backup;
	}
}
int g_dst_hight;  //����ͼ��ĸ߶�
int g_dst_width; //����ͼ��Ŀ��

void CalcDstSize(const vector<cv::Point2f>& corners)
{
	int h1 = sqrt((corners[0].x - corners[3].x)*(corners[0].x - corners[3].x) + (corners[0].y - corners[3].y)*(corners[0].y - corners[3].y));
	int h2 = sqrt((corners[1].x - corners[2].x)*(corners[1].x - corners[2].x) + (corners[1].y - corners[2].y)*(corners[1].y - corners[2].y));
	g_dst_hight = MAX(h1, h2);

	int w1 = sqrt((corners[0].x - corners[1].x)*(corners[0].x - corners[1].x) + (corners[0].y - corners[1].y)*(corners[0].y - corners[1].y));
	int w2 = sqrt((corners[2].x - corners[3].x)*(corners[2].x - corners[3].x) + (corners[2].y - corners[3].y)*(corners[2].y - corners[3].y));
	g_dst_width = MAX(w1, w2);
}
int main()
{
	srcimg = imread("1.png");//��ȡԭͼ
	
	Mat source = srcimg.clone();
	Mat bkup = srcimg.clone();
	Mat img = srcimg.clone();

	cvtColor(img,grayimg,CV_RGB2GRAY);//�ҶȻ�
	GaussianBlur(grayimg, gaussimg, Size(5, 5), 0, 0);//��˹�˲�
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3)); //��ȡ�Զ����
	//MORPH_RECT��ʾ���εľ����

	dilate(gaussimg,dilimg,element);//����
	Canny(dilimg, cannyimg, 30, 120, 3);//��Ե��ȡ
	imshow("��Եͼ",cannyimg);

	//��������
	vector<vector<Point> > contours;
	vector<vector<Point> > f_contours;
	std::vector<cv::Point> approx2;
	//CV_RETR_EXTERNAL��ֻ�������  
	findContours(cannyimg,f_contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//�������������
	int max_area = 0;
	int index;
	for (int i = 0; i <f_contours.size(); i++)
	{
		double tmparea = fabs(contourArea(f_contours[i]));
		if (tmparea>max_area)
		{
			index = i;
			max_area = tmparea;
		}
	}
	contours.push_back(f_contours[index]);//��ȡ������������
	vector<Point> tmp = contours[0];
	//�ҵ��������ĸ���
	cv::Point2f center(0, 0);
	for (int i = 0; i <=3; i++)
	{
		Mat black = cannyimg.clone();
		black.setTo(0);
		drawContours(black,contours,0,Scalar(255),i);
		imshow("show contours",black);
		std::vector<Vec4i> lines;
		std::vector<cv::Point2f> corners;
		std::vector<cv::Point2f> approx;
		int para = 10;
		int flag = 0;
		int round = 0;
		for (; para < 300; para++)
		{
			cout << "round: " << ++round << endl;
			lines.clear();
			corners.clear();
			approx.clear();
			center = Point2f(0, 0);

			cv::HoughLinesP(black, lines, 1, CV_PI / 180, para, 30, 10);
			//���˾���̫����ֱ��
			std::set<int>ErasePt;
			for (int i = 0; i < lines.size(); i++)
			{
				for (int j = i + 1; j < lines.size(); j++)
				{
					if (IsBadLine(abs(lines[i][0] - lines[j][0]),
						abs(lines[i][1] - lines[j][1])) && (IsBadLine(abs(lines[i][2] - lines[j][2]),
							abs(lines[i][3] - lines[j][3]))))
					{
						ErasePt.insert(j);//���û��߼��뼯��
					}
				}
			}
				int Num = lines.size();
				while (Num != 0)
				{
					std::set<int>::iterator j = ErasePt.find(Num);
					if (j != ErasePt.end())
					{
						lines.erase(lines.begin() + Num - 1);
					}
					Num--;
				}
				//����ֱ�ߵĽ��㣬������ͼ��Χ�ڵĲ���
				for (int i = 0; i < lines.size(); i++)
				{
					for (int j = i + 1; j < lines.size(); j++)
					{
						cv::Point2f pt = computeIntersect(lines[i], lines[j]);
						if (pt.x >= 0 && pt.y >= 0 && pt.x <= srcimg.cols && pt.y <= srcimg.rows)             //��֤������ͼ��ķ�Χ֮��
							corners.push_back(pt);
					}
				}
				if (lines.size() != 4&&corners.size()!=4)
				{
					continue;
				}
				bool IsGoodPoints = true;

				//��֤�����ľ����㹻�����ų������
				for (int i = 0; i < corners.size(); i++)
				{
					for (int j = i + 1; j < corners.size(); j++)
					{
						int distance = sqrt((corners[i].x - corners[j].x)*(corners[i].x - corners[j].x) + (corners[i].y - corners[j].y)*(corners[i].y - corners[j].y));
						if (distance < 5)
						{
							IsGoodPoints = false;
						}
					}
				}

				if (!IsGoodPoints) continue;
				//���ĸ��㹹�ɲ����ı��������ų�
				cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);

				if (lines.size() == 4 && corners.size() == 4 && approx.size() == 4)
				{
					flag = 1;
					break;
				}
			}
			for (int i = 0; i < corners.size(); i++)
			{
				center += corners[i];
			}
			if (flag)
			{
				cout << "we found it!" << endl;
				cv::circle(bkup, corners[0], 3, CV_RGB(255, 0, 0), -1);
				cv::circle(bkup, corners[1], 3, CV_RGB(0, 255, 0), -1);
				cv::circle(bkup, corners[2], 3, CV_RGB(0, 0, 255), -1);
				cv::circle(bkup, corners[3], 3, CV_RGB(255, 255, 255), -1);
				cv::circle(bkup, center, 3, CV_RGB(255, 0, 255), -1);
				imshow("backup", bkup);
				sortCorners(corners, center);
				CalcDstSize(corners);
				//͸��任
				cv::Mat quad = cv::Mat::zeros(g_dst_hight, g_dst_width, CV_8UC3);
				std::vector<cv::Point2f> quad_pts;
				quad_pts.push_back(cv::Point2f(0, 0));
				quad_pts.push_back(cv::Point2f(quad.cols, 0));
				quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
				quad_pts.push_back(cv::Point2f(0, quad.rows));
				cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
				cv::warpPerspective(source, quad, transmtx, quad.size());
				imshow("find", bkup);
				cv::imshow("quadrilateral", quad);
			}
	}
	
	waitKey();
	return 0;
}
