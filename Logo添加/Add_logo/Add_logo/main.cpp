#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;
Point ptl, ptr;//��껭�����ο�������յ�
Mat ROI;//ԭͼ��Ҫ�޸�������
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
	namedWindow("ԭͼ", WINDOW_NORMAL);
	imshow("ԭͼ",SrcImage);
	cvSetMouseCallback("ԭͼ", OnMouse);
	 logo = imread("C:\\Users\\yang\\Desktop\\image\\logo2.png");
	if (!logo.data)
	{
		return -1;
	}
	Mat ROIimage = SrcImage(Rect(28, 41, logo.cols, logo.rows));
	////��ԭͼ�пٳ���������Rect��һ��������ʾ�������ϽǶ�������꣬
	//���ڶ�λ��������������ʾ���еĿ�͸�
	addWeighted(ROIimage,0.2,logo,0.8,0.,ROIimage);
	//��һ���ĸ��������Ǹ���Ȩ�أ���5���������ǹ�ʽ�е�ƫִ����gamma��
	namedWindow("���ͼ",WINDOW_NORMAL);
	imshow("���ͼ",SrcImage);
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
//	//��һ���ĸ��������Ǹ���Ȩ�أ���5���������ǹ�ʽ�е�ƫִ����gamma��
//	namedWindow("���ͼ",WINDOW_NORMAL);
//	imshow("���ͼ",SrcImage);
//	}
//}