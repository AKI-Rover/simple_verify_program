/**
 * @file serial.h
 * @author Masatoshi Motohashi
 * @brief Library of Serial
 * @version 0.1
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef SERIAL_2021_1113_184418
#define SERIAL_2021_1113_184418

#include <termios.h>

#define SERIAL_PORT_0 "/dev/ttyS0"		 /** RS232C, for Resolv of pan */
#define SERIAL_PORT_1 "/dev/ttyS1"		 /** RS232C, for Resolv of tilt */
#define SERIAL_PORT_2 "/dev/ttyS2"		 /** N.A. */
#define SERIAL_PORT_3 "/dev/ttyS3"		 /** N.A. */
#define SERIAL_PORT_4 "/dev/ttyS4"		 /** Force torque sensor #1 */
#define SERIAL_PORT_5 "/dev/ttyS5"		 /** Force torque sensor #2 */
#define SERIAL_PORT_6 "/dev/ttyS6"		 /** Force torque sensor #3 */
#define SERIAL_PORT_7 "/dev/ttyS7"		 /** Force torque sensor #4 */
#define SERIAL_PORT_USB0 "/dev/ttyMC1"   /** Motor Driver(Front/Right/Drive) */
#define SERIAL_PORT_USB1 "/dev/ttyMC2"   /** Motor Driver() */
#define SERIAL_PORT_USB2 "/dev/ttyMC3"   /** Motor Driver(Rear/Right/Drive) */
#define SERIAL_PORT_USB3 "/dev/ttyMC4"   /** Motor Driver() */
#define SERIAL_PORT_USB4 "/dev/ttyMC5"   /** Motor Driver(Front/Left/Drive) */
#define SERIAL_PORT_USB5 "/dev/ttyMC6"   /** Motor Driver() */
#define SERIAL_PORT_USB6 "/dev/ttyMC7"   /** Motor Driver() */
#define SERIAL_PORT_USB7 "/dev/ttyMC8"   /** Motor Driver(Rear/Left/Drive) */
#define SERIAL_PORT_USB8 "/dev/ttyMC9"   /** Motor Driver() */
#define SERIAL_PORT_USB9 "/dev/ttyMC10"  /** Motor Driver() */
#define SERIAL_PORT_USB10 "/dev/ttyMC11" /** Motor Driver() */
#define SERIAL_PORT_USB11 "/dev/ttyMC12" /** Motor Driver() */
#define SERIAL_BAUDRATE_MDV B115200		 /** Baudrate of Motor Driver */
#define SERIAL_BAUDRATE_RU B38400		 /** */
#define SERIAL_BAUDRATE_NMN B115200		 /** */

extern int serial_fd[20];					  /** File Descriptor for Serial Port */
extern struct termios oldtio[20], newtio[20]; /** */

int init_motor_serial(int mdv_num); /** Initialize Serial Port for Motor Driver */
int init_serial(int srl_num);		/** Initialize Serial Port */
int open_motor_serial(int mdv_num); /** Open Serial Port for Motor Driver */

#endif