/**
 * @file serial.c
 * @author Masatoshi Motohashi
 * @brief Library of Serial
 * @version 0.1
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "serial.h"

int serial_fd[20];
struct termios oldtio[20], newtio[20];

/**
 * @brief Initalize Serial
 *
 * @param srl_num Serial Number
 */
int init_serial(int srl_num) {
	int open_flag = 0;
	int res;
	int fd;							 //ファイルディスクリプタ
	struct termios old_dummy;		 //シリアル通信設定

	//Open Serial Port
	switch(srl_num) {
		case 0:		   //Resolver1
			fd = open(SERIAL_PORT_0, O_RDWR | O_NOCTTY | O_NONBLOCK);
			//fd = open(SERIAL_PORT_0, O_RDWR) : デバイスをオープンする
			//O_NOCTTY : オープンしたファイルは制御端末にならない
			//O_NONBLOCK : ファイルは非停止モードでオープンされる
			break;
		case 1:
			fd = open(SERIAL_PORT_1, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 2:
			fd = open(SERIAL_PORT_2, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 3:
			fd = open(SERIAL_PORT_3, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 4:
			fd = open(SERIAL_PORT_4, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 5:
			fd = open(SERIAL_PORT_5, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 6:
			fd = open(SERIAL_PORT_6, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 7:
			fd = open(SERIAL_PORT_7, O_RDWR | O_NOCTTY | O_NONBLOCK);
			break;
		case 8:
			fd = open(SERIAL_PORT_USB0, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 9:
			fd = open(SERIAL_PORT_USB1, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 10:
			fd = open(SERIAL_PORT_USB2, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 11:
			fd = open(SERIAL_PORT_USB3, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 12:
			fd = open(SERIAL_PORT_USB4, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 13:
			fd = open(SERIAL_PORT_USB5, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 14:
			fd = open(SERIAL_PORT_USB6, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 15:
			fd = open(SERIAL_PORT_USB7, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 16:
			fd = open(SERIAL_PORT_USB8, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 17:
			fd = open(SERIAL_PORT_USB9, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 18:
			fd = open(SERIAL_PORT_USB10, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		case 19:
			fd = open(SERIAL_PORT_USB11, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
			break;
		default:
			fd = open(SERIAL_PORT_0, O_RDWR | O_NOCTTY | O_NONBLOCK);
			usleep(2000);
	}

	serial_fd[srl_num] = fd;		//setup global variables

	if(fd < 0) {
		printf("Fail to open serial for driver #%d\n", srl_num);
		return fd;
	} else {
		// printf("Open serial port #%d \n", srl_num);
		// printf("fd=%d, serial_fd[%d]=%d\n", fd, srl_num, serial_fd[srl_num]);
	}

	//res=tcgetattr(fd, &oldtio[srl_num]);       // 現在のシリアルポートの設定を待避させる
	res = tcgetattr(fd, &old_dummy);		// 通信処理
	if(res) {
		printf("tcgetattr error \n");
		close(fd);
		printf("tcgetattr close\n");
		return -1;
	}

	memset(&newtio[srl_num], 0, sizeof(newtio[srl_num]));
	newtio[srl_num] = old_dummy;		// ポートの設定をコピー
	newtio[srl_num].c_cflag = CS8 | CLOCAL | CREAD;

	if((srl_num >= 8) && (srl_num <= 19)) {
		newtio[srl_num].c_iflag = ICRNL | IGNPAR;
		newtio[srl_num].c_cc[VMIN] = 1;		   //
		newtio[srl_num].c_cc[VEOF] = 0;
		newtio[srl_num].c_cc[VEOL] = 0;
		newtio[srl_num].c_oflag = 0;
		newtio[srl_num].c_lflag = 0;
	}

	newtio[srl_num].c_cc[VTIME] = 0; /* キャラクタ間タイマを使わない */

	if((srl_num >= 8) && (srl_num <= 19)) {
		res = cfsetispeed(&newtio[srl_num], SERIAL_BAUDRATE_MDV);
		if(res < 0)
			printf("cfsetspeed error \n");
		res = cfsetospeed(&newtio[srl_num], SERIAL_BAUDRATE_MDV);
		if(res < 0)
			printf("cfsetspeed error \n");
	} else if((srl_num == 0) || (srl_num == 1)) {
		res = cfsetispeed(&newtio[srl_num], SERIAL_BAUDRATE_RU);
		if(res < 0)
			printf("cfsetspeed error \n");
		res = cfsetospeed(&newtio[srl_num], SERIAL_BAUDRATE_RU);
		if(res < 0)
			printf("cfsetspeed error \n");
	} else {
		res = cfsetispeed(&newtio[srl_num], SERIAL_BAUDRATE_NMN);
		if(res < 0)
			printf("cfsetspeed error \n");
		res = cfsetospeed(&newtio[srl_num], SERIAL_BAUDRATE_NMN);
		if(res < 0)
			printf("cfsetspeed error \n");
	}

	res = tcflush(fd, TCIFLUSH);
	if(res < 0) {
		printf("tcflush error \n");
		close(fd);
		printf("tcflush close\n");
		return -1;
	}

	res = tcsetattr(fd, TCSANOW, &newtio[srl_num]);		   // ポートの設定を有効にする
	if(res) {
		printf("tcsetattr error \n");
		close(fd);
		printf("tcsetattr close\n");
		return -1;
	}
	oldtio[srl_num] = old_dummy;
	return fd;
}