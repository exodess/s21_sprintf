#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s21_string.h"
#include "s21_sprintf.h"
#include <limits.h>
#define SIZE_MEMORY 128
#define TRUE 1
#define MOD(a) ((a < 0) ? -a : a)

typedef s21_size_t s_t;
typedef unsigned char uchar;
typedef unsigned int uint;


void dump_memory(void *, s_t n); // выводит дамп памяти (первые n байт)
int menu(void);
void fill_memory(unsigned char * , unsigned int); // заполняет участок памяти случайными значениями
void clr(void); // очищает дисплей
void start_testing(unsigned char * memory, unsigned int size, char * test_str); // основная функция

int main(void) {
	char * test_str = "This is test string";
	char * string = malloc(SIZE_MEMORY * sizeof(char));
	int temp = 0;

	uchar * page = (uchar *) malloc(SIZE_MEMORY * sizeof(uchar)); // выделяем память для дальнейшего тестирования
	clr(); // очищаем экран
	printf("Было выделено %d байта для тестирования функций\n", SIZE_MEMORY);
	puts("Память заполняется случайными значениями...");

	fill_memory(page, SIZE_MEMORY); // заполняем участок памяти
	puts("Память успешно заполнена случайными значениями!");
	printf("Также есть строка test_str = \"%s\"", test_str);
	dump_memory(page, (uint) SIZE_MEMORY); // выводим дамп памяти


	s21_sprintf(string,"pUP<%%>UP%1cU|f:%#*.*f|d:%5ld|u:%p|%n|%-8.0f|o: %5.5o|u:%# 0-7X|s:%3.5s|%c|u:%+# 05u", 'q', 9, 3, 30.4, 100000, string, &temp, -34566.2334, 100, 3564, "Test str", 'и', -234); 
	printf("string: ");
	int a = -34566;
	printf("%d", MOD(a));
	dump_memory(string, (uint) SIZE_MEMORY);
	// printf("format     :  %s", "pUP<%%>UP%cU%10d\n");
	printf("printf      : \"pUP<%%>UP%1cU|f:%9.3f|d:%5ld|u:%p|48|%-8.0f|o: %5.5o|u:%# 0-7X|s:%#3.5s|%c|u:%+# 05u\"\n", 'q', 30.4, 100000, string, -34566.2334, 100, 3564, "Test str", 'и', -234);
	printf("s21_sprintf : \"%s\"\n", string);
	printf("%p (str = %p)\n", s21_strchr(string, '\0'), string);
	printf("Press any key...");
	getchar();
	
	start_testing(page, (uint) SIZE_MEMORY, test_str);

	free(string);
	free(page); // освобождаем выделенную память
	return 0;
}

void start_testing(uchar * memory, uint size, char * str) {
	int c = 0;
	uint choice = 0, count = 0;
	puts("Эта программа была создана для тестирования функций s21_*");
	while (TRUE) {
		choice = menu();
		if (choice == 0) {
			printf("Memory:");
			dump_memory(memory, size);
			printf("Test string (len = %lu):", s21_strlen(str) );
			dump_memory(str, 20);
		}
		else if (choice == 1) {
			printf("Какое значение хотите найти: (hex) ");
			scanf("%x", &c);
			printf("memory:    : %p\n", memory);
			printf("s21_memchr : %p\n", s21_memchr(memory, c, SIZE_MEMORY));
			printf("memchr     : %p\n", memchr(memory, c, SIZE_MEMORY));
		}
		else if (choice == 2) {
			printf("Сколько байтов вы хотите сравнить?: ");
			scanf("%u", &count);
			printf("Сравнение первых %d байт памяти...\n", count);
			printf("memcmp     : %d\n", memcmp(memory, str, count));
			printf("s21_memcmp : %d\n", s21_memcmp(memory,str, count));
		}

		else if (choice == 3) {
			printf("Сколько байтов вы хотите скопировать? ");
			if (scanf("%d", &c) == 1 && c > 0) {
				printf("Копирование %d байтов...\n", c);
				printf("memcpy     : %p\n", memcpy(memory, str, c));
				printf("s21_memcpy : %p\n", s21_memcpy(memory, str, c));
			}
			else puts("Неверно указано количество байт памяти, необходимое для копирования");
		}
		else if (choice == 4) {
			printf("Сколько байт памяти вы хотите заполнить значениями? ");
			if (scanf("%d", &c) == 1 && c > 0) {
				printf("Область памяти размером в %d байт будет заполнена значением 'c'", c);
				printf("memset : %p\n", memset(memory, 'c', c));
				printf("s21_memset : %p\n", s21_memset(memory + c, 'd', c));
			}
		}
		else if (choice == 5) {
			printf("Какой символ вы хотите найти в строке? ");
			getchar();
			if (scanf("%c", &c) == 1) {
				printf("str (%p) : %s\n", str, str);
				printf("strchr     : %p\n", strchr(str, c));
				printf("s21_strchr : %p\n", s21_strchr(str, c));
			}
		}
		else if (choice == 6) {
			printf("str (%p) : %s\n", str, str);
			printf("strlen     : %lu\n", strlen(str));
			printf("s21_strlen : %lu\n", s21_strlen(str));
		}
		else {
			clr();
			break;
		}
		printf("Press ENTER...");
		getchar();
		getchar();
	}
}

int menu(void)
{
	clr();
	int choice;
	puts("----------------------------------------------------------");
	puts("| Выберите пункт, который хотите запустить: ");
	puts("| 0: Посмотреть дамп памяти");
	puts("| 1: Поиск первого вхождения символа в памяти (s21_memchr)");
	puts("| 2: Сравнение n байтов памяти (s21_memcmp)");
	puts("| 3: Копировать n байтов (s21_memcpy)");
	puts("| 4: Заполнить n байт памяти значением c (s21_memset)");
	puts("| 5: Найти в строке конкретное значение n (s21_strchr)");
	puts("| 6: Узнать длину строки str (s21_strlen)");
	puts("-----------------------------------------------------------");
	printf("Мой выбор (по умолчанию 0): ");

	if (scanf("%d", &choice) == 0) choice = -1;

	return choice;
}

// выводит содержимое участка памяти
void dump_memory(void * mem, s_t n)
{
	// функция позволяет просматривать содержимое участка памяти в байтах
	// работает как утилита hexdump
	uchar * p = (uchar *) mem;
	printf("\n%p :", p);
	// n - размер участка памяти mem в байтах
	for(s_t i = 0; i < n; i ++) {
		if(i > 0 && i % 8 == 0) printf("\n%p :", (p + i)); 
		printf(" %02x", p[i]);
	}
	printf("\n\n");
}

void clr(void) {
	printf ("\033[0d\033[2J");
	fflush(stdout);
}

void fill_memory(uchar * memory, uint seed) {
	for (int i = 0; i < seed ; i++)
		memory[i] = (uchar) rand() % seed; // заполняем i-ячейку памяти случайным значением (от 0 до seed)
}
