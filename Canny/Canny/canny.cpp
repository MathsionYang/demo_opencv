#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
	Mat SrcImg, grayImg;//ԭͼ�񣬻Ҷ�ͼ��
	Mat binImg;//��ֵ��ͼ
	Mat dstImage;//��˹�˲�ͼ��
	Mat srcimg;
	Mat dst_sobel;//sobel���ͼ
	Mat dst_Laplacian;//laplacian���ͼ
	SrcImg = imread("E:\\Program Files\\Opencv\\TestOpenCv\\img\\123.png");
	if (!SrcImg.data)
	{
		printf("��ȡͼƬ����������������ȷ·����\n");
		system("pause");
		return -1;
	}
	//imshow("ԭʼͼ��", SrcImg);
	cvtColor(SrcImg, grayImg, CV_BGR2GRAY);//תΪ�Ҷ�ͼ
	//imshow("�Ҷ�ͼ��", grayImg);


	GaussianBlur(grayImg, dstImage, Size(3, 3), 0, 0);
	//imshow("��˹�˲�", dstImage);
	
	namedWindow("��ֵ��ͼ", WINDOW_AUTOSIZE);
	threshold(grayImg, binImg, 0, 255, CV_THRESH_OTSU);//��ֵ��
	//imshow("��ֵ��ͼ", binImg);
	srcimg = SrcImg.clone();
	Mat dst;
	//canny����
	Mat edge;
	dst.create(srcimg.size(), srcimg.type());
	blur(grayImg,edge,Size(3,3));//3*3�ں˽���
	Canny(edge, edge, 3, 9, 3); 
	//��dst������Ԫ������Ϊ0
	dst = Scalar::all(0);
	//edge��Ϊ���룬��ԭͼsrc������dst��
	srcimg.copyTo(dst, edge);
	imshow("Canny����ɫͼ", dst);
	//sobel����
	Mat xsobel;
	Sobel(grayImg,xsobel,-1,1,0,3);
	Mat ysobel;
	Sobel(grayImg,ysobel,-1,0,1,3);
	addWeighted(xsobel, 0.5, ysobel, 0.5, 0, dst_sobel);
	imshow("Sobel����ɫͼ", dst_sobel);
	//Laplacian����
	Laplacian(grayImg, dst_Laplacian, -1, 3);
	convertScaleAbs(dst_Laplacian, dst_Laplacian);
	imshow("Laplacian����ɫͼ", dst_Laplacian);

	//��̬ѧ����
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat erode_ouput, dilate_output;
	erode(SrcImg, erode_ouput, element); //��ʴ  
	dilate(SrcImg, dilate_output, element); //����
	Mat basicGradient;
	subtract(dilate_output, erode_ouput, basicGradient, Mat());
	imshow("�����ݶ�", basicGradient);
	waitKey(0);
	return 0;
}





