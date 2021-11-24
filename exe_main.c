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
int motor_num = 1;
int serial_num[] = {12, 8, 15, 10, 13, 9, 14, 11, 0, 0, 0};

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

	// init_serial(serial_num[motor_num]);
	// init_mdv();
	sleep(1);

	int cnt = 0;
	for(cnt = 0; cnt < 10; cnt++) {
		printf("Count : %d\n", cnt);
		printf("Motor Stop\n");
		mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
		sleep(1);

		printf("Motor Accelerate\n");
		int i = 0;
		int max_duty = 500;
		for(i = 0; i <= max_duty; i++) {
			mdv_duty_set(motor_num, i);
			usleep(100);
		}
		sleep(1);
		printf("Motor Decelerate\n");
		for(i = max_duty; i >= 0; i--) {
			mdv_duty_set(motor_num, i);
			usleep(100);
		}

		printf("Motor Stop\n");
		mdv_mtr_stp(motor_num, MTR_STP_STAT_FRE);
		sleep(1);

		printf("Motor Accelerate\n");
		for(i = 0; i >= -max_duty; i--) {
			mdv_duty_set(motor_num, i);
			usleep(100);
		}
		sleep(1);
		printf("Motor Decelerate\n");
		for(i = -max_duty; i <= 0; i++) {
			mdv_duty_set(motor_num, i);
			usleep(100);
		}
		sleep(1);
	}
	printf("Program Finish\n");
	return 0;
}
