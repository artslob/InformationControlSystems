#ifndef __LED__H
#define __LED__H

#include "max.h"

static unsigned char old_led = 0;   // "Видеопамять" линейки светодиодов


void led( unsigned char n, unsigned char on )
{
	unsigned char c;
	unsigned char mask = 1;

    if( n > 7 ) return;

    c = old_led;

    mask <<= n;

    if( on )
        c |= mask;
    else
        c &= ~mask;         

    write_max( SV, c );     

    old_led = c;
}

void leds( unsigned char on ) {
    write_max( SV, on );     
    old_led = on;
}

#endif //__LED__H
