#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "s21_string.h" 
#include "s21_sprintf.h"
#include "print_various_types.h"
#include "analysis.h"

/* 
	файл s21_string.c содержит частичную реализацию стандартной библиотеки string.h,
	включая тип s21_size_t и константу S21_NULL
 */

int s21_sprintf(char * str, char * format, ...) {

	s21_size_t count_printing = 0;
	char * current = format; 				/* указатель на текущий обрабатываемый символ в format */
	s21_size_t len_buffer = 0; 				/* количество символов, которые не относятся к спецификатору формата */
											/* и которые можно сразу вывести в строку str */
	union types value; 						/* для значений разных типов */

	va_list param;
	va_start(param, format); 				/* устанавливаем param на первый необязательный параметр функции s21_printf */

	while (*current != '\0') {
		/*
			i указывает на количество прочитанных символов в строке format
			count_printing - количество выведенных в строку str символов
		*/

		if (*current != '%') {
			/* 
				выводим символы, которые не относятся к спецификаторам
				и которые можно выводить в исходном виде
			 */
			char * next_percent = s21_strchr(current, '%'); /* указатель на следующий символ % */
			len_buffer = (next_percent != S21_NULL) ? next_percent - current : s21_strchr(current, '\0') - current;
			s21_memcpy(str + count_printing, current, len_buffer); 
			current += len_buffer; 			/* теперь current указывает на первый символ после % в строке format (или '\0') */
			count_printing += len_buffer;
		}
		else {
			/* иначе обрабатываем спецификатор формата (%) и выводим значение необходимой переменной */
			current++; /* переходим к первому символу спецификатора */
			format_t info = analysis_format(current);

			/* если было введено значение переменное значение ширины или точности */
			if (info.width == EXPECTED_VALUE) info.width = va_arg(param, int);
			if (info.accuracy == EXPECTED_VALUE) info.accuracy = va_arg(param, int);


			/* обработка спецификаторов */
			if (info.type == '%')
				str[count_printing++] = '%';

			else if (info.type == 'c') {
				value.type_char = va_arg(param, wchar_t); /* lc */
				if(info.length == NONE) value.type_char = (char) value.type_char;

				/* выводим заполнение, после сам символ */
				/* костыль */
				if(info.align ) {
					/* по левому краю */
					for(int i = 0; i < info.width - 1; i++) str[count_printing++] = ' ';
					str[count_printing++] = value.type_char;
				}
				else {
					str[count_printing++] = value.type_char;
					for(int i = 0; i < info.width - 1; i++) str[count_printing++] = ' ';
				}
				
			}

			else if (info.type == 'd') {
				/* для спецификаторов d и i */
				
				value.type_int = va_arg(param, long);
				if (info.length == 'h') value.type_int = (short) value.type_int;
				if (info.length != 'l' && info.length != 'h') value.type_int = (int) value.type_int;
				print_int(str, value.type_int, info, &count_printing);
			}

			else if (info.type == 'u') {
				/* 
					для u, o, x, X, p
					число без знака, т.е знак не учитываем и не выводим
				*/
				
				value.type_unsigned = va_arg(param, unsigned long);

				/* 
					учитываем длину аргумента (long, short), 
					приводя полученное значение к соответствующему типу 
				*/
				if (info.length == 'h') value.type_unsigned = (unsigned short) value.type_unsigned;
				else if (info.length != 'l' && info.length != 'h') value.type_unsigned = (unsigned) value.type_unsigned;

				/* учитываем, что при попытке вывода нулевого адреса,выводим "(nil)" */

				if(info.symbol == 'p' && value.type_unsigned == 0) print_string(str, "(nil)", info, &count_printing);
				
				else print_unsigned(str, value.type_unsigned, info, &count_printing);

			}
			else if (info.type == 'n'){
				/*
					сохраняет количество символов, которые были выведены до %n, в памяти, куда указывает аргумент
					количество выведенных символов (это же и индекс текущего символа в str) хранится в count_printing
				*/
				value.type_point = va_arg(param, int *);
				*((int*)value.type_point) += count_printing;
			}

			else if (info.type == 'f' || info.type == 'e' || info.type == 'g') {

				/* вывод числа с плавающей точкой */
				/* приводим к соответствующей длине */
				if(info.length == 'L') value.type_float = va_arg(param, long double);
				else value.type_float = va_arg(param, double);

				if(info.type == 'e') print_float_e(str, value.type_float, info, &count_printing);
				else if(info.type == 'g') print_float_g(str, value.type_float, info, &count_printing);
				else print_float(str, value.type_float, info, &count_printing);

			}


			else if (info.type == 's') {
				/* вывод строки */
				value.type_str = va_arg(param, char *);
				print_string(str, value.type_str, info, &count_printing);
			}

			 /* здесь будет происходить обработка остальных типов */

			current = s21_strpbrk(current, SPECIFIERS); /* переходим к спецификатору формата (конечный символ) */
			current++; /* переходим к символу, который уже не имеет отношения к данному формату */
		}
	}
	str[count_printing] = '\0'; /* конец строки */

	return count_printing; /*возвращаем количество записанных в строку str символов */
}
