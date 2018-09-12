#include "gradientUtil.h"
#include "wrappers.h"
#include "CellArray.h"
#include "EdgeDetector.h"
#include "EdgeBoxes.h"

#include <time.h>
#include <opencv2\opencv.hpp>
#include <opencv2\ximgproc.hpp>

using namespace cv;
using namespace cv::ximgproc;

float IoU(Box box, cv::Rect rect);

int main()
{
	Mat3f fsrc;
	std::vector<cv::Point> edge_seg;

	CellArray E, O;
	EdgeDetector detector;
	EdgeBoxes edgeBoxes;

	edgeBoxes.initialize(.65, .75, 1, .01, 1000, .1, .5, .5, 3, 1000, 2, 1.5);

	Mat3b src = imread("new pho\\2.jpg");

	if (!src.data){
		std::cout << "Can not load img!" << std::endl;
		return 0;
	}

	int h = src.rows, w = src.cols, d = 1;

	src.convertTo(fsrc, CV_32F, 1.0 / 255.0);

	Ptr<RFFeatureGetter> rfptr = createRFFeatureGetter();
	Ptr<StructuredEdgeDetection> pDollar = createStructuredEdgeDetection("model.yml.gz", rfptr);

	Mat1f edges; Mat edg = Mat::zeros(src.size(), CV_8UC1);
	pDollar->detectEdges(fsrc, edges);

	edges.convertTo(edg, CV_8UC1, 255.0);

	imshow("dff", edg);
	//waitKey(0);

	//for (int j = 0; j < h; j++)
	//{
	//	uchar* edg_s = edg.ptr<uchar>(j);
	//	uchar* ee_s = edges.ptr<uchar>(j);
	//	for (int i = 0; i < w; i++)
	//	{
	//		edg_s[i] = cvRound(edges.at<float>(j, i) * 255);
	//	}
	//}
	//imshow("sss", edges);
	//imshow("dff", edg);
	//waitKey(0);

	E.rows = h, E.cols = w, E.channels = 1;
	E.data = (float*)wrCalloc(w * h, sizeof(float));

	for (int k = 0; k < d; ++k) {
		for (int c = 0; c < w; ++c) {
			for (int r = 0; r < h; ++r) {
				E.data[k * w * h + c * h + r] = ((uint8_t*)edg.data)[r * w * d + c * d + k];
			}
		}
	}

	uint8_t* I = (uint8_t*)wrCalloc(h * w, sizeof(uint8_t)); // 没用到
	detector.edgesDetect(I, h, w, d, E, O);

	clock_t b = clock();
	Boxes boxes = edgeBoxes.generate(E, O);
	clock_t c = clock();

	std::cerr << "edge boxes: " << double(c - b) / CLOCKS_PER_SEC << "ms" << std::endl;
	std::cerr << "number of boxes: " << boxes.size() << std::endl;

	// 输出所有的框 ==========================================================
#if 0
	cv::Mat roi_img1, roi_img2;
	std::vector<cv::Rect> box_img;
	char* filename1 = (char*)malloc(sizeof(char)* boxes.size());
	char* filename2 = (char*)malloc(sizeof(char)* boxes.size());
	for (size_t i = 0; i < boxes.size(); i++){
		box_img.push_back(cv::Rect(boxes[i].c, boxes[i].r, boxes[i].w, boxes[i].h));
	}
	for (size_t j = 0; j < box_img.size(); j++){
		src(box_img[j]).copyTo(roi_img1);
		edges(box_img[j]).copyTo(roi_img2);
		sprintf(filename1, "E:/我的/test demo/edgeBoxes/box maps/%d.jpg", j);
		sprintf(filename2, "E:/我的/test demo/edgeBoxes/roi maps/%d.jpg", j);
		cv::imwrite(filename1, roi_img1);
		cv::imwrite(filename2, roi_img2);
	}
	free(filename1);
	free(filename2);
#endif
	//=========================================================================

	Box max_b;
	cv::Rect rect, a;
	// gt 
	rect.x = 56, rect.y = 63;
	rect.width = 228, rect.height = 227;

	cv::rectangle(src, rect, cv::Scalar(0, 255, 0), 2, 8, 0);

	// 输出框到 boxes.txt
	FILE* fp = fopen("boxes.txt", "w");
	for (int i = 0; i < boxes.size(); i++){
		fprintf(fp, "%d %d %d %d %f\n", boxes[i].c, boxes[i].r, boxes[i].w, boxes[i].h, boxes[i].s);
	}
	fclose(fp);

	float score_max = 0.0f, ratio_max = 0.0f;
	for (int i = 0; i < boxes.size(); i++){
		Box box = boxes[i];
		float ratio = IoU(box, rect);
		if (ratio == 0.7f){
			if (box.s > score_max){
				score_max = box.s;
				ratio_max = ratio;
				max_b = box;
				//std::cout << i << std::endl;
			}
		}
	}
	a.x = max_b.c, a.y = max_b.r;
	a.width = max_b.w, a.height = max_b.h;
	cv::rectangle(src, a, cv::Scalar(255, 0, 0), 2, 8, 0);

	std::cout << "The box's score is: " << max_b.s << std::endl;
	std::cout << "The box's IoU is: " << ratio_max << std::endl;

	cv::imshow("original map", src);
	waitKey(0);

	return 0;
}
// 计算 IoU
float IoU(Box box, cv::Rect rect)
{
	float ratio;
	int x1 = box.c, w1 = box.w;
	int x2 = rect.x, w2 = rect.width;

	int y1 = box.r, h1 = box.h;
	int y2 = rect.y, h2 = rect.height;

	int endx = (x1 + w1) > (x2 + w2) ? (x1 + w1) : (x2 + w2);
	int stax = x1 > x2 ? x2 : x1;
	int ww = w1 + w2 - (endx - stax);

	int endy = (y1 + h1) > (y2 + h2) ? (y1 + h1) : (y2 + h2);
	int stay = y1 > y2 ? y2 : y1;
	int hh = h1 + h2 - (endy - stay);

	if (ww <= 0 || hh <= 0){
		return 0;   //  无重叠部分
	}
	else{
		int area = ww * hh;
		int area1 = box.w * box.h;
		int area2 = rect.width * rect.height;
		ratio = area * 1.0f / (area1 + area2 - area);
	}
	ratio = cvRound(ratio * 10) / 10.0f;

	return ratio;
}
