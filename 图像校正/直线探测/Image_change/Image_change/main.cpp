#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

#define ERROR 1234
double DegreeTrans(double theta)
{
	double res = theta / CV_PI * 180;
	return res;
}
//逆时针旋转图像degree角度（原尺度）
void rotateImage(Mat src, Mat& img_rotate, double degree)
{
	//旋转中心为图像中心
	Point2f center;
	center.x = float(src.cols/2.0);
	center.y = float(src.rows/2.0);
	int length = 0;
	length = sqrt(src.cols*src.cols+src.rows*src.rows);
	//计算二维旋转的仿射变换矩阵
	Mat M = getRotationMatrix2D(center,degree,1);
	//仿射变换，背景色为白色
	warpAffine(src, img_rotate, M, Size(length, length), 1, 0, Scalar(255, 255, 255));
}//霍夫变换检测直线
double CalcDegree(const Mat  &srcImage, Mat &dst)
{
	Mat midImage, dstImage;
	Canny(srcImage,midImage,50,200,3);
	cvtColor(midImage,dstImage,CV_GRAY2BGR);

	//通过霍夫变换检测直线
	vector<Vec2f>lines;
	HoughLines(midImage,lines,1,CV_PI/180,300,0,0);
	//第五个参数是阈值，越大，检测精度越高
	 /*/由于图像不同，阈值不好设定，
	因为阈值设定过高导致无法检测直线，阈值过低直线太多，速度很慢
	//所以根据阈值由大到小设置了三个阈值，
		如果经过大量试验后，可以固定一个适合的阈值。*/
	if (!lines.size())
	{
		HoughLines(midImage, lines, 1, CV_PI / 180, 200, 0, 0);
	}
	if (!lines.size())
	{
		HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);
	}
	if (!lines.size())
	{
		cout << "没有检测到直线！" << endl;
		return ERROR;
	}
	float sum = 0;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho;
		double y0 = b * rho;
		//取直线上距离（x0,y0）1000de 两个点，作为直线上两点
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		//只选角度最小的为旋转角度
		sum += theta;
		line(dstImage,pt1,pt2,Scalar(55,100,195),1,LINE_AA);
		imshow("直线探测效果图",dstImage);
	}
	float average = sum / lines.size(); 
	//对所有角度求平均，这样做旋转效果会更好
	cout << "average theta:" << average << endl;
	double angle = DegreeTrans(average) - 90;
	rotateImage(dstImage, dst, angle);
	return angle;
}
void ImageRecify(const char* pInFileName,const char* pOutFilename)
{
	double degree;
	Mat src = imread(pInFileName);
	Mat dst ;
	//倾斜角度校正
	degree = CalcDegree(src,dst);
	if (degree==ERROR)
	{
		cout << "矫正失败！" << endl;
		return;
	}
	rotateImage(src,dst,degree);
	cout << "angle:" << degree << endl;
	imshow("旋转调整后", dst);
	Mat resulyImage = dst(Rect(0,0,dst.cols,500));
	//根据先验知识，估计好文本长宽，在裁剪
	//imshow("裁剪后",resulyImage);
}
int main()
{
	ImageRecify("text.png", "FinalImage.jpg");
	waitKey();
	return 0;
}