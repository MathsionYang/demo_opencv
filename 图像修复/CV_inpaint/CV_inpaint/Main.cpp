#include <imgproc\imgproc.hpp>
#include <highgui\highgui.hpp>
#include <photo\photo.hpp>
#include <core/core.hpp>
#include<opencv2\opencv.hpp> 
using namespace cv;
using namespace std;
Point ptl, ptr;//��껭�����ο�������յ�
Mat ROI;//ԭͼ��Ҫ�޸�������
Mat  SrcImage, SrcImageCopy;
void OnMouse(int event, int x, int y, int flag, void *ustg);

int main()
{
	 SrcImage = imread("C:\\Users\\yang\\Desktop\\image\\dema1.jpg");
	if (!SrcImage.data)
	{
		return -1;
	}
	imshow("ԭͼ",SrcImage);
	cvSetMouseCallback("ԭͼ", OnMouse);
	waitKey();
	return 0;
}
	//ȫ������ֵ����+Mask���ʹ���
	
	////ȫ������ֵ����+Mask���ʹ���
	//Mat GrayImage;
	////�Ҷ�ת��
	//cvtColor(SrcImage,GrayImage,CV_RGB2GRAY,0);
	//Mat MaskImage;
	//Mat(SrcImage.size(),CV_8UC1,Scalar::all(0));
	////ͨ����ֵ����Mask
	//threshold(GrayImage,MaskImage,240,255,CV_THRESH_BINARY);
	//Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	////��mask��������
	//Mat DilaMAsk;
	//dilate(MaskImage, DilaMAsk,Kernel);
	//inpaint(SrcImage, DilaMAsk,SrcImage,5,INPAINT_TELEA);
	//imshow("MASK",MaskImage);
	//imshow("�޸���",SrcImage);//ȫ������ֵ����+Mask���ʹ���
	
	//���Ȧ��������ֵ����+Mask���ʹ���
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
		imshow("ԭͼ",SrcImageCopy);
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
	////��������Ҽ���ʼͼ���޸�
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		//SrcImageCopy = ROI.clone();
		//Mat GrayImage;
		//cvtColor(ROI,GrayImage,CV_RGB2GRAY);//�Ҷ�ת��
		//Mat MaskImage = Mat(ROI.size(),CV_8UC1,Scalar::all(0));
		////ͨ����ֵ����Mask
		//threshold(GrayImage, MaskImage, 235, 255, CV_THRESH_BINARY);
		//Mat Kernel = getStructuringElement(MORPH_RECT,Size(3,3));
		//dilate(MaskImage,MaskImage,Kernel);//mask����
		//inpaint(ROI,MaskImage,ROI,9,INPAINT_TELEA);//�޸�
		//imshow("�޸���",SrcImage);
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //PNG��ʽͼƬ��ѹ������  
		compression_params.push_back(9);  //�������ñ����ͼ����������

		imwrite("test3.png", ROI, compression_params);
		

	}
}







