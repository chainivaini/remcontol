#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <rt_sys.h>
#include "math.h"


#ifndef __U_HEXTO
  #define __U_HEXTO
	
	typedef volatile struct{
		int32_t Byte0;
		int16_t Byte1;
		int8_t dataS[4];
		int32_t dataL[4];
		char hexbytes[4];
	}invers;
	
char* dec2bin(int16_t num);
int32_t hexplus(int32_t rpm);
int32_t hexmin(int32_t rpm);
int32_t U_data_inverse(int32_t data);
int32_t U_RXdata_inverse(int32_t dataL);
char digit(int num);
int16_t U_TEMPdata_inverse(int32_t data);
int RXtoRPM(int32_t num);

#endif

	
	