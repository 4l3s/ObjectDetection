/*

@All right reserved.
@summary: used to track the black line to correct the diretion of the car
@author: LiHao
@date: 2018.7.29
@refer: https://docs.opencv.org/master/
@location: DJI
*/





#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat frame;
VideoCapture capture;

//Ҫ���͵����ݼ���
struct CV_Data
{
	int distance[2];
	float slope[2];
};

//ʵ��������
CV_Data CD;

string IntToStr(int data)
{
	char ch_str[10] = { 0 };
	sprintf(ch_str, "%d", data);
	string  str_data(ch_str);
	return str_data;
}
/*@brief: ��㵽ֱ�߾���
*         ��бʽ�󷨺�
*/
int dst_calc(int x1, int y1, int x2, int y2, int tar_x = 144, int tar_y = 144)
{
	line(frame, Point(x1, y1), Point(x2, y2), Scalar(0, 155, 155), 3);
	double A, B, C, dst;
	A = y2 - y1;
	B = x1 - x2;
	C = x2 * y1 - x1 * y2;
	dst = abs(A*tar_x + B * tar_y + C) / sqrt(A*A + B * B);
	return int(dst);
}




//��ʼ��ͼ�β�Ȧ��
string detect(vector<Point> cnts_single, vector<Point> & approx)
{
	string shape = "undentified";
	double peri = arcLength(cnts_single, true);
	approxPolyDP(cnts_single, approx, 0.015 * peri, true);
	cout << "approx.size(): " << approx.size() << endl;
	if (approx.size() == 3)
	{
		shape = "triangle";
	}
	else if (approx.size() == 4)
	{
		shape = "rectangle";
		/*�����*/
	}
	else if (approx.size() == 5)
	{
		shape = "pentagon";
	}
	else if (approx.size() > 10)
	{
		shape = "SHizi";
	}

	return shape;
}





bool CV_Close = false; //���ڹر�����ͷCV
float slope_up;
int height;
int width;

