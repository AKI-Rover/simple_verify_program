CC = arm-linux-gnueabi-gcc
# CC = gcc
CFLAGS = -Wall -Wextra -O2 -march=armv5te
LDFLAGS =
INCLUDES = -lpthread -lm -lrt
DEFINES  =
OPTIMIZE =
CFLAGS   = $(DEFINES)  $(OPTIMIZE)

OBJS = exe_main

$(OBJS) : exe_main.o motor_driver.o serial.o com_prot.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(INCLUDES)
	rm -f *.o
	
motor_driver.o: motor_driver.c
	$(CC) -c $^ $(LDFLAGS) $(CFLAGS) $(INCLUDES)

serial.o: serial.c
	$(CC) -c $^ $(LDFLAGS) $(CFLAGS) $(INCLUDES)

com_prot.o: com_prot.c
	$(CC) -c $^ $(LDFLAGS) $(CFLAGS) $(INCLUDES)

exe_main.o: exe_main.c
	$(CC) -c $^ $(LDFLAGS) $(CFLAGS) $(INCLUDES)

clean:
	rm -f *.o
	rm -f $(OBJS)

exe_main : exe_main.o
exe_main.o : exe_main.c
