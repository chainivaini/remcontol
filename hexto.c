#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "hexto.h"
char hexmsg [126];
extern volatile invers U_inv;

//
char* dec2bin(int16_t num)
{
    int bin =0, k = 1;
    while (num)
    {
        bin += (num % 2)*k;
        k *= 10;
        num /= 2;
    }
		 bin=bin*0.0001;
		
		switch(bin){
			case 0:
				return "P-0,I-0,S-0,F-0";
			case 1:
				return "P-0,I-0,S-0,F-1";
			case 10:
				return "P-0,I-0,S-1,F-0";
			case 11:
				return "P-0,I-0,S-1,F-1";
			case 100:
				return "P-0,I-1,S-0,F-0";
			case 101:
				return "P-0,I-1,S-0,F-1";
			case 110:
				return "P-0,I-1,S-1,F-0";
			case 111:
				return "P-0,I-1,S-1,F-1";
			case 1000:
				return "P-1,I-0,S-0,F-0";
			case 1001:
				return "P-1,I-0,S-0,F-1";
			case 1010:
				return "P-1,I-0,S-1,F-0";
			case 1011:
				return "P-1,I-0,S-1,F-1";
			case 1100:
				return "P-1,I-1,S-0,F-0";
			case 1101:
				return "P-1,I-1,S-0,F-1";
			case 1110:
				return "P-1,I-1,S-1,F-0";
			case 1111:
				return "P-1,I-1,S-1,F-1";
			default:
				return "ERROR";
		}
	 
}


// 
int RXtoRPM(int32_t num)
{
  int k = 1,count_z=0,cneg=0;
	float  i=0;
	if(num < 0){num = num - 1;num=~num;cneg++;}
	while (num)
	{ 
					if((count_z <= 24) & ((num%2) == 1)){ i+= 1./(pow(2,(24-count_z)));}
					count_z ++;
					k *= 10;
					num /= 2;
	}
	if(cneg > 0)return -1000*i;
	return i*1000;
}



	
int32_t U_data_inverse(int32_t data)
{
	volatile invers U_inv;
	int16_t revs = 0x00FF;
	revs &= data; 
	revs = revs << 8;
	U_inv.Byte0 = data >> 8;
	U_inv.Byte0 &= 0x00FF;
	revs = revs | U_inv.Byte0 ;
	return revs;
}

 int32_t U_RXdata_inverse(int32_t data)
{
	volatile invers U_inv;
	
	int32_t revs = 0x00000000;
	U_inv.dataS[0] = data;
	U_inv.dataL[0] =(U_inv.dataS[0])<<24;
	U_inv.dataL[0] &=0xFF000000;

		
	U_inv.dataS[1] = data>>8;
	U_inv.dataL[1] =(U_inv.dataS[1])<<16;
	U_inv.dataL[1] &=0x00FF0000;
	
		
	U_inv.dataS[2] = data>>16;
	U_inv.dataL[2] =(U_inv.dataS[2])<<8;
	U_inv.dataL[2] &=0x0000FF00;
	
		
	U_inv.dataS[3] = data>>24;
	U_inv.dataL[3] =(U_inv.dataS[3]);
	U_inv.dataL[3] &=0x000000FF;
	revs =((U_inv.dataL[0])|(U_inv.dataL[1])|(U_inv.dataL[2])|(U_inv.dataL[3]));
	return revs ;

}

int16_t U_TEMPdata_inverse(int32_t data)
{
	int16_t revs = 0;
	int8_t itog = data >>16;
	int8_t itog2 = data >> 24;
	revs = itog <<8;
	revs &=0xFF00;
	revs = revs | itog2;
	return revs;
}


