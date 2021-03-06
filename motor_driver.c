/**
 * @file motor_driver.c
 * @author Masatoshi Motohashi
 * @brief Library of Motor Driver
 * @version 0.1
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "com_prot.h"
#include "motor_driver.h"
#include "serial.h"

int mdv_ch[DRV_NMB + STR_NMB] = {5, 1, 8, 3, 6, 2, 7, 4};
int fd_array[DRV_NMB + STR_NMB] = {12, 8, 15, 10, 13, 9, 14, 11};

/**
 * @brief Motor Driver Initialization
 *
 */
void init_mdv(void) {
	mdv_duty_set(0, 0);		   //set all target 0
	printf("Set target 0\n");
}

/**
 * @brief Set Duty Set to Motor Driver
 *
 * @param mdv_num Motor Number
 * @param drv_dty Duty Ratio
 */
void mdv_duty_set(int mdv_num, int drv_dty) {
	char cmd_buf[SBUF_CMD_SIZE];
	char tlm_buf[SBUF_TLM_SIZE];
	int res;
	int fd;
	int num_cmd_dat;
	int slc_srl_nmb = fd_array[mdv_num];
	int fd_close_status = 0;
	memset(cmd_buf, 0, SBUF_CMD_SIZE);
	num_cmd_dat = make_mdv_cmd_duty_set(mdv_num, drv_dty, cmd_buf);

	fd = open_motor_serial(mdv_num);
	if(fd < 0) {
		printf("Serial Open Error\n");
	} else {
		res = write(fd, cmd_buf, num_cmd_dat);
		if(res < 0) {
			printf("Write Error for #%d: %s at mdv_duty_set\n", slc_srl_nmb, strerror(errno));
		}
		usleep(20000);
		res = read(fd, tlm_buf, SBUF_TLM_SIZE);
		fd_close_status = close(fd);
	}
}

/**
 * @brief Stop Motor
 *
 * @param mdv_num Motor Number
 * @param mtr_stat Stop Status
 */
void mdv_mtr_stp(int mdv_num, char mtr_stat) {
	char cmd_buf[SBUF_CMD_SIZE];
	char tlm_buf[SBUF_TLM_SIZE];
	int res;
	int fd;
	int num_cmd_dat;
	int slc_srl_nmb = fd_array[mdv_num];
	int fd_close_status = 0;
	memset(cmd_buf, 0, SBUF_CMD_SIZE);
	num_cmd_dat = make_mdv_cmd_mtr_stp(mdv_num, mtr_stat, cmd_buf);
	// printf("mdv_num:%d\tslc_srl_nmb:%d\tserial_fd:%d\n", mdv_num, slc_srl_nmb, serial_fd[slc_srl_nmb]);

	fd = open_motor_serial(mdv_num);
	if(fd < 0) {
		printf("Serial Open Error\n");
	} else {
		res = write(fd, cmd_buf, num_cmd_dat);
		if(res < 0) {
			printf("Write Error for #%d: %s at mdv_mtr_stp\n", slc_srl_nmb, strerror(errno));
		}
		usleep(20000);
		res = read(fd, tlm_buf, SBUF_TLM_SIZE);
		fd_close_status = close(fd);
	}
}

/**
 * @brief Clear encoder value
 * 
 * @param mdv_num Motor Number
 */
void mtr_enc_clr(int mdv_num) {
	char cmd_buf[SBUF_CMD_SIZE];
	char tlm_buf[SBUF_TLM_SIZE];
	int res;
	int fd;
	int num_cmd_dat;
	int slc_srl_nmb = fd_array[mdv_num];
	int fd_close_status = 0;
	memset(cmd_buf, 0, SBUF_CMD_SIZE);
	num_cmd_dat = make_mdv_cmd_enc_clr(mdv_num, cmd_buf);

	fd = open_motor_serial(mdv_num);
	if(fd < 0) {
		printf("Serial Open Error\n");
	} else {
		res = write(fd, cmd_buf, num_cmd_dat);
		if(res < 0) {
			printf("Write Error for #%d: %s at mtr_enc_clr\n", slc_srl_nmb, strerror(errno));
		}
		usleep(20000);
		res = read(fd, tlm_buf, SBUF_TLM_SIZE);
		fd_close_status = close(fd);
		if(res >= 0) {
			int i = 0;
			printf("Encoder Clear\t");
			for(i = 0; i < 10; i++) {
				printf("%x ", cmd_buf[i]);
			}
			printf("\n");
		}
	}
}

