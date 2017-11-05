#include "main.h"
#include "kb.h"

#define NORMAL 0xFF
#define DEBUG 0xFE


void main() {
	unsigned char dip = 0;
	
	UART_SER_init();
	//UART_INT_init();
	init_kb_timer();
	
	EA = 1;
	
	while (1) {
		dip = readdip();
		if (dip == NORMAL) {
			
		}
		else if (dip == DEBUG) {
			
		}
		else {
			leds(0xAA);
		}
	}
}
