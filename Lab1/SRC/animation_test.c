#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

unsigned char animation() {
	static unsigned char two = 0x03;
	static unsigned char one = 0x80;
	static unsigned char answer = 0x00;
	answer = 0x00 | one | two;
	one = one >> 1 | one << 7;
	two = two << 1 | two >> 7;
	return answer;
}

void main(void) {
	unsigned char res = 0;
	while(1)
	{
		res = animation();
		printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(res));
		sleep(1);
	}
}