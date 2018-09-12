#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;
Point ptl, ptr;//鼠标画出矩形框的起点和终点
Mat ROI;//原图需要修复的区域
Mat logo;
Mat  SrcImage, SrcImageCopy;
void OnMouse(int event, int x, int y, int flag, void *ustg);
int main()
{
	 SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dog.jpg");
	if (!SrcImage.data)
	{
		return -1;
	}
	namedWindow("原图", WINDOW_NORMAL);
	imshow("原图",SrcImage);
	cvSetMouseCallback("原图", OnMouse);
	 logo = imread("C:\\Users\\yang\\Desktop\\image\\logo2.png");
	if (!logo.data)
	{
		return -1;
	}
	Mat ROIimage = SrcImage(Rect(28, 41, logo.cols, logo.rows));
	////从原图中抠出矩形区域，Rect第一二参数表示矩形左上角定点的坐标，
	//用于定位，后两个参数表示举行的宽和高
	addWeighted(ROIimage,0.2,logo,0.8,0.,ROIimage);
	//第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
	namedWindow("结果图",WINDOW_NORMAL);
	imshow("结果图",SrcImage);
	waitKey();
	return 0;
}
//void OnMouse(int event, int x, int y, int flag, void *ustg)
//{
//	if (event == CV_EVENT_LBUTTONDOWN)
//	{
//		ptl = Point(x, y);
//		ptr = Point(x, y);
//	}
//	if (flag == CV_EVENT_FLAG_LBUTTON)
//	{
//		ptr = Point(x, y);
//		SrcImageCopy = SrcImage.clone();
//		rectangle(SrcImageCopy, ptl, ptr, Scalar(255, 0, 0));
//	}
//	if (event == CV_EVENT_LBUTTONUP)
//	{
//		if (ptl != ptr)
//		{
//			ROI = SrcImage(Rect(ptl, ptr));
//		}
//	}
//	if (event ==CV_EVENT_RBUTTONDOWN)
//	{
//	addWeighted(ROI,0.2,logo,0.8,0.,ROI);
//	//第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
//	namedWindow("结果图",WINDOW_NORMAL);
//	imshow("结果图",SrcImage);
//	}
//}