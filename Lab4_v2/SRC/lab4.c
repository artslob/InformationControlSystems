#include "main.h"
#include "kb.h"
#include "serial.h"

#define NORMAL 0xFF
#define DEBUG 0xFE

void print_error() {
	type("Invalid arguments.");
	type(EOL);
}

void print_result(unsigned char result) {
	type(EOL);
	UART_SER_write(0x30 + result / 100);
	result = result % 100;
	UART_SER_write(0x30 + result / 10);
	result = result % 10;
	UART_SER_write(0x30 + result);
	type(EOL);
}

unsigned char mode = 0, first = 0, second = 0;

void error(){
	mode = first = second = 0;
}

void main() {
	unsigned char dip = 0, buf = 0;
	
	UART_SER_init();
	init_kb_timer();
	
	EA = 1;
	
	while (1) {
		dip = readdip();
		if (dip == NORMAL) {
			if (is_queue_empty())
				continue;
			
			buf = get_button();
			leds(buf);
			UART_SER_write(buf);
			
			switch (mode) {
				case 0:
					if (0x30 <= buf && buf <= 0x39) {
						first = buf - 0x30;
						mode = 1;
					}
					else {
						error();
						print_error();
					}
				break;
				case 1:
					if (0x30 <= buf && buf <= 0x39) {
						first = first * 10 + (buf - 0x30);
						mode = 2;
					}
					else if (buf == 'A'/*+*/) {
						mode = 3;
					}
					else {
						error();
						print_error();
					}
				break;
				case 2:
					if (buf == 'A'/*+*/)
						mode = 3;
					else {
						error();
						print_error();
					}
				break;
				case 3:
					if (0x30 <= buf && buf <= 0x39) {
						second = buf - 0x30;
						mode = 4;
					}
					else {
						error();
						print_error();
					}
				break;
				case 4:
					if (0x30 <= buf && buf <= 0x39) {
						second = second * 10 + (buf - 0x30);
						mode = 5;
					}
					else if (buf == '#'/*=*/) {
						mode = 6;
					}
					else {
						error();
						print_error();
					}
				break;
				case 5:
					if (buf == '#'/*=*/)
						mode = 6;
					else {
						error();
						print_error();
					}
				break;
			}
			if (mode == 6) {
				mode = 0;
				print_result(first + second);
			}
			
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
