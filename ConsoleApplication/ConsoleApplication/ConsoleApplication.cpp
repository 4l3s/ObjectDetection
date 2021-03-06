//#include "stdafx.h"
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//using namespace cv;
//int main()
//{
//	int i = 1;
//	
//	Mat src_img = imread("C:\\Users\\10027\\source\\repos\\ObjectDetection\\PythonApplication\\sample.jpg");
//	Mat mid_img;
//	cvtColor(src_img, mid_img, CV_BGR2GRAY);			  //灰度化图像
//	threshold(mid_img, mid_img, 30, 255, THRESH_BINARY);  //二值化图像
//	vector<vector<Point>> cnts;
//	findContours(mid_img, cnts, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//	for (int i = 0; i < cnts.size(); i++)
//	{
//		for (int j = 0; j < cnts[i].size(); j++)
//		{
//			line(src_img, cnts[i][j], cnts[i][j], Scalar(0, 255, 0), 1);
//		}
//		int a = 0;
//	}
//
//	
//	
//	waitKey(0);
//	return 0;
//}
//


#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

int main()
{
	Mat skin = imread("C:\\Users\\10027\\Desktop\\skin.jpg");
	imshow("skin", skin);
	Mat dstSkin;
	cvtColor(skin, dstSkin, CV_BGR2GRAY);
	putText(dstSkin, "dst:120-130 ", Point(201, 907), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);
	putText(dstSkin, "dst:190-200 ", Point(576, 1004), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);
	putText(dstSkin, "dst:140-150 ", Point(576, 838), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);
	putText(dstSkin, "dst:160-170 ", Point(787, 969), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);
	imshow("dstSkin", dstSkin);
	waitKey(0);
	return 0;
}