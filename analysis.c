/* 
	данные функции написаны для обработки исходной строки format,
	помогая получать из нее информацию о каждом спецификаторе:
	флаги, ширину поля, точность, длину и, наконец, сам спецификатор формата,
	отвечающий за тип переменной
*/

#include "analysis.h"

/* главная функция */
format_t analysis_format(const char * current) 
{
	/*
		эта функция анализирует строку форматирования и
		возвращает информацию о первом спецификаторе, который встретится в строке
		тип format_t содержит в себе информацию о переменной, которую нужно считать с помощью va_arg(),
		а также дополнительные сведения о том, как именно выводить это значение в строку

		спецификатор формата имеет вид: %[флаги][ширина][.точность][длина]спецификатор
	*/
	
	s21_size_t i = 0;
	
	/* информация о текущем спецификаторе */
	format_t info = {NONE, 1, NONE, ' ', NONE, DEFAULT_VALUE, NONE, NONE, 10, NONE};
	/*
		сначала проверяем наличие флага, если нашли - передвигаем указатель вперед
		потом аналогично проверяем наличие ширины, точности, длины и спецификатора
	*/
	if (s21_strchr(FLAGS, *(current + i)) != S21_NULL) {
		/* обработка введенного флага */
		i += analysis_flags(&info, current + i);
	}

	if (s21_strchr(WIDTH, *(current + i)) != S21_NULL)
		/* обработка введенной ширины */
		i += analysis_width(&info, (current + i));

	if (*(current + i) == '.')
		/* обработка введенной точности */
		i += analysis_accuracy(&info, (current + i));

	if (s21_strchr(LENGTH, *(current + i))) {
		/* обработка спецификатора длины (h, l или L) */
		analysis_length(&info, *(current + i));
		i++;
	}

	/* и наконец анализируем спецификатор формата */
	analysis_specifier(&info, *(current + i));

	return info;

}

/* обработка поступившего формата */
void analysis_specifier(format_t * info, char specifier)
{

	/* 
		Список спецификаторов: c, d, i, e, E, f, g, G, o, s, u, x, X, p, n, %
		Спецификаторы, которые различаются: c, d, e, f, g, s, p, n, %

		d, i, o, x, X используются для вывода целых чисел
		e, E, f, g, G используются для вывода чисел с плавающей точкой
		o, u, x, X используются для вывода беззнаковых чисел

		по умолчанию разрядность для каждого числа равна 10
		для x, X, o она изменяется на 16, 16 и 8 соответственно
	*/
	
	/* i и d идентичны */
	if (specifier == 'i') info -> type = 'd'; 

	/* g и G также идентичны */
	else if (specifier == 'g' || specifier == 'G') {
		if(info -> accuracy == DEFAULT_VALUE) info -> accuracy = 6;
		info -> type = 'g';
		info -> symbol = specifier;
	}

	/* для спецификатора f стандартной точностью является 6 знаков после запятой */
	else if(specifier == 'f') {
		if(info -> accuracy == DEFAULT_VALUE) info -> accuracy = 6;
		info -> type = specifier;
		info -> symbol = specifier;
	}
	
	else if (specifier == 'u') {
		info -> type = specifier;
		info -> symbol = specifier;
	}
	
	else if (specifier == 'o') {
		info -> capacity = 8;
		info -> symbol = specifier;
		info -> type = 'u';
	}

	else if (specifier == 'x' || specifier == 'X') {
		info ->	capacity = 16;
		info -> type = 'u';
		info -> symbol = specifier;
	}

	else if (specifier == 'e' || specifier == 'E') {
		if(info -> accuracy == DEFAULT_VALUE) info -> accuracy = 6;
		info -> type = 'e';
		info -> symbol = specifier;
	}

	else if (specifier == 'p') {
		/*
			в мануале для printf представлен спецификатор %p как %#x или %#lx
			т.е приводим адрес аргумента к типу (long) unsigned и выводим с префиксом 0x
		*/
		info -> type = 'u';
		info -> capacity = 16;
		info -> length = 'l';
		info -> prefix = 1;
		info -> symbol = specifier;
	}
	
	else info -> type = specifier;

}

void analysis_length (format_t * info, char ch)
{
	/*
		h применяется вместе: i, d, o, u, x, X
		l применяется вместе: i, d, o, u, x, X, c, s (wchar_t и wchar_t * соответственно)
		L применяется вместе: e, E, f, g, G

		Ни с кем не сочетаются только n и p
	*/
	
	info -> length += ch;

}

s21_size_t analysis_flags(format_t * info, const char * current)
{
	/* флаг [SPACE] сильнее флага '+' */

	/* счетчик количества считанных флагов */
	s21_size_t i = 0; 
	
	while (s21_strchr(FLAGS, *(current + i)) != S21_NULL) {
		char flag = *(current + i);
		if (flag == '-') info -> align = 0; /* выравнивание по левому краю */
		else if (flag == '+') info -> sign = 2; /* выводит и +, и - */
		else if (flag == ' ' && info -> sign != 2) info -> sign = 1; /* выводит -, вместо + ставится пробел */
		else if (flag == '#') info -> prefix = 1; /* будет выведен префикс для o, x, X */
		else if (flag == '0') info -> fill = '0';

		i++;
	}

	return i;
}

s21_size_t analysis_width(format_t * info, const char * start_width)
{
	/* сколько байт занимает число, отвечающее за ширину вывода */
	s21_size_t i = 0; 

	int number = 0;

	if (*start_width == '*') {
		info -> width = EXPECTED_VALUE;
		i = 1;
	}

	else {
		while (s21_is_digit(*(start_width + i))) {
			number *= 10;
			number += (*(start_width + i) - 0x30);
			i++;
		}
		info -> width = number;
	}
	return i;
}

s21_size_t analysis_accuracy(format_t * info, const char * start_acc)
{
	/* число после точки интерпретируется как точность для выводимых чисел */

	s21_size_t i = 1; /* пропускаем точку */
	long number = 0;

	if (*(start_acc + i) == '*') {
		number = EXPECTED_VALUE;
		i++;
	}
	else if (s21_is_digit(*(start_acc + i))){
		while (s21_is_digit(*(start_acc + i))) {
			number *= 10;
			number += (*(start_acc + i) - 0x30);
			i++;
		}

	}
	info -> accuracy = number;
	return i;
}

int s21_is_digit(char ch)
{
	return ((ch >= 48) && (ch <= 57));
}
