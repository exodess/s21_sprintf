#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include "s21_string.h"

#define MOD(a) ((a >= 0) ? a : -a)  /* модуль числа */
#define DEFAULT_VALUE (-2)
#define EXPECTED_VALUE (-1) /* показывает, что программа ожидает значение поля (для переменной точности '*') */
#define NONE 0 /* значение не получено */
#define EPS 0.000001

#define FLAGS "-+ #0" /* список флагов для функции s21_printf */
#define SPECIFIERS "cdieEfgGosuxXpn%"
#define WIDTH "0123456789*"
#define LENGTH "hlL"

typedef struct {

	int type;		/* тип переменной
						хранит символ спецификатора */
						
	char align;     /* выравнивание
						0 : по левому краю
						1 : по правому краю */
						
	char sign;      /* знак числа и как его выводить
						0 : выводим только '-'
						1 : выводим '-', вместо '+' выводим [SPACE]
						2 : выводим и '-', и '+' */
						
	char fill;      /* заполнение 
						'0' : заполнение нулями
						[SPACE] : заполнение пробелами */
						
	char prefix;    /* будет ли выведен префикс числа
						0 - не выводить
						1 : выводить префикс (+ условие для чисел с плавающей точкой) */
						
	long accuracy;  /* точность вычисления (для чисел) */
	
	long width;		/* ширина поля */
	
	char length;    /* был ли введен спецификатор h, l или L 
						содержит символ 'h', 'l', 'L' или ничего (length = 0) */
						
	char capacity;  /* разрядность выводимого числа
						содержит число 10, 8 или 16 (по умолч. 10) */
						
	char symbol;    /* для u, x, X, o, p, e, E (определяет, какие символы выводятся вместе с цифрами)
						содержит спецификатор формата */
} format_t;

// wchar_t это int

union types {
	long type_int;		/* d, для совместимости с %ld (long int) */
	int type_char;		/* wchar_t, для c и lc */
	long double type_float;	/* f, для совместимости с %Lf (long double) и %lf (double) */
	char * type_str;	/* wchar_t, для s и ls */
	unsigned long type_unsigned;
	void * type_point;	/* p */
};

int s21_sprintf(char *, char *, ...);


#endif
