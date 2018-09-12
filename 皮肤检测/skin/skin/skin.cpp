#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cvaux.h>
#include <highgui.h>
using namespace cv;
#if 1
class skin_detector
{
public:
	Mat RGB_detector(Mat &img)
	{
		Mat detect = img.clone();
		detect.setTo(0);
		if (img.empty()||img.channels()!=3)
		{
			return detect;
		}
		for (int  i = 0; i < img.rows; i++)
		{
			for (int j = 0; i < img.cols; j++)
			{
				uchar* p_detect = detect.ptr<uchar>(i, j);
				uchar *p_img = img.ptr<uchar>(i, j);
				if ((p_img[2] > 95 && p_img[1] > 40 && p_img[0] > 20 &&
					(MAX(p_img[0], MAX(p_img[1], p_img[2])) - MIN(p_img[0], MIN(p_img[1], p_img[2])) > 15) &&
					abs(p_img[2] - p_img[1]) > 15 && p_img[2] > p_img[1] && p_img[1] > p_img[0]) ||
					(p_img[2] > 200 && p_img[1] > 210 && p_img[0] > 170 && abs(p_img[2] - p_img[1]) <= 15 &&
						p_img[2] > p_img[0] && p_img[1] > p_img[0]))
				{
					p_detect[0] = p_img[0];
					p_detect[1] = p_img[1];
					p_detect[2] = p_img[2];
				}

			}

		}
	}






};

#endif
int main(int argc, char **argv)
{
	return 0;
}