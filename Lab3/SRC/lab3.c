#include "aduc812.h"
#include "led.h"
#include "max.h"
#include "interrupt.h"
#include "timer.h"


#define ANIMATION 0
#define LOCKING 1

#define OFF 0
#define ON 1

unsigned char display_mode = ANIMATION;
unsigned char lock_count = 1;

void INT0_PROG( void ) __interrupt ( 0 ) { //смена режима отображения
	display_mode = (display_mode == LOCKING) ? ANIMATION : LOCKING;
}

void T0_PROG( void ) __interrupt ( 1 ) { //T0 счётчик замыканий
	TH0 = 0xFF;
	TL0 = 0xFF;
	lock_count++;
}

#define PERIOD 10 //period of animation in ms
const long PERIODS[] = {1, 2, 4, 6, 8, 10};

void animation() {
	static unsigned long last_shift_time = GetMsCounter();
	static unsigned char shift = 0;
	unsigned long start = GetMsCounter();
	int i = 0;
	
	if (DTimeMs(last_shift_time) >= 1000) {
		last_shift_time = GetMsCounter();
		shift++;
	}
	
	while(1) {
		led((1 + shift) % 8, OFF);
		led((2 + shift) % 8, OFF);
		for (i = 0; i < 6; i++){
			if (PERIODS[i] >= DTimeMs(start)){
				led((3 + i + shift) % 8, ON);
			} else {
				led((3 + i + shift) % 8, OFF);
			}
		}
		if (DTimeMs(start) > PERIOD)
			break;
	}
}

void main(void) {
	IT0 = 1; // по перепаду INT0 с 1 на 0
	write_max(ENA, 0x20);
	SetVector(0x2003, (void *) INT0_PROG);
	EX0 = 1; //INT0 interrupt enable
	
	TMOD = 0x05; // T0=16 bit, C/T0
	TH0 = 0xFF;
	TL0 = 0xFF;
	SetVector(0x200B, (void *) T0_PROG);
	TR0 = 1;
	ET0 = 1; //T0 interrupt enable
	
	InitTimer(); //init T1
	
	EA = 1;
	leds(0x05);
	
	display_mode = ANIMATION;
	while (1) {
		if (display_mode == ANIMATION) {
			animation();
		} else {
			leds(lock_count);
		}
		//leds(lock_count + 1);
		//animation();
	}
}
