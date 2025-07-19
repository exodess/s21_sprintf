/* 
	этот файл содержит реализацию функций, 
	которые используються в s21_sprintf для вывода 
	переменных различных типов данных
*/

#include "print_various_types.h"
#include "tools.h"

void print_string(char * str, char * value, format_t info, s21_size_t * count) {
	/* 
		здесь заполнение представлено только пробелами
		если указатель на строку равен 0, то выводим "(null)"
	 */
	if(value == S21_NULL) value = "(null)";

	int len = (info.accuracy == DEFAULT_VALUE || info.accuracy > (int) s21_strlen(value)) ? (int) s21_strlen(value) : info.accuracy;
	int count_filling = (info.width > len) ? info.width - len : 0; /* количество символов для заполнения */

	if (info.align) {
		/* 
			выравнивание по правому краю
			сначала выводим заполнение, потом строку
		*/
		for (int i = 0 ; i < count_filling; i++) str[(*count)++] = ' ';
		s21_memcpy(str + *count, value, len);
		*count += len;
	}
	else {
		/* 
			выравнивание по левому краю
			сначала выводим строку, потом заполнение
		*/
		s21_memcpy(str + *count, value, len);
		*count += len;
		for (int i = 0 ; i < count_filling; i++) str[(*count)++] = ' ';
	}
}


void print_unsigned(char * str, long unsigned value, format_t info, s21_size_t * count) {
	/*
		Для %p присутствует также вывод знака!
		
		int count_printing = *count;
		info.accuracy показывает минимальное количество выводимых цифр в числе
	*/

	int count_filling = 0;
	int len = len_number(value, info.capacity);

	if (info.prefix && info.symbol != 'u')
		/* 
			префикс выводится только для спецификаторов o, x, X
			для o это 0, для x и X это 0x и 0X соответственно
		*/
		len += (info.symbol == 'o') ? 1 : 2;

	int min_len_number = (len > info.accuracy) ? len : info.accuracy;
	count_filling = (info.width > min_len_number) ? info.width - min_len_number: 0;

	if (info.align) {
		/* 
			выравнивание по правому краю 
		   	если заполнение представлено пробелами, оно должно идти перед префиксом 
		   	если заполнение представлено нулями, то оно должно идти после префикса и перед числом 
		*/

		/* вывод заполнения, если оно представлено пробелами */
		if (info.fill == ' ') for (int i = 0; i < count_filling; i++) str[(*count)++] = info.fill;

		if(info.symbol == 'p'){
			*count += print_sign(str + *count, value, info.sign); 
			info.symbol = 'x';
		} 
		
		/* вывод префикса, если он есть */
		if (info.prefix &&  info.symbol != 'u') {
			str[(*count)++] = '0';
			if (info.symbol != 'o') str[(*count)++] = info.symbol;
		}

		/* вывод заполнения, если оно представлено нулями */
		if (info.fill == '0') for (int i = 0; i < count_filling; i++) str[(*count)++] = info.fill;

		/* вывод числа в строку */
		for(int i = 0; i < (info.accuracy - len); i++) str[(*count)++] = '0';
		*count += print_number(str + *count, value, info.capacity, info.symbol);
	}
	else {
		/* выравнивание по левому краю */
		/* сначала выводим знак, префикс (если есть) и само число, после заполнение */

		if(info.symbol == 'p'){
			*count += print_sign(str + *count, value, info.sign); 
			info.symbol = 'x';
		} 
		if (info.prefix &&  info.symbol != 'u') {
			str[(*count)++] = '0';
			if (info.symbol == 'x' || info.symbol == 'X') str[(*count)++] = info.symbol;
		}
		for(int i = 0; i < (info.accuracy - len); i++) str[(*count)++] = '0';
		*count += print_number(str + *count, value, info.capacity, info.symbol);

		for (int i = 0; i < count_filling; i++) str[(*count)++] = ' '; /* вывод заполнения */
	}


}


