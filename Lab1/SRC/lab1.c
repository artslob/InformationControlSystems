#include "aduc812.h"
#include "led.h"
#include "max.h"


// Задержка на заданное количество мс
void delay (unsigned long ms)
{
	volatile unsigned long i, j;

    for( j = 0; j < ms; j++ )
        for( i = 0; i < 50; i++ );
}

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
	unsigned char const KEY = 0xEE;
	unsigned char dip = 0;
	while(1) {
		dip = readdip();
		if (dip == KEY) {
			leds(animation());
		} else {
			leds(dip);
		}
		delay(400);
	}
}