/**
 * @brief Get the Motor Control Parameter
 *
 * @param mdv_num Motor Number
 * @param ctrl_param Control Parameter to get
 */
long long get_mtr_ctrl_param(int mdv_num, char ctrl_param) {
	char cmd_buf[SBUF_CMD_SIZE];
	char tlm_buf[SBUF_TLM_SIZE];
	int res;
	int fd;
	int i;
	int num_cmd_dat;
	int slc_srl_nmb = fd_array[mdv_num];
	int fd_close_status = 0;
	long long param_val = 0;
	memset(cmd_buf, 0, SBUF_CMD_SIZE);
	memset(tlm_buf, 0, SBUF_CMD_SIZE);
	num_cmd_dat = make_mdv_cmd_tlm_snd(mdv_num, ctrl_param, cmd_buf);

	fd = open_motor_serial(mdv_num);
	if(fd < 0) {
		printf("Serial Open Error\n");
	} else {
		res = write(fd, cmd_buf, num_cmd_dat);

		if(res < 0) {
			printf("Write Error for #%d: %s\n at get_mtr_ctrl_param", slc_srl_nmb, strerror(errno));
		}
		usleep(20000);

		res = read(fd, tlm_buf, SBUF_TLM_SIZE);
		if(res < 0) {
			// printf("Read Error in telemetry at get_mtr_ctrl_param\n");
		} else {
			// tlm_buf[res] = 0;
		}
		fd_close_status = close(fd);
		if(res >= 0) {
			if(tlm_buf[3] == MDV_CMD_RETURN_TLM_SND) {		  // Read Encoder
				param_val += tlm_buf[7];
				param_val = param_val << 8;
				param_val += tlm_buf[6];
				param_val = param_val << 8;
				param_val += tlm_buf[5];
				param_val = param_val << 8;
				param_val += tlm_buf[4];
				// printf("%x ", tlm_buf[0]);
				// printf("%x ", tlm_buf[1]);
				// printf("%x\t", tlm_buf[2]);
				// printf("%x\t", tlm_buf[3]);
				// printf("%x ", tlm_buf[7]);
				// printf("%x ", tlm_buf[6]);
				// printf("%x ", tlm_buf[5]);
				// printf("%x ", tlm_buf[4]);
				// printf("\t%lld", param_val);
				// printf("\n");
			} else {
			}
		} else {
			printf("Error : Getting Control Parameter\t%d\t%x\n", res, tlm_buf[3]);
		}
	}
	return param_val;
}

/**
 * @brief Make Command to set duty
 *
 * @param mdv_num Motor Number
 * @param drv_dty Duty Ratio
 * @param buf Array to store the command
 * @return int Command Length(10)
 */
int make_mdv_cmd_duty_set(int mdv_num, int drv_dty, char* buf) {
	int n = 0, num = 0;
	unsigned short UH_tmp = 0;
	memset(buf, 0, SBUF_CMD_SIZE);

	buf[0] = 0x02;								   //Header
	buf[1] = (char)mdv_ch[mdv_num];				   //Driver ID
	buf[2] = (char)MDV_TLM_DTY_SET_DAT_SIZ;		   //Length of Command
	buf[3] = (char)MDV_CMD_DTY_SET;				   //Command ID
	itoc4(drv_dty, &buf[4]);					   // Controll Command
	UH_tmp = serial_checksum(&buf[1], 7);
	buf[8] = ustoc1(UH_tmp);		//check sum
	buf[9] = 0;
	return 10;
}

