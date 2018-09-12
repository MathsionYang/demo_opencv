#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat srcImage = imread("E:\\Program Files\\Opencv\\HoughLines\\Houghlines\\x64\\Debug\\img.png");//��ȡimg
	imshow("ԭͼ",srcImage);//���崰�����֣���ʾͼƬ

	Mat midImage, dstImage;
	Canny(srcImage,midImage,50,200,3);//��Ե���
	cvtColor(midImage, dstImage, CV_GRAY2BGR);//�ҶȻ�

	vector<Vec2f>lines;//����ʸ���ṹ��ż���������
	
	//ͨ��������������ǾͿ��Եõ���������ֱ�߼�����
	HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);
	//�������������ʾ��ֵ����ֵԽ�󣬱�������Խ��׼���ٶ�Խ�죬
	//�õ���ֱ��Խ�٣��õ���ֱ�߶��Ǻ��а��յ�ֱ�ߣ�

	//���λ���ÿ���߶�
	for (size_t i = 0; i < lines.size(); i++)
	{
		float radio = lines[i][0];//Բ�İ뾶r
		float theta = lines[i][1];//ֱ�߽Ƕ�
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);

		double x0 = a * radio;
		double y0 = b * radio;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dstImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA); 
		//Scalar�������ڵ����߶���ɫ���߶���ʾ����ʲô��ɫ
		imshow("��Ե�����ͼ", midImage);
		imshow("����Ч��ͼ", dstImage);
	}
	waitKey();
	return 0;



}