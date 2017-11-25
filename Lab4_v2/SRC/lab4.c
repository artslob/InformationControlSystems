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
	init_kb_timer();
	
	EA = 1;
	
	while (1) {
		dip = readdip();
		if (dip == NORMAL) {
			if (is_queue_empty())
				continue;
			
			
		}
		else if (dip == DEBUG) {
			if (!is_queue_empty()) {
				UART_SER_write(get_button());
			}
		}
		else {
			leds(0xAA);
		}
	}
}
