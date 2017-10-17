#include "aduc812.h"
#include "serial.h"
#include "led.h"
#include "max.h"
#include "interrupt.h"


#define SERIAL 0xFF
#define INTERRUPT 0xFE

unsigned char transform_char(unsigned char c) {
	if (0x61 <= c && c <= 0x7A)       // en to EN
		return c - 0x20;
	else if (0x80 <= c && c <= 0x8F)  // А..П to а..п
		return c + 0x20;
	else if (0x90 <= c && c <= 0x9F)  // Р..Я to р..я
		return c + 0x50;
	else if (c == 0xF0)               // Ё to ё
		return 0xF1;
	else return c;
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
