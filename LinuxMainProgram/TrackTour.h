/*

@All right reserved.
@summary: used to track the black line to correct the diretion of the car
@author: LiHao
@date: 2018.7.24
@refer: https://www.opencv-python-tutroals.readthedocs.io
@location: DJI
*/

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat frame;
VideoCapture capture;

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
int dst_calc(int x1, int y1, int x2, int y2, int tar_x = 320, int tar_y = 240)
{
	double A, B, C, dst;
	A = y2 - y1;
	B = x1 - x2;
	C = x2 * y1 - x1 * y2;
	dst = abs(A*tar_x + B * tar_y + C) / sqrt(A*A + B * B);
	return int(dst);
}

/*
*@brief: λ�÷�������
*/
string pos_calc(Point pt1, Point  pt2, Point  pt3, Point  pt4, Point  center_point, float &dst, int camID)
{
	int dst = 0, line_x1, line_x2, line_y1, line_y2;
	float slope_up = 0;
	float height = 0.1, width = 0.1;
	if (pt2.y + 10 > 200)
	{
		putText(frame, "p1", Point(pt1.x, pt1.y + 10), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);//p1 λ��
		putText(frame, "p2", Point(pt2.x, pt2.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);//p2λ��
		putText(frame, "p3 ", Point(pt3.x, pt3.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);//p3λ��
		putText(frame, "p4 ", Point(pt4.x, pt4.y + 10), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);//p4λ��
		line_x1 = (pt1.x + pt4.x) / 2;
		line_x2 = (pt2.x + pt3.x) / 2;
		line_y1 = (pt1.y + pt4.y) / 2;
		line_y2 = (pt2.y + pt3.y) / 2;
		dst = dst_calc(line_x1, line_x2, line_y1, line_y2);

		height = float(pt1.y - pt2.y);
		width = float(-pt1.x + pt2.x);
	}
	else if (pt2.y + 10 < 100)
	{
		//��������ͷ����
		putText(frame, "p1 ", Point(pt1.x, pt1.y + 10), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2); //p1λ��
		putText(frame, "p2 ", Point(pt2.x, pt2.y + 10), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);  //p2λ��
		putText(frame, "p3 ", Point(pt3.x, pt3.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2); //p3λ��
		putText(frame, "p4 ", Point(pt4.x, pt4.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2); //p4λ��
		
		line_x1 = (pt1.x + pt2.x) / 2;
		line_x2 = (pt4.x + pt3.x) / 2;
		line_y1 = (pt1.y + pt2.y) / 2;
		line_y2 = (pt4.y + pt3.y) / 2;
		dst = dst_calc(line_x1, line_x2, line_y1, line_y2);
		
		height = float(pt2.y - pt3.y);
		width = float(-pt2.x + pt3.x);
	}
	else
	{
		float x = abs(320 - center_point.x) * abs(320 - center_point.x);//��ȡ��ͷ�������ʵ�ĺ���벢ƽ��
		float y = abs((240 - center_point.y)) * abs((240 - center_point.y)); //��ȡ��ͷ�������ʵ���ݾ��룬����ƽ��
		dst = sqrt(x + y);
	}
	//������ѧб�ʷ�
	if (height != 0 && width != 0)
		slope_up = float(height / width);
	else if (true)
		slope_up = 14;//��б�ʴ�����ֵ


	CD.slope[camID] += slope_up;
	//���ݽǶȷ����н�����
	if (slope_up >= 13 || slope_up <= -13)
	{
		printf("%f\n", slope_up);
		return "true";
	}
	else if (slope_up > 0)
	{
		printf("%f\n", slope_up);
		return "left";
	}
	else if (slope_up < 0)
	{
		printf("%f\n", slope_up);
		return "right";
	}
}


/*
*@brief:
*/
void drawLine(string shape, vector<Point> & approx)
{
	Point pt[13];
	if (shape == "rectangle" || shape == "sqaure")
	{
		pt[1] = approx[0];
		pt[2] = approx[1];
		pt[3] = approx[2];
		pt[4] = approx[3];
		line(frame, pt[1], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[3], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[3], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[1], Scalar(0, 255, 0), 3);
	}
	else if (shape == "pentagon")
	{
		pt[1] = approx[0];
		pt[2] = approx[1];
		pt[3] = approx[2];
		pt[4] = approx[3];
		pt[5] = approx[4];
		line(frame, pt[1], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[3], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[3], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[5], Scalar(0, 255, 0), 3);
		line(frame, pt[5], pt[1], Scalar(0, 255, 0), 3);
	}
	else if (shape == "cross")
	{
		pt[1] = approx[0];
		pt[2] = approx[1];
		pt[3] = approx[2];
		pt[4] = approx[3];
		pt[5] = approx[4];
		pt[6] = approx[5];
		line(frame, pt[1], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[3], pt[2], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[3], Scalar(0, 255, 0), 3);
		line(frame, pt[4], pt[5], Scalar(0, 255, 0), 3);
		line(frame, pt[5], pt[6], Scalar(0, 255, 0), 3);
		line(frame, pt[1], pt[1], Scalar(0, 255, 0), 3);
	}

}

//��ʼ��ͼ�β�Ȧ��
string detect(vector<Point> cnts_single, vector<Point> & approx)
{
	string shape = "undentified";
	double peri = arcLength(cnts_single, true);
	approxPolyDP(cnts_single, approx, 0.01 * peri, true);
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
	else
	{
		shape = "circle";
	}

	return shape;
}

//ƫ��״̬��ö������
enum Dirt_Status
{
	correct, left, right
};
//λ��״̬��ö������
enum Pos_Status
{
	correct, left, right
};

//Ҫ���͵����ݼ���
struct CV_Data
{
	int distance[2];
	float slope[2];
	Dirt_Status ds;	//No need
	Pos_Status ps;	//No need
};

//ʵ��������
CV_Data CD;

bool CV_Close = false; //���ڹر�����ͷCV


int OpenCV_Main(int camID)
{

	capture.open(camID);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);//��� 
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);//�߶�
	//���1
	if (!capture.isOpened())
	{
		printf("--(!)Error opening video capture\n"); return -1;
	}
	//��ʱ5�Σ� ȡƽ��ֵ
	int num = 0;
	while (capture.read(frame) && num < 5)
	{
		num++;
		//���2
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		//��ʽ�任
		Mat imgHSV;
		cvtColor(frame, imgHSV, COLOR_BGR2HSV);

		Scalar lower_black = Scalar(0, 0, 0);
		Scalar upper_black = Scalar(255, 255, 46);
		//ȡ��ɫ��Χ
		Mat imgThresHold;
		inRange(imgHSV, lower_black, upper_black, imgThresHold);

		Mat erode_element = getStructuringElement(MORPH_RECT, Size(2, 2));
		Mat erode_out;     //��ʴ���ͼ��
		erode(imgThresHold, erode_out, erode_element, Point(-1, -1), 1);

		Mat dilate_element = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilate_out;     //���ͺ��ͼ��
		dilate(erode_out, dilate_out, dilate_element, Point(-1, -1), 4);
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

				drawLine(shape, approx);  //������

				line(frame, Point(cX, cY), Point(0, cY), Scalar(255, 255, 255), 1);//��������
				line(frame, Point(cX, cY), Point(cX, 0), Scalar(255, 255, 255), 1);

				putText(frame, "black line " + shape, Point(cX, cY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
				line(frame, Point(cX, cY), Point(cX, cY), (255, 255, 255), 5);
				putText(frame, "position " + (cX) + " , " + IntToStr(cY), Point(cX, cY + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255), 2);  //����λ��

				float dst = 0;

				//�������������ʱpos_calc()����
				if (shape == "rectangle" || shape == "square")
				{
					string angle_status = "";
					angle_status = pos_calc(approx[0], approx[1], approx[2], approx[3], Point(cX, cY), dst, camID);
					//С���ܵ��ұ�ȥ��
					if (cX > 340)
					{
						CD.ps = Pos_Status::left;
						CD.distance[camID] += int(dst);
						putText(frame, "Left Off", Point(340, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);  //ƫ��״̬
						putText(frame, "Distance " + IntToStr(int(dst)), Point(cX, cY + 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);  //ƫ��״̬
					}
					else if (cX < 300)
					{
						CD.ps = Pos_Status::right;
						CD.distance[camID] += int(dst);
						putText(frame, "Right Off", Point(340, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);  //ƫ��״̬
						putText(frame, "Distance" + IntToStr(int(dst)), Point(cX, cY + 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);  //ƫ��״̬
					}
					else
					{
						CD.ps = Pos_Status::correct;
						CD.distance[camID] += int(dst);
						putText(frame, "Distance" + IntToStr(int(dst)), Point(cX, cY + 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);  //ƫ��״̬
						if (angle_status == "true")
						{
							CD.ds = Dirt_Status::correct;
							putText(frame, "Correct direction!", Point(320, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 4);
						}
						else if (angle_status == "left")
						{
							CD.ds = Dirt_Status::left;
							putText(frame, "Go right!", Point(320, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 4);
						}
						else if (angle_status == "right")
						{
							CD.ds = Dirt_Status::right;
							putText(frame, "Go left!", Point(320, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 4);
						}
						else
						{
							putText(frame, "No rectangle!!!", Point(320, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 4);
						}
					}
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
	CD.distance[camID] = CD.distance[camID] / 5;
	CD.slope[camID] = CD.slope[camID] / 5;
	return 0;
}