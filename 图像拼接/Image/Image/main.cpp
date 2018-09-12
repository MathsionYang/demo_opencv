#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>
#include <iostream>  

using namespace cv;
using namespace std;


void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst);
typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}four_corners_t;

four_corners_t corners;

void CalcCorners(const Mat& H, const Mat& src)
{
	double v2[] = {0,0,1};//左上角
	double v1[3];//变换后的坐标值
	Mat V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	Mat V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	//左上角(0,0,1)
	cout << "V2: " << V2 << endl;
	cout << "V1: " << V1 << endl;
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];
	
	//左下角(0,src,rows,1)
	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	//右上角(src.cols,0,1)
	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	//右下角（src.cols,src.rows,1）
	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];
}

int main(int argc ,char *argv[])
{

	Mat srcimg1, srcimg2;
	srcimg1 = imread("09.jpg",1);
	srcimg2 = imread("10.jpg",1);
	imshow("p1",srcimg1);
	imshow("p2",srcimg2);

	//灰度图转换
	Mat grayimg1, grayimg2;
	cvtColor(srcimg1,grayimg1,CV_RGB2GRAY);
	cvtColor(srcimg2, grayimg2, CV_RGB2GRAY);

	//提取特征点
	SurfFeatureDetector surfDetector(2000);
	vector<KeyPoint>keyPoint1, keyPoint2;
	surfDetector.detect(grayimg1,keyPoint1);
	surfDetector.detect(grayimg2, keyPoint2);

	//特征点描述
	SurfDescriptorExtractor Descriptor;
	Mat imgDesc1, imgDesc2;
	Descriptor.compute(grayimg1,keyPoint1,imgDesc1);
	Descriptor.compute(grayimg2, keyPoint2, imgDesc2);

	FlannBasedMatcher matcher;
	vector<vector<DMatch>>matchpoints;

	vector<DMatch> GoodMatchePoints;

	vector<Mat> train_desc(1, imgDesc1);
	matcher.add(train_desc);
	matcher.train();
	
	matcher.knnMatch(imgDesc2, matchpoints, 2);
	cout << "total match points: " << matchpoints.size() << endl;

	//Low's 筛选最佳匹配点
	for (int i = 0; i < matchpoints.size(); i++)
	{
		if (matchpoints[i][0].distance < 0.4 * matchpoints[i][1].distance)
		{
			GoodMatchePoints.push_back(matchpoints[i][0]);
		}
	}
	Mat img_match;
	drawMatches(srcimg2,keyPoint2,srcimg1,keyPoint1,GoodMatchePoints,img_match);

	vector<Point2f> imgPoint1, imgPoint2;
	for (int i = 0; i < GoodMatchePoints.size(); i++)
	{
		imgPoint2.push_back(keyPoint2[GoodMatchePoints[i].queryIdx].pt);
		imgPoint1.push_back(keyPoint1[GoodMatchePoints[i].trainIdx].pt);
		
	}
	//获取图像1到图像2的投影映射矩阵，size3*3
	Mat homo = findHomography(imgPoint1,imgPoint2,CV_RANSAC);
	cout << "变换矩阵为：\n" << homo << endl << endl; //输出映射矩阵      
	 
	//计算匹配图的四个定点
	CalcCorners(homo,srcimg1);
	cout << "left_top:" << corners.left_top << endl;
	cout << "left_bottom:" << corners.left_bottom << endl;
	cout << "right_top:" << corners.right_top << endl;
	cout << "right_bottom:" << corners.right_bottom << endl;

	//图像配准
	Mat imgTransfrm1, imgTransfrm2;
	warpPerspective(srcimg1,imgTransfrm1,homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), srcimg2.rows));
	//imshow("透视矩阵变换",imgTransfrm1);

	//创建拼接后的图，提前计算图的大小
	int dst_width = imgTransfrm1.cols;//取最右点的长度为拼接图的长度
	int dst_height = srcimg2.rows;

	Mat dst(dst_height,dst_width,CV_8UC3);
	dst.setTo(0);
	imgTransfrm1.copyTo(dst(Rect(0,0,imgTransfrm1.cols,imgTransfrm1.rows)));
	srcimg2.copyTo(dst(Rect(0,0,srcimg2.cols,srcimg2.rows)));

	OptimizeSeam(srcimg2,imgTransfrm1,dst);
	//imshow("dst", dst);
	imwrite("E:\\Program Files\\Opencv\\图像拼接\\Image\\Image\\dst2.jpg", dst);
	waitKey();
	return 0;
}
//优化两图的连接处，使得拼接自然
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
{
	//开始位置，即重叠区域的左边界
	int start = MIN(corners.left_top.x,corners.left_bottom.x);
	double processWidth = img1.cols - start;//重叠区域的宽度
	int rows = dst.rows;
	int cols = img1.cols;//列数*通道数
	double alpha = 1;//img1中像素的权重  
	for (int  i = 0; i < rows; i++)
	{
		uchar* p = img1.ptr<uchar>(i);
		//获取第i行的首地址
		uchar* t = trans.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int  j= start; j < cols; j++)
		{
			//如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
			if (t[j * 3] == 0 && t[j*3+1]==0&&t[j*3+2]==0)
			{
				alpha = 1;
			}
			else
			{
				//img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
				alpha = (processWidth - (j - start)) / processWidth;
			}
			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
		}
	}



}