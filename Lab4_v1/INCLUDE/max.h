#ifndef __MAX__H
#define __MAX__H

#include "aduc812.h"

//Названия регистров
#define KB          0x0
#define DATA_IND    0x1
#define EXT_LO      0x2
#define EXT_HI      0x3
#define ENA         0x4
#define C_IND       0x6
#define SV          0x7


#define MAXBASE 0x8 //Номер страницы внешней памяти (xdata), куда отображаются
                    //регистры расширителя.

					
void write_max( unsigned char xdata *regnum, unsigned char val ) {
	unsigned char oldDPP = DPP;
    DPP     = MAXBASE;
    *regnum = val;
    DPP     = oldDPP;
}

unsigned char read_max( unsigned char xdata *regnum ) {
	unsigned char oldDPP=DPP;
	unsigned char val;

    DPP = MAXBASE;
    val = *regnum;
    DPP = oldDPP;

    return val;
}

unsigned char readdip() {
	write_max(ENA, 0x00);
	return read_max(EXT_LO);
}

#endif //__MAX__H
