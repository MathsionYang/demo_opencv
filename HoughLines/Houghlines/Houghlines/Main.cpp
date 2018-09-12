#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat srcImage = imread("E:\\Program Files\\Opencv\\HoughLines\\Houghlines\\x64\\Debug\\img.png");//读取img
	imshow("原图",srcImage);//定义窗口名字，显示图片

	Mat midImage, dstImage;
	Canny(srcImage,midImage,50,200,3);//边缘检测
	cvtColor(midImage, dstImage, CV_GRAY2BGR);//灰度化

	vector<Vec2f>lines;//定义矢量结构存放检测出来的线
	
	//通过这个函数，我们就可以得到检测出来的直线集合了
	HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);
	//第五个参数，表示阈值，阈值越大，表明检测的越精准，速度越快，
	//得到的直线越少（得到的直线都是很有把握的直线）

	//依次画出每条线段
	for (size_t i = 0; i < lines.size(); i++)
	{
		float radio = lines[i][0];//圆的半径r
		float theta = lines[i][1];//直线角度
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);

		double x0 = a * radio;
		double y0 = b * radio;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dstImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA); 
		//Scalar函数用于调节线段颜色，线段显示的是什么颜色
		imshow("边缘检测后的图", midImage);
		imshow("最终效果图", dstImage);
	}
	waitKey();
	return 0;



}