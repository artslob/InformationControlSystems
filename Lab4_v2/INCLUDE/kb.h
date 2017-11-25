#ifndef __KB__H
#define __KB__H

#include "serial.h"
#include "max.h"
#include "led.h"

#define MAX_KEYS_PRESSED 2
#define QUEUE_LENGTH 10

unsigned char column = 3;
unsigned char bounce[4][4] = {{0}};
unsigned short time[4][4] = {{0}};
unsigned char queue[QUEUE_LENGTH] = {0};
char start_queue = 0, end_queue = 0;
unsigned char keyboard[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

unsigned char is_queue_empty(){
	return start_queue == end_queue;
}

void capture_button(unsigned char c) {
	if (start_queue == QUEUE_LENGTH)
		start_queue = 0;
	queue[start_queue++] = c;
}

unsigned char get_button() {
	if (start_queue == end_queue)
		return 0x3F;
	if (end_queue == QUEUE_LENGTH)
		end_queue = 0;
	return queue[end_queue++];
}

void invalid_input(){
	unsigned char i, j;
	EA = 0;
	type("fantom mistake\n");
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++){
			bounce[i][j] = 0;
			time[i][j] = 0;
		}
	}
	EA = 1;
}

unsigned char scan_row() {
	unsigned char col, row;
	
	if (column == 3)
		column = 0;
	else column++;
	
	col = 0x1 << column; //0001,0010,0100,1000,0001,...
	write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...
	
	row = read_max(KB) & (0xF0);
	row = (~(row >> 4)) & 0x0F;
	return row;
	//return (~(read_max(KB) & 0xF0)) >> 4;
}

void timer_kb(void) __interrupt( 1 ) {
	unsigned char row, scanned_row, key_pressed = 0;
	unsigned char i, j;
	
	scanned_row = scan_row();
	key_pressed = 0;
	for (row = 0; row < 4; row++) {
		//leds(scanned_row);
		if (scanned_row & (0x01 << row)) {
			if (bounce[row][column] < 3)
				bounce[row][column]++;
			else
				time[row][column]++;
		}
		else {
			if (bounce[row][column] > 0)
				bounce[row][column] = 0;
			else if (time[row][column] > 0)
				time[row][column] = 0;
		}
		
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				if(bounce[i][j] == 3)
					key_pressed++;
			}
		}
		
		leds(key_pressed);
		if (key_pressed > MAX_KEYS_PRESSED)
			invalid_input();
		else {
			key_pressed = 0;
			if (bounce[row][column] >= 3 && time[row][column] == 1) {
				capture_button(keyboard[row][column]);
			}
			else if (time[row][column] >= 15) {
				capture_button(keyboard[row][column]);
				time[row][column] = 2;
			}
		}
	}
	TH0 = 0xED;    // T0 200Hz
	TL0 = 0xBB;
}

void init_kb_timer(){
	SetVector(0x200B, (void*) timer_kb); // T0 int prog
	TH0 = 0xED;    // T0 200Hz
	TL0 = 0xBB;
	TMOD |= 0x01;  // T0 16 bit
	ET0 = 1;       // T0 int
	TR0 = 1;       // T0 run
}

#endif //__KB__H
