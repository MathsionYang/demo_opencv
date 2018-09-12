#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
	Mat SrcImg, grayImg;//原图像，灰度图像
	Mat binImg;//二值化图
	Mat dstImage;//高斯滤波图像
	Mat srcimg;
	Mat dst_sobel;//sobel检测图
	Mat dst_Laplacian;//laplacian检测图
	SrcImg = imread("E:\\Program Files\\Opencv\\TestOpenCv\\img\\123.png");
	if (!SrcImg.data)
	{
		printf("读取图片错误，请重新输入正确路径！\n");
		system("pause");
		return -1;
	}
	//imshow("原始图像", SrcImg);
	cvtColor(SrcImg, grayImg, CV_BGR2GRAY);//转为灰度图
	//imshow("灰度图像", grayImg);


	GaussianBlur(grayImg, dstImage, Size(3, 3), 0, 0);
	//imshow("高斯滤波", dstImage);
	
	namedWindow("二值化图", WINDOW_AUTOSIZE);
	threshold(grayImg, binImg, 0, 255, CV_THRESH_OTSU);//二值化
	//imshow("二值化图", binImg);
	srcimg = SrcImg.clone();
	Mat dst;
	//canny算子
	Mat edge;
	dst.create(srcimg.size(), srcimg.type());
	blur(grayImg,edge,Size(3,3));//3*3内核降噪
	Canny(edge, edge, 3, 9, 3); 
	//将dst内所有元素设置为0
	dst = Scalar::all(0);
	//edge作为掩码，将原图src拷贝到dst中
	srcimg.copyTo(dst, edge);
	imshow("Canny检测彩色图", dst);
	//sobel算子
	Mat xsobel;
	Sobel(grayImg,xsobel,-1,1,0,3);
	Mat ysobel;
	Sobel(grayImg,ysobel,-1,0,1,3);
	addWeighted(xsobel, 0.5, ysobel, 0.5, 0, dst_sobel);
	imshow("Sobel检测彩色图", dst_sobel);
	//Laplacian算子
	Laplacian(grayImg, dst_Laplacian, -1, 3);
	convertScaleAbs(dst_Laplacian, dst_Laplacian);
	imshow("Laplacian检测彩色图", dst_Laplacian);

	//形态学计算
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat erode_ouput, dilate_output;
	erode(SrcImg, erode_ouput, element); //腐蚀  
	dilate(SrcImg, dilate_output, element); //膨胀
	Mat basicGradient;
	subtract(dilate_output, erode_ouput, basicGradient, Mat());
	imshow("基本梯度", basicGradient);
	waitKey(0);
	return 0;
}





