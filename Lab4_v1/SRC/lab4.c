#include "main.h"
#include "kb.h"
#include "serial.h"

#define NORMAL 0xFF
#define DEBUG 0xFE

unsigned char WFIFO = 0;

unsigned char RFIFO[5] = {0};
unsigned char ir = 0;

void print_error(){
	EA = 0;
	type(EOL);
	type("Invalid arguments.");
	type(EOL);
	EA = 1;
}

void print_result() {
	unsigned char i = 0;
	
	for (i = 0; i < 8; i++){
		if (( WFIFO >> (7 - i)) & 0x1)
			UART_SER_write('1');
		else UART_SER_write('0');
	}
	WFIFO = 0;
	type(EOL);
}

void main() {
	unsigned char dip = 0, button = 0, j = 0;
	
	UART_SER_init();
	//UART_INT_init();
	init_kb_timer();
	
	EA = 1;
	
	while (1) {
		dip = readdip();
		if (dip == NORMAL) {
			if (!is_queue_empty()) {
				ET0 = 0;
				
				button = get_button();
				UART_SER_write(button);
				leds(button);
				
				if ('0' <= button <= '9' || button == '*') {
					RFIFO[ir++] = button;
					if ( (ir == 4 && RFIFO[ir - 1] != '*') || (ir == 1 && RFIFO[ir - 1] == '*') ) {
						ir = 0;
						print_error();
						ET0 = 1;
						continue;
					}
					else if (ir >= 2 && RFIFO[ir - 1] == '*') {
						for (j = 0; j < ir - 1; j++) {
							if (RFIFO[j] < 0x30 || RFIFO[j] > 0x39) {
								ir = 0;
								WFIFO = 0;
								print_error();
								ET0 = 1;
								continue;
							}
							WFIFO = WFIFO * 10 + (RFIFO[j] - 0x30);
						}
						type(EOL);
						ir = 0;
						leds(WFIFO);
						print_result();
					}
				}
				else {
					ir = 0;
					type(EOL);
					print_error();
					ET0 = 1;
				}
				
				ET0 = 1;
			}
		}
		else if (dip == DEBUG) {
			if (!is_queue_empty()) {
				ET0 = 0;
				UART_SER_write(get_button());
				ET0 = 1;
			}
		}
		else {
			leds(0xAA);
		}
	}
}
