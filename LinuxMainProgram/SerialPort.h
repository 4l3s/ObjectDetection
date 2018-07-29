/*
@All right reserved.
@summary: used to Serial communicate between STM32 MCU and Ubuntu14.04
@author: LiHao
@date: 2018.7.25
@refer: https://www.csdn.com
@location: DJI
*/

#include <stdio.h>      /*��׼�����������*/
#include <stdlib.h>     /*��׼�����ⶨ��*/
#include <unistd.h>     /*Unix��׼��������*/
#include <sys/types.h>  /**/
#include <sys/stat.h>   /**/
#include <fcntl.h>      /*�ļ����ƶ���*/
#include <termios.h>    /*PPSIX�ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
#include <string.h>
#include <string>

#include "TrackTour.h"




/*@brief  ���ô���ͨ������
*@param  fd     ���� int  �򿪴��ڵ��ļ����
*@param  speed  ���� int  �����ٶ�
*@return  void
*/
#define FALSE 1
#define TRUE 0

char *recchr = "We received:\"";

int speed_arr[] = { B921600, B460800, B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 921600, 460800, 230400, 115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,  19200,  9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed)
{
	int   i;
	int   status;
	struct termios   Opt;
	tcgetattr(fd, &Opt);
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
	{
		if (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0)
				perror("tcsetattr fd1");
			return;
		}
		tcflush(fd, TCIOFLUSH);
	}
}
/*
*@brief  ���ô�������λ��ֹͣλ��Ч��λ
*@param  fd       ����  int  �򿪵Ĵ����ļ����*
*@param  databits ����  int  ����λ   ȡֵ Ϊ 7 ����8*
*@param  stopbits ����  int  ֹͣλ   ȡֵΪ 1 ����2*
*@param  parity   ����  int  Ч������ ȡֵΪN,E,O,,S
*/
int set_Parity(int fd, int databits, int stopbits, int parity)
{
	struct termios options;
	if (tcgetattr(fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) /*��������λ��*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size\n");
		return (FALSE);
	}
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);  /* ����Ϊ��Ч��*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* ת��ΪżЧ��*/
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity\n");
		return (FALSE);
	}
	/* ����ֹͣλ*/
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported stop bits\n");
		return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}
/*
*@breif �򿪴���
*/
int OpenDev(char *Dev)
{
	int fd = open(Dev, O_RDWR | O_NONBLOCK);         //| O_NOCTTY | O_NDELAY
	if (-1 == fd)
	{ /*��������λ��*/
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		return fd;

}

/*
*@breif:  �����˶�����
*@usage:  ��������Ȼ��ȴ��ش�
*/
void Trans_Port(int fd, string trans_str)
{
	char buff[512];
	bool checkStauts = false;
	int nread;

	write(fd, trans_str.c_str(), trans_str.length());
}


/*
*@brief: ��������ͷУ׼����
*@usage���ڵ���Ŀ�ĵغ���øú�������У׼���ݴ���
*/
void Calibrate_Port(int fd)
{
	int distance_x, distance_y, slope;
	if (OpenCV_Main(0) == 0)
	{
		distance_x = CD.distance[0];
	}
	if (OpenCV_Main(1) == 0)
	{
		distance_y = CD.distance[1];
		slope = CD.slope[1];
	}
	string pos_string = "G1 X" + IntToStr(distance_x) + " Y" + IntToStr(distance_y) + " Z" + IntToStr(slope) + ";";
	Trans_Port(fd, pos_string);

	char buff[512];
	bool checkStauts = false;
	int nread;

	
}