#ifndef __SERIAL__H
#define __SERIAL__H

const char EOL[3] = { 0xD, 0xA, 0x00 };

// interrupt
void UART_INT_init();

// serial
void UART_SER_init();
unsigned char UART_SER_read_ready();
void UART_SER_write(unsigned char c);
unsigned char UART_SER_read();
void type(char * str);

#endif //__SERIAL__H