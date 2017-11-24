#ifndef __SERIAL__H
#define __SERIAL__H

const char EOL[3] = { 0xD, 0xA, 0x00 };

// interrupt
void UART_INT_init();

unsigned char is_wfifo_full();
void wfifo_capture_symbol(unsigned char symbol);
unsigned char wfifo_get_symbol();

unsigned char is_rfifo_empty();
void rfifo_capture_symbol(unsigned char symbol);
unsigned char rfifo_get_symbol();

// serial
void UART_SER_init();
unsigned char UART_SER_read_ready();
void UART_SER_write(unsigned char c);
unsigned char UART_SER_read();
void type(char * str);

#endif //__SERIAL__H