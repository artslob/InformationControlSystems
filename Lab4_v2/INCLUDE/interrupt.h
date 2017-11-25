#ifndef __INTERRUPT__H
#define __INTERRUPT__H

//////////////////////// SetVector //////////////////////////
// Функция, устанавливающая вектор прерывания в
// пользовательской таблице прерываний.
// Вход:
// Vector – адрес обработчика прерывания,
// Address – вектор пользовательской таблицы прерываний.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////
void SetVector(unsigned char xdata * Address, void * Vector) {
	unsigned char xdata * TmpVector; // Временная переменная
	// Первым байтом по указанному адресу записывается код ljmp, равный 02h
	*Address = 0x02;
	// Далее записывается адрес перехода Vector
	TmpVector = (unsigned char xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;
	// Таким образом, по адресу Address теперь
	// располагается инструкция ljmp Vector
}

#endif //__INTERRUPT__H