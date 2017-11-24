#include "serial.h"
#include "aduc812.h"
#include "interrupt.h"
#include "led.h"


/*** interrupt ***/

#define QUEUE_LENGTH 10

/* WFIFO */
unsigned char WFIFO[QUEUE_LENGTH] = {0};
unsigned char Wfifo_start_queue = 0, Wfifo_end_queue = 0;

unsigned char is_wfifo_full(){
	unsigned char next_end_position = Wfifo_end_queue;
	if (next_end_position == QUEUE_LENGTH)
		next_end_position = 0;
	return next_end_position == Wfifo_start_queue;
}

void wfifo_capture_symbol(unsigned char symbol) {
	if (Wfifo_start_queue == QUEUE_LENGTH)
		Wfifo_start_queue = 0;
	WFIFO[Wfifo_start_queue++] = symbol;
}

unsigned char wfifo_get_symbol() {
	if (Wfifo_start_queue == Wfifo_end_queue)
		return 0x00;
	if (Wfifo_end_queue == QUEUE_LENGTH)
		Wfifo_end_queue = 0;
	return WFIFO[Wfifo_end_queue++];
}

/* RFIFO */
unsigned char RFIFO[QUEUE_LENGTH] = {0};
char Rfifo_start_queue = 0, Rfifo_end_queue = 0;

unsigned char is_rfifo_empty(){
	return Rfifo_end_queue == Rfifo_start_queue;
}

void rfifo_capture_symbol(unsigned char symbol) {
	if (Rfifo_start_queue == QUEUE_LENGTH)
		Rfifo_start_queue = 0;
	RFIFO[Rfifo_start_queue++] = symbol;
}

unsigned char rfifo_get_symbol() {
	if (Rfifo_start_queue == Rfifo_end_queue)
		return 0x00;
	if (Rfifo_end_queue == QUEUE_LENGTH)
		Rfifo_end_queue = 0;
	return RFIFO[Rfifo_end_queue++];
}

void SIO_ISR( void ) __interrupt ( 4 ) {
	unsigned char buf = 0;
	
	if(TI) { // Передача байта
		SBUF = wfifo_get_symbol();
		TI = 0;
	}
	if(RI) { // Прием байта
		rfifo_capture_symbol(SBUF);
		RI = 0;
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
