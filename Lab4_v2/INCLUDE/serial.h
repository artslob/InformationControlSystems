#ifndef __SERIAL__H
#define __SERIAL__H

const char EOL[3] = { 0xD, 0xA, 0x00 };

// declarations
// serial
void UART_SER_init();
unsigned char UART_SER_read_ready();
void UART_SER_write(unsigned char c);
unsigned char UART_SER_read();
void type(char * str);


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

#endif //__SERIAL__H