/**
 * @brief Make Command to Get Control Parameter
 *
 * @param mdv_num Motor Number
 * @param ctrl_parm Control Parameter
 * 		00h->Control Status
 * 		01h->Duty Ratio,
 * 		02h->Thermometer,
 * 		03h->Current,
 * 		04h->Absolute Value of Encorder Count,
 * 		05h->Relative Value of Encorder Count
 * @param buf Array to store the command
 * @return int Command Length(7)
 */
int make_mdv_cmd_tlm_snd(int mdv_num, int ctrl_parm, char* buf) {
	int n = 0, num = 0;
	unsigned short UH_tmp = 0;
	memset(buf, 0, SBUF_CMD_SIZE);

	buf[0] = 0x02;								   //Header
	buf[1] = (char)mdv_ch[mdv_num];				   //Driver ID
	buf[2] = (char)MDV_CMD_TLM_SND_DAT_SIZ;		   //Length of Command
	buf[3] = (char)MDV_CMD_TLM_SND;				   //Command ID
	buf[4] = (char)ctrl_parm;					   //Control Parameter
	UH_tmp = serial_checksum(&buf[1], 4);
	buf[5] = ustoc1(UH_tmp);		//check sum
	buf[6] = 0;
	return 7;
}

/**
 * @brief Make Command to Clear Encoder
 *
 * @param mdv_num Motor Number
 * @param buf Array to store the command
 * @return int Command Length(6)
 */
int make_mdv_cmd_enc_clr(int mdv_num, char* buf) {
	int n = 0, num = 0;
	unsigned short UH_tmp = 0;
	memset(buf, 0, SBUF_CMD_SIZE);

	buf[0] = 0x02;							 //Header
	buf[1] = (char)mdv_ch[mdv_num];			 //Driver ID
	buf[2] = MDV_CMD_ENC_CLR_DAT_SIZ;		 //Length of Command
	buf[3] = (char)MDV_CMD_ENC_CLR;			 //Command ID
	UH_tmp = serial_checksum(&buf[1], 3);
	buf[4] = ustoc1(UH_tmp);		//check sum
	buf[5] = 0;
	return 6;
}

/**
 * @brief Make Command to Stop Motor
 *
 * @param mdv_num Motor Number
 * @param mtr_stat Motor Stop Status
 * @param buf Array to store the command
 * @return int Command Length(7)
 */
int make_mdv_cmd_mtr_stp(int mdv_num, char mtr_stat, char* buf) {
	int n = 0, num = 0;
	unsigned short UH_tmp = 0;
	memset(buf, 0, SBUF_CMD_SIZE);

	buf[0] = 0x02;								   //Header
	buf[1] = (char)mdv_ch[mdv_num];				   //Driver ID
	buf[2] = (char)MDV_CMD_MTR_STP_DAT_SIZ;		   //Length of Command
	buf[3] = (char)MDV_CMD_MTR_STP;				   //Command ID
	buf[4] = mtr_stat;							   //Motor Stop Status
	UH_tmp = serial_checksum(&buf[1], 4);
	buf[5] = ustoc1(UH_tmp);		//check sum
	buf[6] = 0;
	return 7;
}

/**
 * @brief Create Serial Checksum
 *
 * @param addr Command
 * @param leng Command Length
 * @return unsigned short Checksum
 */
unsigned short serial_checksum(char* addr, int leng) {
	int nleft = leng;		 //1byte order
	int sum = 0;
	char* w = addr;
	unsigned short answer = 0;

	while(nleft > 0) {
		sum += *w++;
		nleft -= 1;
	}
	sum = (sum >> 16) + (sum & 0xffff);		   //0xffff ?????????1,addition of upper and lower value
	sum += (sum >> 16);						   //adding carry value
	answer = sum;							   //inversion of value
	return (answer);
}