int main()
{
	/*"C:\\Users\\10027\\Pictures\\Camera Roll\\final2.mp4"*/
	//frame = imread("C:\\Users\\10027\\Pictures\\Camera Roll\\final3_Moment.jpg");
	capture.open("C:\\Users\\10027\\Pictures\\Camera Roll\\final2.mp4");
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);//��� 
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);//�߶�
											   //���1
	if (!capture.isOpened())
	{
		printf("--(!)Error opening video capture\n"); return -1;
	}
	//��ʱ5�Σ� ȡƽ��ֵ
	int num = 0;
	while (capture.read(frame))		 //��μ�⣬ ȡƽ��ֵ
	{
		num++;
		//Sobel����

		Rect rect(200, 100, 320, 320);
		frame = frame(rect);
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(frame, frame, CV_8UC3, kernel);
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y, dst;
		//��x�����ݶ�
		Sobel(frame, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);
		//��y�����ݶ�
		Sobel(frame, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);
		//�ϲ��ݶ�
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

		//��ֵ���󷨺�
		dst.convertTo(dst, -1, 2.5, 50);
		cvtColor(dst, dst, COLOR_BGR2GRAY);
		imshow("Gray Frame", dst);
		threshold(dst, dst, 100, 255, CV_THRESH_BINARY);
		imshow("thres Frame", dst);

		Mat erode_element = getStructuringElement(MORPH_RECT, Size(1, 1));
		//��ʴ���ͼ��
		erode(dst, dst, erode_element, Point(-1, -1), 1);
		//����
		Mat dilate_element = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat dilate_out;     //���ͺ��ͼ��
		dilate(dst, dilate_out, dilate_element, Point(-1, -1), 6);

		//��ȡ��ɫ���ű߿�
		erode_element = getStructuringElement(MORPH_RECT, Size(4, 4));
		//�ٴθ�ʴ
		erode(dilate_out, dilate_out, erode_element, Point(-1, -1), 3);
		Rect rect_after(9, 9, 288, 288);
		dilate_out = dilate_out(rect_after);
		imshow("Dilate Frame", dilate_out);

		vector<vector<Point>>cnts;//��ȡ��һ����һ�ѵ�
		findContours(dilate_out, cnts, RETR_EXTERNAL, CHAIN_APPROX_NONE);





		for (int i = 0; i < cnts.size(); i++)
		{
			vector<Point> cnts_single = cnts[i];//��ȡ������һ�ѵ��е�һ����
			if (cnts_single.size() > 300)
			{
				vector<Point> approx;
				string shape = detect(cnts_single, approx);
				Moments M = moments(cnts_single);
				int cX, cY;
				if (M.m10 != 0)
				{
					//��ʾͼ������
					cX = int((M.m10 / M.m00));
					cY = int((M.m01 / M.m00));
				}
				else
				{
					cX = cY = 0;
				}
				line(frame, Point(320, 0), Point(320, 480), (255, 255, 255), 4);//����λ��

				cout << shape << endl;


				line(frame, Point(cX, cY), Point(0, cY), Scalar(255, 255, 255), 1);//��������
				line(frame, Point(cX, cY), Point(cX, 0), Scalar(255, 255, 255), 1);

				putText(frame, "shape: " + shape, Point(cX, cY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
				line(frame, Point(cX, cY), Point(cX, cY), (255, 255, 255), 5);
				putText(frame, "position " + IntToStr(cX) + " , " + IntToStr(cY), Point(cX, cY + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);  //����λ��

				float dst = 0;

				//�������������ʱpos_calc()����
				if (shape == "rectangle" || shape == "square")
				{
				}
				else if (shape == "SHizi")
				{
					line(frame, approx[0], approx[0], Scalar(0, 0, 255), 15);   putText(frame, "1", approx[0], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[1], approx[1], Scalar(0, 0, 255), 15);   putText(frame, "2", approx[1], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[2], approx[2], Scalar(0, 0, 255), 15);   putText(frame, "3", approx[2], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[3], approx[3], Scalar(0, 0, 255), 15);   putText(frame, "4", approx[3], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[4], approx[4], Scalar(0, 0, 255), 15);   putText(frame, "5", approx[4], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[5], approx[5], Scalar(0, 0, 255), 15);   putText(frame, "6", approx[5], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[6], approx[6], Scalar(0, 0, 255), 15);   putText(frame, "7", approx[6], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[7], approx[7], Scalar(0, 0, 255), 15);   putText(frame, "8", approx[7], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[8], approx[8], Scalar(0, 0, 255), 15);   putText(frame, "9", approx[8], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
					line(frame, approx[9], approx[9], Scalar(0, 0, 255), 15);   putText(frame, "10", approx[9], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);


					//��ֱ����
					int dst = dst_calc(approx[1].x, approx[1].y, approx[2].x, approx[2].y);
					CD.distance[1] += int(dst);
					putText(frame, "distance_x: " + IntToStr(dst), Point(cX, cY + 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);  //����λ��
					dst = dst_calc(approx[4].x, approx[4].y, approx[5].x, approx[5].y);
					CD.distance[1] += int(dst);
					putText(frame, "distance_y: " + IntToStr(dst), Point(cX, cY + 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);  //����λ��


																																			//б��

					height = float(approx[2].y - approx[1].y);
					width = float(-approx[2].x + approx[1].x);
					if (height != 0 && width != 0)
						slope_up = atan2(height, width) / (3.1415926 * 2) * 360 + 180;
					putText(frame, "slope_up: " + IntToStr(slope_up), Point(cX, cY + 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);  //����λ��

				}
			}
			else
			{
				//printf("too fucking small %d\n", cnts_single.size());
			}
		}
		imshow("Result Frame", frame);
		//waitKey(50);
		if (waitKey(50) == 'q' || CV_Close == true)
		{
			capture.release();
			break;
		} // escape
	}
	CD.distance[1] = CD.distance[1] / 5;
	CD.slope[1] = CD.slope[1] / 5;
	cout << CD.distance[1] << "      " << CD.slope[1] << endl;
	system("pause");
	return 0;
}


