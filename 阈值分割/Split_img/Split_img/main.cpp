#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp" 
using namespace std;
using namespace cv;
int main()
{
	
	Mat srcimg;//ԭͼ��
	srcimg = imread("1.png");
	//�ж�ԭͼ���Ƿ�ɹ���ȡ
	if (!srcimg.data)
	{
		return -1;
	}

	Mat grayimg;//�Ҷ�ͼ
	cvtColor(srcimg,grayimg,CV_RGB2GRAY);// �ҶȻ�

	//ȫ�ֶ�ֵ��
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