void print_float(char * str, long double value, format_t info, s21_size_t * count) 
{

	/* вывод числа с плавающей точкой */

	int count_filling = 0;
	int len = len_number((long unsigned) MOD(value), 10);
	long temp_for_sign = (value >= 0.0) ? 1 : -1;
	/* получение длины целой части числа (длина = длина_целой_части + точность + [точка] + знак) */
	len += info.accuracy + (info.prefix || info.accuracy) + (value < 0 || info.sign);
	count_filling = (info.width > len) ? info.width - len : 0;
	long double uvalue = MOD(value);
	if(info.accuracy == 0) uvalue += 0.5;
	if (info.align) {
		/*
			выравнивание по правому краю
			если заполнение осуществляется с помощью '0', то сначала выводим знак, заполнение('0'), а уже после само число
		*/
		if (info.fill == '0') *count += print_sign(str + *count, temp_for_sign, info.sign);

		for (int i = 0; i < count_filling; i++)
			str[(*count)++] = info.fill; /* вывод заполнения */

		/* если заполнение осуществляется пробелами, то сначала выводим заполнения, а уже после знак и само число */
		if (info.fill == ' ') *count += print_sign(str + *count, temp_for_sign, info.sign);

		*count += print_number(str + *count, uvalue, 10, NONE);
		if (info.accuracy || info.prefix)
			/* 
				выводим точку, только если есть после нее дробные значения
				или это прописано с помощью флага #
			*/
			str[(*count)++] = '.';
		uvalue -= (int) uvalue;
		for(int i = 0; i < info.accuracy; i++) {
			uvalue = uvalue * 10.0;
			if(i == info.accuracy - 1) uvalue += 0.5;
			str[(*count)++] = (char)(uvalue) % 10 + 0x30;
			uvalue -= (int) uvalue;
		}
	}
	else {
		/* выравнивание по левому краю */
		*count += print_sign((str + *count), temp_for_sign, info.sign);
		*count += print_number(str + *count, uvalue, 10, NONE);

		if (info.accuracy || info.prefix) str[(*count)++] = '.';

		uvalue -= (int) uvalue;
		for(int i = 0; i < info.accuracy; i++) {
			uvalue = uvalue * 10.0;
			if(i == info.accuracy - 1) uvalue += 0.5;
			str[(*count)++] = (char)(uvalue) + 0x30;
			uvalue -= (int) uvalue;
		}
		for (int i = 0; i < count_filling; i++)
			str[(*count)++] = ' '; /* вывод заполнения */

	}
}

void print_int(char * str, long value, format_t info, s21_size_t * count) 
{
	/* если длина числа меньше минимальной точности, то происходит заполнение нулями */

	int len = len_number(MOD(value), 10);
	
	int min_len_number = (len > info.accuracy) ? len : info.accuracy;
	/* len += (value < 0 || info.sign);  нужен ли дополнительный символ для знака */ 
	int count_filling = (info.width > min_len_number) ? (info.width - min_len_number) : 0; /* количество символов для заполнения */
	count_filling -= (value < 0 || info.sign);
	if (info.align) {
		/* 	
			выравнивание по правому краю
			сначала выводится заполнение, после знак числа и значение числа
		*/
		
		if (info.fill == '0') *count += print_sign(str + *count, value, info.sign);

		/* вывод заполнения */
		for (int i = 0; i < count_filling; i++) str[(*count)++] = info.fill;
			
		/* вывод знака числа, если заполнение представлено  */
		if (info.fill == ' ') *count += print_sign(str + *count, value, info.sign);
		/* вывод самого числа */
		for(int i = 0; i < (min_len_number - len); i++) str[(*count)++] = '0';
		*count += print_number(str + *count , MOD(value), 10, NONE);
	}
	else {
		/* 
			выравнивание по левому краю
			сначала выводится знак числа и значение числа, после заполнение (пробелами)
		*/
		*count += print_sign(str + *count, value, info.sign);

		/* вывод самого числа */
		for(int i = 0; i < (min_len_number - len); i++) str[(*count)++] = '0';
		*count += print_number(str + *count, MOD(value), 10, NONE);

		/* вывод заполнения */
		for (int i = 0; i < count_filling; i++) str[(*count)++] = ' ';

	}
}

