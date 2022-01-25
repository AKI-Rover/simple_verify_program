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
int motor_num = 0;

/**
 * @brief Ctrl-C Handler
 *
 */
void ctrl_c_handler() {
	printf("\n\nCtrl_C\n");
	mdv_mtr_stp(0, MTR_STP_STAT_FRE);
	mdv_mtr_stp(1, MTR_STP_STAT_FRE);
	mdv_mtr_stp(2, MTR_STP_STAT_FRE);
	mdv_mtr_stp(3, MTR_STP_STAT_FRE);
	exit(1);
}

int main(int argc, char* argv[]) {
	int i = 0;
	int max_duty = 200;
	long long enc_val_lf = 0;
	long long enc_val_rf = 0;
	long long enc_val_lr = 0;
	long long enc_val_rr = 0;
	FILE* fd;

	printf("Initializing\n");
	signal(SIGINT, ctrl_c_handler);		   //Register Ctrl-C Handler

	init_motor_serial(0);
	init_motor_serial(1);
	init_motor_serial(2);
	init_motor_serial(3);
	mtr_enc_clr(0);
	mtr_enc_clr(1);
	mtr_enc_clr(2);
	mtr_enc_clr(3);
	sleep(1);

	printf("Motor Stop\n");
	mdv_mtr_stp(0, MTR_STP_STAT_FRE);
	mdv_mtr_stp(1, MTR_STP_STAT_FRE);
	mdv_mtr_stp(2, MTR_STP_STAT_FRE);
	mdv_mtr_stp(3, MTR_STP_STAT_FRE);
	fd = fopen("enc_log.txt", "a");
	for(i = 0; i <= 10; i++) {
		enc_val_lf = get_mtr_ctrl_param(0, 0x04);		 // Encoder Absolute Value
		enc_val_rf = get_mtr_ctrl_param(1, 0x04);
		enc_val_lr = get_mtr_ctrl_param(2, 0x04);
		enc_val_rr = get_mtr_ctrl_param(3, 0x04);
		fprintf(fd, "%d\t%lld\t%lld\t%lld\t%lld\n", 0, enc_val_lf, enc_val_rf, enc_val_lr, enc_val_rr);
		// printf("%lld\n", enc_val);
		usleep(100);
	}
	fclose(fd);
	// get_mtr_ctrl_param(motor_num, 0x04);		// Encoder Absolute Value
	sleep(1);

	printf("Motor Accelerate\n");
	fd = fopen("enc_log.txt", "a");
	for(i = 0; i <= max_duty; i += 5) {
		mdv_duty_set(0, i);
		mdv_duty_set(1, -i);
		mdv_duty_set(2, i);
		mdv_duty_set(3, -i);
		enc_val_lf = get_mtr_ctrl_param(0, 0x04);		 // Encoder Absolute Value
		enc_val_rf = get_mtr_ctrl_param(1, 0x04);
		enc_val_lr = get_mtr_ctrl_param(2, 0x04);
		enc_val_rr = get_mtr_ctrl_param(3, 0x04);
		fprintf(fd, "%d\t%lld\t%lld\t%lld\t%lld\n", i, enc_val_lf, enc_val_rf, enc_val_lr, enc_val_rr);
		// printf("%lld\n", enc_val);
		usleep(1000);
	}
	for(i = 0; i <= 150; i++) {
		mdv_duty_set(0, max_duty);
		mdv_duty_set(1, -max_duty);
		mdv_duty_set(2, max_duty);
		mdv_duty_set(3, -max_duty);
		enc_val_lf = get_mtr_ctrl_param(0, 0x04);		 // Encoder Absolute Value
		enc_val_rf = get_mtr_ctrl_param(1, 0x04);
		enc_val_lr = get_mtr_ctrl_param(2, 0x04);
		enc_val_rr = get_mtr_ctrl_param(3, 0x04);
		fprintf(fd, "%d\t%lld\t%lld\t%lld\t%lld\n", max_duty, enc_val_lf, enc_val_rf, enc_val_lr, enc_val_rr);
		// printf("%lld\n", enc_val);
		usleep(100);
	}
	fclose(fd);
	// sleep(1);
	mdv_mtr_stp(0, MTR_STP_STAT_FRE);
	mdv_mtr_stp(1, MTR_STP_STAT_FRE);
	mdv_mtr_stp(2, MTR_STP_STAT_FRE);
	mdv_mtr_stp(3, MTR_STP_STAT_FRE);
	printf("Program Finish\n");
	return 0;
}
