#include <imgproc\imgproc.hpp>
#include <highgui\highgui.hpp>
#include <photo\photo.hpp>
#include <core/core.hpp>
#include<opencv2\opencv.hpp> 
using namespace cv;
using namespace std;
Point ptl, ptr;//鼠标画出矩形框的起点和终点
Mat ROI;//原图需要修复的区域
Mat  SrcImage, SrcImageCopy;
void OnMouse(int event, int x, int y, int flag, void *ustg);

int main()
{
	 SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg");
	if (!SrcImage.data)
	{
		return -1;
	}
	imshow("原图",SrcImage);
	cvSetMouseCallback("原图", OnMouse);
	waitKey();
	return 0;
}
	//全区域阈值处理+Mask膨胀处理
	
	////全区域阈值处理+Mask膨胀处理
	//Mat GrayImage;
	////灰度转换
	//cvtColor(SrcImage,GrayImage,CV_RGB2GRAY,0);
	//Mat MaskImage;
	//Mat(SrcImage.size(),CV_8UC1,Scalar::all(0));
	////通过阈值生成Mask
	//threshold(GrayImage,MaskImage,240,255,CV_THRESH_BINARY);
	//Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	////对mask进行膨胀
	//Mat DilaMAsk;
	//dilate(MaskImage, DilaMAsk,Kernel);
	//inpaint(SrcImage, DilaMAsk,SrcImage,5,INPAINT_TELEA);
	//imshow("MASK",MaskImage);
	//imshow("修复后",SrcImage);//全区域阈值处理+Mask膨胀处理
	
	//鼠标圈定区域阈值处理+Mask膨胀处理
void OnMouse(int event, int x, int y, int flag, void *ustg)
{
	if (event==CV_EVENT_LBUTTONDOWN)
	{
		ptl = Point(x,y);
		ptr = Point(x,y);
	}
	if (flag==CV_EVENT_FLAG_LBUTTON)
	{
		ptr = Point(x, y);
		SrcImageCopy = SrcImage.clone();
		rectangle(SrcImageCopy,ptl,ptr,Scalar(255,0,0));
		imshow("原图",SrcImageCopy);
	}
	if (event==CV_EVENT_LBUTTONUP)
	{
		if (ptl != ptr)
		{
			ROI = SrcImage(Rect(ptl,ptr));
			imshow("ROI",ROI);
			waitKey();
		}
	}
	////单击鼠标右键开始图像修复
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		//SrcImageCopy = ROI.clone();
		//Mat GrayImage;
		//cvtColor(ROI,GrayImage,CV_RGB2GRAY);//灰度转换
		//Mat MaskImage = Mat(ROI.size(),CV_8UC1,Scalar::all(0));
		////通过阈值生成Mask
		//threshold(GrayImage, MaskImage, 235, 255, CV_THRESH_BINARY);
		//Mat Kernel = getStructuringElement(MORPH_RECT,Size(3,3));
		//dilate(MaskImage,MaskImage,Kernel);//mask膨胀
		//inpaint(ROI,MaskImage,ROI,9,INPAINT_TELEA);//修复
		//imshow("修复后",SrcImage);
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //PNG格式图片的压缩级别  
		compression_params.push_back(9);  //这里设置保存的图像质量级别

		imwrite("test3.png", ROI, compression_params);
		

	}
}







