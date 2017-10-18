#include "aduc812.h"
#include "serial.h"
#include "led.h"
#include "max.h"
#include "interrupt.h"
#include "stdio.h"
#include "stdlib.h"


#define SERIAL 0xFF
#define INTERRUPT 0xFE

unsigned char transform_char(unsigned char c) {
	if (0x61 <= c && c <= 0x7A) {       // en to EN
		return c - 0x20;
	}
	/*else if (0x80 <= c && c <= 0x8F) {  // Ð..ÐŸ to Ð°..Ð¿
		return c + 0x20;
	}
	else if (0x90 <= c && c <= 0x9F) {  // Ð ..Ð¯ to Ñ€..Ñ
		return c + 0x50;
	}
	else if (c == 0xF0) {               // Ð to Ñ‘
		return 0xF1;
	}*/
	else if (0x30 <= c && c <= 0x3F) {  //  ..ï  ..ï
		leds(c);
		return c + 0x70;
	}
	else if (0x40 <= c && c <= 0x4F) {  //  ..ï  ..ï
		return c + 0xA0;
	}
	else if (c == 0x01) {
		return 0xF1;
	}
	else if (c == 0x51) {
		return 0xF1;
	}
	else if (0x10 <= c && c <= 0x1F) { // €..Ÿ  ..ï
		return c + 0x90;
	}
	else if (0x20 <= c && c <= 0x2F) { // €..Ÿ  ..ï
		return c + 0xC0;
	}
	else {
		leds(c);
		return c;
	}
}

void print_code(unsigned char c) {
	char buf[10] = {0};
	unsigned char high = ( (c >> 4) & 0xF );
	unsigned char low = ( c & 0xF );
	if (high >= 0xA)
		high += 0x37;
	else high += 0x30;
	if (low >= 0xA)
		low += 0x37;
	else low += 0x30;
	buf[0] = high;
	buf[1] = low;
	buf[2] = 0xD;
	buf[3] = 0xA;
	buf[4] = 0x00;
	type(buf);
}

void main() {
	unsigned char dip = 0, c = 0, i = 0;
	
	UART_SER_init();
	UART_INT_init();
	
	while (1) {
		dip = readdip();
		if (dip == SERIAL) {
			EA = 0;
			
			if (!UART_SER_read_ready())
				continue;
			c = UART_SER_read();
			
			print_code(c);
			
			for (i = 0; i < 3; i++)
				UART_SER_write(transform_char(c));
			type(EOL);
		} 
		else  if (dip == INTERRUPT) {
			EA = 1;
		} else {
			leds(0xAA);
		}
	}
}
