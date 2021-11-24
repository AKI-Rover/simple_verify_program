/**
 * @file com_prot.c
 * @author Masatoshi Motohashi
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "com_prot.h"

unsigned char ustoc1(unsigned short usd);

void itoc4(int id, unsigned char* cd) {
	int i;
	union {
		int f;
		struct {
			unsigned char c[4];
		} i;
	} j;

	j.f = id;
	for(i = 0; i < 4; i++)
		*(cd + i) = j.i.c[i];
}

unsigned char ustoc1(unsigned short usd) {		  //little endian version
	void* p;
	p = &usd;
	return (*((unsigned char*)p + 0));
}