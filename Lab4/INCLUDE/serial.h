#ifndef __SERIAL__H
#define __SERIAL__H

const char EOL[3] = { 0xD, 0xA, 0x00 };

// declarations
// interrupt
void UART_INT_init();
// serial
void UART_SER_init();
unsigned char UART_SER_read_ready();
void UART_SER_write(unsigned char c);
unsigned char UART_SER_read();
void type(char * str);



/*** interrupt ***/
/*
void print_error(){
	EA = 0;
	type("Invalid arguments.");
	type(EOL);
	EA = 1;
}

unsigned char WFIFO = 0;
unsigned char iw = 0;
unsigned char ready_to_write = 0;

unsigned char RFIFO[5] = {0};
unsigned char ir = 0;

void SIO_ISR( void ) __interrupt ( 4 ) {
	unsigned char buf = 0;
	char j = 0;
	
	if(TI && ready_to_write) { // Передача байта
		if (iw < 8) {
			if (( WFIFO >> (7 - iw)) & 0x1)
				SBUF = 0x31;
			else SBUF = 0x30;
			iw++;
		}
		else if (iw == 8) {
			WFIFO = 0;
			SBUF = 0xA;
			ready_to_write = 0;
			iw = 0;
		}
		TI = 0;
	}
	if(RI) { // Прием байта
		buf = SBUF;
		leds(buf);
		UART_SER_write(buf);
		RI = 0;
		if ((0x30 <= buf && buf <= 0x39) || buf == 0xD) {
			RFIFO[ir++] = buf;
			if (ir == 4 && (RFIFO[ir - 1] != 0xD)) {
				ir = 0;
				print_error();
				return;
			}
			else if (ir >= 2 && RFIFO[ir - 1] == 0xD) {
				for (j = 0; j < ir - 1; j++) {
					if (RFIFO[j] < 0x30 || RFIFO[j] > 0x39) {
						ir = 0;
						WFIFO = 0;
						print_error();
						return;
					}
					WFIFO = WFIFO * 10 + (RFIFO[j] - 0x30);
				}
				type(EOL);
				ir = 0;
				ready_to_write = 1;
				leds(WFIFO);
				TI = 1;
			}
			else if (ir == 1 && RFIFO[ir - 1] == 0xD) {
				ir = 0;
				print_error();
				return;
			}
		}
		else {
			ir = 0;
			print_error();
		}
	}
}

void UART_INT_init() {
	ES = 1;  // interrupt serial on
	SetVector(0x2023, (void *) SIO_ISR);
}
*/
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