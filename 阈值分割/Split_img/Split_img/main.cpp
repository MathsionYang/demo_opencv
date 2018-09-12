#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp" 
using namespace std;
using namespace cv;
int main()
{
	
	Mat srcimg;//原图像
	srcimg = imread("1.png");
	//判断原图像是否成功读取
	if (!srcimg.data)
	{
		return -1;
	}

	Mat grayimg;//灰度图
	cvtColor(srcimg,grayimg,CV_RGB2GRAY);// 灰度化

	//全局二值化
	int thresh = 180;
	Mat binary;
	threshold(grayimg,binary,thresh,255,CV_THRESH_BINARY);

	vector<Mat>channels;
	split(srcimg,channels);
	Mat red = channels[2];
	Mat blue = channels[0];
	Mat green = channels[1];

	Mat red_binary;
	threshold(red,red_binary,thresh,255,CV_THRESH_BINARY);


	imshow("red channel", red);
	imshow("blue channel", blue);
	imshow("green channel", green);
	imshow("red+binary", red_binary);








	
	waitKey();
	return 0;
}