#include "stdafx.h"
#include <iostream>
#include <opencv.hpp>
#include <string>
using namespace std;
using namespace cv;
int main()
{

	Mat M(480, 640, CV_8UC3, Scalar(0, 0, 255));

	imshow("M", M);
	waitKey(0);
	return 0;
}