#include "serial.h"
#include "aduc812.h"
#include "interrupt.h"
#include "led.h"


/*** interrupt ***/

unsigned char mode = 0, writing_mode = 100, ready_to_write = 0;
unsigned char first = 0, second = 0, result = 0;

void print_error(){
	EA = 0;
	type("Invalid arguments.");
	type(EOL);
	EA = 1;
}

void error(){
	mode = first = second = 0;
}


void SIO_ISR( void ) __interrupt ( 4 ) {
	unsigned char buf = 0;
	
	if(TI && ready_to_write) { // Передача байта
		if (writing_mode == 100) {
			SBUF = 0x30 + result / 100;
			result = result % 100;
			writing_mode = 10;
		}
		else if (writing_mode == 10) {
			SBUF = 0x30 + result / 10;
			result = result % 10;
			writing_mode = 1;
		}
		else if (writing_mode == 1) {
			SBUF = 0x30 + result;
			writing_mode = 100;
			ready_to_write = 0;
		}
		TI = 0;
	}
	if(RI) { // Прием байта
		buf = SBUF;
		leds(buf);
		//UART_SER_write(buf);
		RI = 0;
		
		switch (mode) {
			case 0:
				if (0x30 <= buf && buf <= 0x39) {
					first = buf - 0x30;
					mode = 1;
				}
				else if (buf != 0xD) {
					error();
					print_error();
				}
			break;
			case 1:
				if (0x30 <= buf && buf <= 0x39) {
					first = first * 10 + (buf - 0x30);
					mode = 2;
				}
				else if (buf == '+') {
					mode = 3;
				}
				else {
					error();
					print_error();
				}
			break;
			case 2:
				if (buf == '+')
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
				else if (buf == '=') {
					mode = 6;
				}
				else {
					error();
					print_error();
				}
			break;
			case 5:
				if (buf == '=')
					mode = 6;
				else {
					error();
					print_error();
				}
			break;
		}
		if (mode == 6) {
			mode = 0;
			result = first + second;
			ready_to_write = 1;
			TI = 1;
		}
	}
}

void UART_INT_init() {
	ES = 1;  // interrupt serial on
	SetVector(0x2023, (void *) SIO_ISR);
}

/*** serial ***/

void UART_SER_init() {
	TH1 = 0xFD;   // 9600 бод
	TMOD |= 0x20; // Таймер 1 будет работать в режиме autoreload
	TR1 = 1;      // start T1
	SCON = 0x50;  // REN = 1, UART mode 1
}

unsigned char UART_SER_read_ready() {
	return RI;
}

void UART_SER_write(unsigned char c) {
	SBUF = c;
	TI = 0;
	while(!TI);
}

unsigned char UART_SER_read() {
	while(!RI);
	RI = 0;
	return SBUF;
}

void type(char* str) {
	while(*str)
		UART_SER_write(*str++);
}
