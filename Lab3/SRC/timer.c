#include "interrupt.h"
#include "aduc812.h"

unsigned long ms = 0;

void T1_PROG( void ) __interrupt ( 3 ) { //таймер 1, считающий милисекунды
	TL1 = 0x65;
	TH1 = 0xFC;
	ms++;
}

//Инициализация таймера.
void InitTimer(void){
	TMOD |= 0x10; //T1 16 bit
	TL1 = 0x65;
	TH1 = 0xFC;
	SetVector(0x201B, (void *) T1_PROG);
	TR1 = 1; //run T1
	ET1 = 1; //T1 interrupt enable
}

//Получение текущей метки времени в миллисекундах.
unsigned long GetMsCounter(void){
	unsigned long result;
	ET1 = 0;
	result = ms;
	ET1 = 1;
	return result;
}

//Измерение количества миллисекунд, прошедших с временной метки t0 и до текущего времени.
unsigned long DTimeMs(unsigned long t0) {
	return GetMsCounter() - t0;
}

//Задержка на заданное количество мс
void delay (unsigned long t) {
	unsigned long start = GetMsCounter();
	while (DTimeMs(start) < t);
}