#include "aduc812.h"
#include "serial.h"
#include "led.h"
#include "max.h"
#include "interrupt.h"
#include "stdio.h"
#include "stdlib.h"


#define SERIAL 0xFF
#define INTERRUPT 0xFE

unsigned char is_russian(unsigned char c){
	return (0x30 <= c && c <= 0x3F) || (0x40 <= c && c <= 0x4F) ||
		   (0x10 <= c && c <= 0x1F) || (0x20 <= c && c <= 0x2F) ||
		   (c == 0x01) || (c == 0x51);
}

unsigned char has_next_with_offset(unsigned char c, unsigned char offset){
	unsigned char next_char = c + offset;
	return (0x30 <= next_char && next_char <= 0x4F) ||
		   (0x10 <= next_char && next_char <= 0x2F) ||
		   ((c == 0x01) || (c == 0x51));
}

unsigned char to_lowercase_with_offset(unsigned char c, unsigned char offset){
	unsigned char next_char = c + offset;
	if ((c == 0x01) || (c == 0x51)) {
		return 0xA5 + offset;
	}
	else if (0x30 <= next_char && next_char <= 0x3F) {  //  ..¯  ..¯
		return next_char + 0x70;
	}
	else if (0x40 <= next_char && next_char <= 0x4F) {  // ¯..ï ¯..ï
		return next_char + 0xA0;
	}
	else if (0x10 <= next_char && next_char <= 0x1F) { // €..  ..¯
		return next_char + 0x90;
	}
	else if (0x20 <= next_char && next_char <= 0x2F) { // ..Ÿ ¯..ï
		return next_char + 0xC0;
	}
	else {
		leds(c);
		return c;
	}
}

unsigned char to_uppercase(unsigned char c) {
	if (0x30 <= c && c <= 0x4F) { //  ..ï €..Ÿ
		return c + 0x50;
	}
	else if ((c == 0x01) || (c == 0x51)) { // ñð   ð
		return 0xF0;
	}
	else if (0x10 <= c && c <= 0x2F) { // €..Ÿ €..Ÿ
		return c + 0x70;
	}
	else {
		leds(c);
		return c;
	}
}

unsigned char transform_char(unsigned char c) {
	if (0x61 <= c && c <= 0x7A) {       // en to EN
		return c - 0x20;
	}
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

void write_symbol(unsigned char symbol) {
	while (is_wfifo_full());
	wfifo_capture_symbol(symbol);
	TI = 1;
}

void write_str(char* str) {
	while(*str)
		write_symbol(*str++);
}

void print_error(){
	write_str("Invalid arguments.");
	write_str(EOL);
}

unsigned char mode = 0, writing_mode = 100, ready_to_write = 0;
unsigned char first = 0, second = 0;

void error(){
	mode = first = second = 0;
}

void print_result(unsigned char result) {
	write_symbol(0x30 + result / 100);
	result = result % 100;
	write_symbol(0x30 + result / 10);
	result = result % 10;
	write_symbol(0x30 + result);
}

void handle_interrupt_mode() {
	unsigned char buf = 0;
	
	if (is_rfifo_empty())
		return;
	
	buf = rfifo_get_symbol();
	leds(buf);
	write_symbol(buf);
	
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
		print_result(first + second);
	}
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
			
			if (!is_russian(c))
				continue;
			
			UART_SER_write(to_uppercase(c));
			
			for (i = 0; i < 5; i++) {
				if (!has_next_with_offset(c, i + 1))
					break;
				UART_SER_write(to_lowercase_with_offset(c, i + 1));
			}
			
			type(EOL);
		} 
		else  if (dip == INTERRUPT) {
			EA = 1;
			
			handle_interrupt_mode();
		} else {
			leds(0xAA);
		}
	}
}
