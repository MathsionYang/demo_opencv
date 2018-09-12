#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

//��һ������������ͼƬ���ƣ��ڶ������������ͼƬ����
void GetContoursPic(const char* pSrcFileName, const char* pDstFileName)
{
	Mat srcImg = imread(pSrcFileName);
	imshow("ԭʼͼ", srcImg);
	Mat gray, binImg;
	//�ҶȻ�
	cvtColor(srcImg, gray, COLOR_RGB2GRAY);
//	imshow("�Ҷ�ͼ", gray);
	//��ֵ��
	threshold(gray, binImg, 100, 200, CV_THRESH_BINARY);
//	imshow("��ֵ��", binImg);

	vector<vector<Point> > contours;
	vector<Rect> boundRect(contours.size());
	//ע���5������ΪCV_RETR_EXTERNAL��ֻ�������  
	findContours(binImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //������
	cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		//��Ҫ��ȡ������  
		CvPoint2D32f rectpoint[4];
		CvBox2D rect = minAreaRect(Mat(contours[i]));

		cvBoxPoints(rect, rectpoint); //��ȡ4����������  
		//��ˮƽ�ߵĽǶ�  
		float angle = rect.angle;
		cout << angle << endl;

		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));
		//rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
		//���̫С��ֱ��pass
		if (line1 * line2 < 600)
		{
			continue;
		}

		//Ϊ���������κ��ŷţ�������ת�Ƕ��ǲ�һ���ġ����ŵģ�������90�ȣ�������  
		if (line1 > line2)
		{
			angle = 90 + angle;
		}

		//�½�һ������Ȥ������ͼ����С��ԭͼһ����  
		Mat RoiSrcImg(srcImg.rows, srcImg.cols, CV_8UC3); //ע���������ѡCV_8UC3
		RoiSrcImg.setTo(0); //��ɫ������Ϊ��ɫ  
		//imshow("�½���ROI", RoiSrcImg);
		//�Եõ����������һ��  
		drawContours(binImg, contours, -1, Scalar(255), CV_FILLED);

		//��ͼ��RoiSrcImg
		srcImg.copyTo(RoiSrcImg, binImg);


		//����ʾһ�¿��������˸���Ȥ�������������ֶ��Ǻ�ɫ����  
		//namedWindow("RoiSrcImg", 1);
		//imshow("RoiSrcImg", RoiSrcImg);

		//����һ����ת���ͼ��  
		Mat RatationedImg(RoiSrcImg.rows, RoiSrcImg.cols, CV_8UC1);
		RatationedImg.setTo(0);
		//��RoiSrcImg������ת  
		Point2f center = rect.center;  //���ĵ�  
		Mat M2 = getRotationMatrix2D(center, angle, 1);//������ת�����ŵı任���� 
		warpAffine(RoiSrcImg, RatationedImg, M2, RoiSrcImg.size(), 1, 0, Scalar(0));//����任 
		imshow("��ת֮��", RatationedImg);
		imwrite("r1.jpg", RatationedImg); //���������ͼƬ��������
	}

#if 1
	//��ROI������п�ͼ

	//����ת���ͼƬ����������ȡ  
	vector<vector<Point> > contours2;
	Mat raw = imread("r1.jpg");
	Mat SecondFindImg;
	//SecondFindImg.setTo(0);
	cvtColor(raw, SecondFindImg, COLOR_BGR2GRAY);  //�ҶȻ�  
	threshold(SecondFindImg, SecondFindImg, 80, 200, CV_THRESH_BINARY);
	findContours(SecondFindImg, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//cout << "sec contour:" << contours2.size() << endl;

	for (int j = 0; j < contours2.size(); j++)
	{
		//��ʱ����ʵ����һ���������ˣ����Ի�ȡrect  
		Rect rect = boundingRect(Mat(contours2[j]));
		//���̫С������ֱ��pass,ͨ�����ù��������С�����Ա�ֻ֤�õ����
		if (rect.area() < 600)
		{
			continue;
		}
		Mat dstImg = raw(rect);
		imshow("dst", dstImg);
		imwrite(pDstFileName, dstImg);
	}
#endif


}


void main()
{
	GetContoursPic("test.png", "FinalImage.jpg");
	waitKey();
}