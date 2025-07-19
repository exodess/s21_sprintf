#include "tools.h"

s21_size_t print_sign(char * str, long number, char flag_sign)
{
	int result = 0;
	if (number < 0) {
		*str = '-';
		result = 1;
	}
	else if (number >= 0 && flag_sign){
		*str = (flag_sign == 1) ? ' ' : '+';
		result = 1;
	}
	return result;
}

int len_number(long unsigned number, int capacity) 
{
	int len = 0;
	long n = number;
	while (n > 0) {
		n /= capacity;
		len++;
	}
	return (len == 0) ? 1 : len;
}
s21_size_t print_number(char * str, unsigned long number, const int capacity, const char symbol) 
{
	unsigned long n = number;
	s21_size_t i = 0; /* количество цифр в числе number */

	/* проверяем, является ли число нулем */
	if (n == 0) {
		str[i] = '0';
		i++;
	}
	
	/* считываем цифры числа number */
	while (n > 0) {
		/* 
			вывод цифр происходит в обратном порядке
			так как считывание происходит от самого младшего до самого старшего разрядов
		*/
		str[i] = (char) (n % capacity > 9) ? (n % capacity) + 0x37 : n % capacity + 0x30;
		if (symbol == 'x' && str[i] >= 0x41) str[i] += 0x20;

		n /= capacity;
		i++;
	}
	
	for (s21_size_t j = 0; j < (i / 2); j++) {
		/* разворачиваем число в нормальный порядок */
		char temp = str[j];
		str[j] = str[i - 1 - j];
		str[i - 1 - j] = temp;
	}

	return i ;
}
