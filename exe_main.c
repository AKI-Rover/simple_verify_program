/**
 * @file exe_main.c
 * @author Masatoshi Motohashi
 * @brief
 * @version 0.1
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "motor_driver.h"
#include "serial.h"

// 0 -> Left Front Drive
// 1 -> Right Front Drive
// 2 -> Left Rear Drive
// 3 -> Right Read Drive
// 4 -> Left Front Stir
// 5 -> Right Front Stir
// 6 -> Left Rear Stir
// 7 -> Right Rear Stir
int motor_num = 2;

/**
 * @brief Ctrl-C Handler
 *
 */
void ctrl_c_handler() {
	printf("\n\nCtrl_C\n");
	mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
	exit(1);
}

int main(int argc, char* argv[]) {
	printf("Initializing\n");
	signal(SIGINT, ctrl_c_handler);		   //Register Ctrl-C Handler

	init_motor_serial(motor_num);
	mtr_enc_clr(motor_num);
	sleep(1);

	int cnt = 0;
	for(cnt = 0; cnt < 1; cnt++) {
		printf("Count : %d\n", cnt);
		printf("Motor Stop\n");
		mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
		get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
		sleep(1);

		printf("Motor Accelerate\n");
		int i = 0;
		int max_duty = 500;
		for(i = 0; i <= max_duty; i++) {
			// mdv_duty_set(motor_num, i);
			if(i == 200) {
				mtr_enc_clr(motor_num);
				printf("Encoder Cleared\n");
			}
			get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
			usleep(1000);
		}
		sleep(1);
		// printf("Motor Decelerate\n");
		// for(i = max_duty; i >= 0; i--) {
		// 	mdv_duty_set(motor_num, i);
		// 	get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
		// 	usleep(100);
		// }

		// printf("Motor Stop\n");
		// mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
		// sleep(1);

		// printf("Motor Accelerate\n");
		// for(i = 0; i >= -max_duty; i--) {
		// 	mdv_duty_set(motor_num, i);
		// 	get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
		// 	usleep(100);
		// }
		// sleep(1);
		// printf("Motor Decelerate\n");
		// for(i = -max_duty; i <= 0; i++) {
		// 	mdv_duty_set(motor_num, i);
		// 	get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
		// 	usleep(100);
		// }
		// sleep(1);
	}
	mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
	printf("Program Finish\n");
	return 0;
}
