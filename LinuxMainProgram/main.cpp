#include <iostream>
#include "SerialPort.h"
#include "data_processing.h"

using namespace std;

int main()
{
	//��ʼ������
	int fd;
	char *dev = "/dev/ttyTHS2"; //Uart1
	sleep(1);
	fd = OpenDev(dev); //1open uart
	if (fd>0)
	{
		set_speed(fd, 19200); //2set speed
	}
	else
	{
		printf("Can't Open Serial Port!\n");
		exit(0);
	}
	if (set_Parity(fd, 8, 1, 'N') == FALSE) //3set parity
	{
		printf("Set Parity Error\n");
		exit(1);
	}



	lcm::LCM lcm;
	if (!lcm.good())
		return 1;
	Point_Data this_pos;
	Action_Data this_action;
	Handler handlerObject;

	lcm.subscribe("SENDHEAD", &Handler::handleMessage1, &handlerObject);
	while (1)
	{

		
		switch (start_area)
		{
		case 1:
			start_data.x = -930 / 2;
			start_data.y = -930 / 2;
			break;
		case 2:
			start_data.x = -930 / 2;
			start_data.y = 930 * 8+ 930 / 2;
			break;
		case 3:
			start_data.x = 930 * 8+ 930 / 2;
			start_data.y = 930 * 8+ 930 / 2;
			break;
		case 4:
			start_data.x = 930 * 8+ 930 / 2;
			start_data.y = -930 / 2;
			break;
		default:
			lcm.subscribe("SENDHEAD", &Handler::handleMessage1, &handlerObject);
			break;
		}
		this_pos.x = 0;
		this_pos.y = 0;

		for (int i = 0; i < run_times; ++i)
		{
			lcm.subscribe("SENDNORMAL", &Handler::handleMessage1, &handlerObject, i);
			int flag = 0;
			Point_Data resource_point_data[van[i].num + 10];
			for (int j = 0; j < van[i].num; ++j)
			{

				int id = van[i].resource[j];
				int temp1 = id / 100, temp2 = (id / 10) % 10, temp3 = id % 10;
				switch (temp1)
				{
				case 1:
					int py = 0;
					if (temp3 == 3)py = 1;
					if (temp3 == 4)py = 2;
					if (temp3 == 5)py = 3;
					resource_point_data[j].x = float(temp2) * 930 + 310 * py;
					resource_point_data[j].y = 7 * 930;
					resource_point_data[j].dir = 0;
					break;
				case 2:
					int py = 0;
					if (temp3 == 3)py = 1;
					if (temp3 == 4)py = 2;
					if (temp3 == 5)py = 3;
					resource_point_data[j].x = 7 * 930;
					resource_point_data[j].y = (7 - temp2) * 930 + (4 - py) * 310;
					resource_point_data[j].dir = 90;
					break;
				case 3:
					int py = 0;
					if (temp3 == 3)py = 1;
					if (temp3 == 4)py = 2;
					if (temp3 == 5)py = 3;
					resource_point_data[j].x = (7 - temp2) * 930 + (4 - py) * 310;
					resource_point_data[j].y = 0.0;
					resource_point_data[j].dir = 180;
					break;
				case 4:
					int py = 0;
					if (temp3 == 3)py = 1;
					if (temp3 == 4)py = 2;
					if (temp3 == 5)py = 3;
					resource_point_data[j].x = 0;
					resource_point_data[j].y = float(temp2) * 930 + (py) * 310;
					resource_point_data[j].dir = 270;
					break;
				default:
					continue;
					break;
				}

				if (resource_point_data[j].y == this_pos.y)
				{
					//ƽ��
					this_action.dir = 1;
					this_action.target_point = resource_point_data[j];

					//moveit();
					//getit();//����this_action.target_point.dir-----������ת����
				}
				else
				{
					//
					this_action.dir = 1;//dir==1��ʾƽ�ƣ�dir==0��ʾֱ��
					this_action.target_point.y = this_pos.y;
					this_action.target_point.x = resource_point_data[j].x / 930 * 930;
					string pos_string = "G0" + to_string(1) + " X" + to_string(resource_point_data[j].x / 930 * 930) + " Y" + to_string(this_pos.y) + " Z0" + ";";
					Trans_Port(fd, pos_string);
					Calibrate_Port(fd);
					//moveit();//X=this_action.target_point.x//����ƶ���Ŀ��������
					this_action.dir = 0;
					this_action.target_point.y = resource_point_data[j].y;
					this_action.target_point.x = this_pos.x;
					string pos_string = "G0" + to_string(0) + " X" + to_string(this_pos.x) + " Y" + to_string(resource_point_data[j].y) + " Z0" + ";";
					Trans_Port(fd, pos_string);
					//moveit();
					this_action.dir = 1;
					this_action.target_point.y = this_pos.y;
					this_action.target_point.x = resource_point_data[j].x;
					string pos_string = "G0" + to_string(1) + " X" + to_string(resource_point_data[j].x) + " Y" + to_string(this_pos.y) + " Z0" + ";";
					Trans_Port(fd, pos_string);
					//moveit();
					this_action.target_point.dir = resource_point_data[j].dir;//������ת����
					string pos_string = "G0" + to_string(1) + " X" + to_string(0) + " Y" + to_string(0) + " Z" + to_string(resource_point_data[j].dir) + ";";
					Trans_Port(fd, pos_string);
					//getit();//ץȡʱ���õ�����ת���򣬲���ץȡ����ת��0��
				}

			}
			Point_Data work_point_data[van[i].num + 10];
			for (int j = 0; j < van[i].num; ++j)
			{
				int id = van[i].place[j];


				int y = (id - 1) / 8;
				int x = (id - 1) % 8;
				work_point_data[j].x = 930 * x + 930 / 2;
				work_point_data[j].y = 930 * y;

				this_action.dir = 1;
				this_action.target_point.y = this_pos.y;
				this_action.target_point.x = this_pos.x / 930 * 930 + 930;
				string pos_string = "G0" + to_string(1) + " X" + to_string(this_pos.x / 930 * 930 + 930) + " Y" + to_string(this_pos.y) + " Z0" + ";";
				Trans_Port(fd, pos_string);
				//moveit();

				this_action.dir = 0;
				this_action.target_point.y = work_point_data[j].y;
				this_action.target_point.x = this_pos.x;
				string pos_string = "G0" + to_string(1) + " X" + to_string(this_pos.x) + " Y" + to_string(work_point_data[j].y) + " Z0" + ";";
				Trans_Port(fd, pos_string);
				//moveit();

				this_action.dir = 1;
				this_action.target_point.y = this_pos.y;
				this_action.target_point.x = work_point_data[j].x;
				string pos_string = "G0" + to_string(1) + " X" + to_string(this_pos.x / 930 * 930 + 930) + " Y" + to_string(this_pos.y) + " Z0" + ";";
				Trans_Port(fd, pos_string);
				//moveit();
				//putit();//���¿�

			}

		}
		while (0 == lcm.handle());

	}	   
	close(fd);
	exit(0);
	return 0;
}
