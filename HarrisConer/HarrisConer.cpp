//// HarrisConer.cpp: 定义控制台应用程序的入口点。
////
//
//#include "stdafx.h"
//
///**
//* @function cornerHarris_Demo.cpp
//* @brief Demo code for detecting corners using Harris-Stephens method
//* @author OpenCV team
//*/
//
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
///// Global variables
//Mat src, src_gray;
//int thresh = 200;
//int max_thresh = 255;
//
//const char* source_window = "Source image";
//const char* corners_window = "Corners detected";
//
///// Function header
//void cornerHarris_demo(int, void*);
//
///**
//* @function main
//*/
//int main(int argc, char** argv)
//{
//	/// Load source image and convert it to gray
//	CommandLineParser parser(argc, argv, "{@input | C:/Users/10027/source/repos/ObjectDetection/HarrisConer/data/building.png | input image}");
//	src = imread(parser.get<String>("@input"), IMREAD_COLOR);
//	if (src.empty())
//	{
//		cout << "Could not open or find the image!/n" << endl;
//		cout << "Usage: " << argv[0] << " <Input image>" << endl;
//		return -1;
//	}
//	cvtColor(src, src_gray, COLOR_BGR2GRAY);
//
//	/// Create a window and a trackbar
//	namedWindow(source_window, WINDOW_AUTOSIZE);
//	createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
//	imshow(source_window, src);
//
//	cornerHarris_demo(0, 0);
//
//	waitKey(0);
//	return(0);
//}
//
///**
//* @function cornerHarris_demo
//* @brief Executes the corner detection and draw a circle around the possible corners
//*/
//void cornerHarris_demo(int, void*)
//{
//
//	Mat dst, dst_norm, dst_norm_scaled;
//	dst = Mat::zeros(src.size(), CV_32FC1);
//
//	/// Detector parameters
//	int blockSize = 2;
//	int apertureSize = 3;
//	double k = 0.04;
//
//	/// Detecting corners
//	cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
//
//	/// Normalizing
//	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());	   //归一化原图像, 解决光照问题？，别问我是什么东西
//	convertScaleAbs(dst_norm, dst_norm_scaled);			  //转为8位图像
//
//	/// Drawing a circle around corners
//	for (int j = 0; j < dst_norm.rows; j++)
//	{
//		for (int i = 0; i < dst_norm.cols; i++)
//		{
//			if ((int)dst_norm.at<float>(j, i) > thresh)
//			{
//				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);	   //Scalar(0)设置为灰度值
//			}
//		}
//	}
//	/// Showing the result
//	namedWindow(corners_window, WINDOW_AUTOSIZE);
//	imshow(corners_window, dst_norm_scaled);
//}
//
//
/**
* @file CannyDetector_Demo.cpp
* @brief Sample code showing how to detect edges using the Canny Detector
* @author OpenCV team
*/

//#include "stdafx.h"
//#include <iostream>
//#include <opencv.hpp>
//
//using namespace std;
//using namespace cv;
//
//int main()
//{
//	Mat img = imread("C:\\Users\\10027\\source\\repos\\ObjectDetection\\PythonApplication\\RoboSample\\mix\\mix1.jpg", IMREAD_COLOR);
//	vector<Mat> channel;
//	split(img, channel);
//	Mat blueimg = channel.at(0);
//	Mat greenimg = channel.at(1);
//	Mat redimg = channel.at(2);
//	//Mat blueimg_ = blueimg < 100;
//	imshow("hello", blueimg);
//	waitKey(0);
//	return 0;
//}

