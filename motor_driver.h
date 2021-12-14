/**
 * @file motor_driver.h
 * @author Masatoshi Motohashi
 * @brief Library of Motor Driver
 * @version 0.1
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef MOTOR_DRIVER_2021_1113_174050
#define MOTOR_DRIVER_2021_1113_174050

#define DRV_NMB 4 /** Number of Drive Motors */
#define STR_NMB 4 /** Number of Stir Motors */

#define MDV_CMD_NON 0x00	 /** Commnad Number of None */
#define MDV_CMD_ENC_CLR 0x15 /** Command Number of Clear Encoder */
#define MDV_CMD_MTR_STP 0x20 /** Command Number of Motor Stop */
#define MDV_CMD_DTY_SET 0x21 /** Command Number of Duty Set */
#define MDV_CMD_TLM_SND 0x22 /** Command Number of Getting Control Parameter */

#define MD_CMD_RETURN_DTY_SET 0xA1  /** Command Return Value of Duty Set */
#define MDV_CMD_RETURN_TLM_SND 0xA2 /** Command Return Value of Getting Control Parameter */

#define MDV_TLM_ENC_CLR_DAT_SIZ 1	/** */
#define MDV_TLM_DTY_SET_DAT_SIZ 5	/** */
#define MDV_TLM_TLM_SND_DAT_SIZ_00 3 /** */
#define MDV_TLM_TLM_SND_DAT_SIZ_01 4 /** */
#define MDV_TLM_TLM_SND_DAT_SIZ_02 4 /** */
#define MDV_TLM_TLM_SND_DAT_SIZ_03 4 /** */
#define MDV_TLM_TLM_SND_DAT_SIZ_04 6 /** */
#define MDV_TLM_TLM_SND_DAT_SIZ_05 6 /** */
#define MDV_CMD_ENC_CLR_DAT_SIZ 1	/** */
#define MDV_CMD_DTY_SET_DAT_SIZ 5	/** */
#define MDV_CMD_TLM_SND_DAT_SIZ 2	/** */
#define MDV_CMD_MTR_STP_DAT_SIZ 2	/** */

#define MDV_SCN_CMD_STT 0			 /** */
#define MDV_SCN_CMD_DTY 1000		 /** */
#define MDV_SCN_CMD_TMP 2000		 /** */
#define MDV_SCN_CMD_CRR 3000		 /** */
#define MDV_SCN_CMD_ABS_ENC_CNT 4000 /** */
#define MDV_SCN_CMD_RLT_ENC_CNT 5000 /** */
#define MDV_TLM_ENC_CLR 0x95		 /** */
#define MDV_TLM_DTY_SET 0xA1		 /** */
#define MDV_TLM_TLM_SND 0xA2		 /** */

#define MTR_STP_STAT_DIA 0x00 /** Motor Stop by Disbale */
#define MTR_STP_STAT_FRE 0x01 /** Motor Stop by Free*/
#define MTR_STP_STAT_BRK 0x02 /** Motor Stop by Break*/

#define SBUF_CMD_SIZE 255
#define SBUF_TLM_SIZE 255

extern int mdv_ch[DRV_NMB + STR_NMB];   /**  */
extern int fd_array[DRV_NMB + STR_NMB]; /** */

void mdv_duty_set(int mdv_num, int drv_dty);					 /** */
void mdv_mtr_stp(int mdv_num, char mtr_stat);					 /** */
int make_mdv_cmd_duty_set(int mdv_num, int drv_dty, char* buf);  /** */
int make_mdv_cmd_mtr_stp(int mdv_num, char mtr_stat, char* buf); /** */
int make_mdv_cmd_tlm_snd(int mdv_num, int ctrl_parm, char* buf); /** */
unsigned short serial_checksum(char* addr, int leng);			 /** */

#endif