void print_float_e(char * str, long double value, format_t info, s21_size_t * count)
{
	/* 
		Вывод числа с плавающей точкой в формате экспоненты

		
		Вывод в данном формате схож с %f,
		за исключением того, чтьо в конце добавляется "eSNN"
		NN - двухзначное число экспоненты, S - знак числа

		Пример: 2.34e+01
	 */
	int count_filling = 0;
	long temp_for_sign = (value >= 0.0) ? 1 : -1;
	/* long double uvalue = MOD(value); */

	/*  
		длина поля сладывается из 1 символа целой части числа,
		точности вывода (кол-во знаков после запятой),
		и строки "eSNN" (s - знак, NN - экспонента)
	*/
	int len = 1 + info.accuracy + 4;
	
	/* будет ли выведен знак и символ '.' */
	len += (value < 0 || info.sign) + (info.accuracy || info.prefix);
	count_filling = (info.width > len) ? info.width - len : 0;

	if(info.align) {
		/* 
			выравнивание по правому краю 
			сначала выводим заполнение, знак, потом число
			и завершающую последовательность "eSNN"
		*/
			
		if (info.fill == '0') *count += print_sign(str + *count, temp_for_sign, info.sign);

		for(int i = 0; i < count_filling; i++) str[(*count)++] = info.fill;

		if (info.fill == ' ') *count += print_sign(str + *count, temp_for_sign, info.sign);

		/* вывод числа */
		print_number_for_e(str, value, info, count);
	}

	else {
		/* 
			выравнивание по левому краю 
			сначала выводим знак, число, последовательность "eSNN",
			после заполнение
		*/

		*count += print_sign(str + *count, temp_for_sign, info.sign);

		print_number_for_e(str, value, info, count);


		for(int i = 0; i < count_filling; i++) str[(*count)++] = ' ';
	}
	
}
int print_number_for_e(char * str, long double value, format_t info, s21_size_t * count)
{
	int exp = 0;
	long double uvalue = MOD(value); 
	while(uvalue >= 10.0) {
		uvalue /= 10;
		exp++;
	}
	while(uvalue < 1.0){
		uvalue *= 10;
		exp--;
	}
	if(info.accuracy == 0) uvalue += 0.5;
	str[(*count)++] = (int) uvalue + 0x30;

	uvalue -= (int) uvalue;
	if(info.prefix || info.accuracy) str[(*count)++] = '.';
	for(int i = 0; i < info.accuracy; i++) {
		uvalue = uvalue * 10.0;
		if(i == info.accuracy - 1) uvalue += 0.5;
		str[(*count)++] = (char)(uvalue) + 0x30;
		uvalue -= (int) uvalue;
	}
	str[(*count)++] = info.symbol;
	str[(*count)++] = (exp >= 0) ? '+' : '-';
	str[(*count)++] = MOD(exp / 10) + 0x30;
	str[(*count)++] = MOD(exp % 10) + 0x30;

	return exp;
}

void print_float_g(char * str, long double value, format_t info, s21_size_t * count)
{
	/* 
		объединяет в себе %e и %f
		info.accuracy описывает минимальное количество цифр для целой и дробной частей в сумме
	*/
	int count_filling = 0;
	long temp_for_sign = (value >= 0.0) ? 1 : -1;
	long double uvalue = MOD(value);
	int len = len_number(uvalue, 10);

	if (info.width > (info.accuracy + 1 + (info.sign || info.prefix))) count_filling = info.width - info.accuracy + 1 + (info.sign || info.prefix);
	if(info.accuracy == DEFAULT_VALUE) info.accuracy = 6;
	if (len > info.accuracy || uvalue < EPS) {
		/* выводим в виде %e с accuracy = 0 */
		info.accuracy = 0;
		info.symbol -= 2;
		print_float_e(str, value, info, count);
	}
	else {
		info.accuracy -= len;
		if(info.accuracy == 0) uvalue += 0.5;

		if(info.align) {
			if (info.fill == '0') *count += print_sign(str + *count, temp_for_sign, info.sign);
	
			for (int i = 0; i < count_filling; i++)
				str[(*count)++] = info.fill; /* вывод заполнения */
	
			/* если заполнение осуществляется пробелами, то сначала выводим заполнения, а уже после знак и само число */
			if (info.fill == ' ') *count += print_sign(str + *count, temp_for_sign, info.sign);

			*count += print_number(str + *count, uvalue, 10, NONE);
			
			if (info.accuracy || info.prefix)
				/* 
					выводим точку, только если есть после нее дробные значения
					или это прописано с помощью флага #
				*/
				str[(*count)++] = '.';
			
			uvalue -= (int) uvalue;
			int i = 0;
			while(i < info.accuracy && ((int)(uvalue * 10.0) != 0)){
				uvalue = uvalue * 10.0;
				if(i == info.accuracy - 1) uvalue += 0.5;
				str[(*count)++] = (char)(uvalue) % 10 + 0x30;
				uvalue -= (int) uvalue;
				i++;
			}
			/* если не было */
			if(i == 0 && !info.prefix) *count -= 1;
		}
		if(info.align == 0) {
			*count += print_sign((str + *count), temp_for_sign, info.sign);
			*count += print_number(str + *count, uvalue, 10, NONE);
	
			if (info.accuracy || info.prefix) str[(*count)++] = '.';

			uvalue -= (int) uvalue;
			int i = 0;
			while(i < info.accuracy && ((int)(uvalue * 10.0) != 0)){
				uvalue = uvalue * 10.0;
				if(i == info.accuracy - 1) uvalue += 0.5;
				str[(*count)++] = (char)(uvalue) % 10 + 0x30;
				uvalue -= (int) uvalue;
				i++;
			}
			if(i == 0 && !info.prefix) *count -= 1;

			for (int i = 0; i < count_filling; i++)
							str[(*count)++] = ' '; /* вывод заполнения */
		}
	}
}
