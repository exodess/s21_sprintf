CC = gcc
CFLAGS = -Wall -Wextra -Wextra

all : run

run: test_unit
	./test

# проверка работоспособности функции s21_sprintf
test_unit : s21_sprintf.o s21_string.o analysis.o string_unit_tests.o print_various_types.o tools.o
	$(CC) -o test s21_sprintf.o s21_string.o analysis.o string_unit_tests.o print_various_types.o tools.o

s21_sprintf.o : s21_sprintf.c
	$(CC) $(CFLAGS) -c s21_sprintf.c

s21_string.o : s21_string.c
	$(CC) $(CFLAGS) -c s21_string.c

analysis.o : analysis.c
	$(CC) $(CFLAGS) -c analysis.c

string_unit_tests.o : string_unit_tests.c
	$(CC) $(CFLAGS) -c string_unit_tests.c

print_various_types.o : print_various_types.c
	$(CC) $(CFLAGS) -c print_various_types.c

tools.o : tools.c
	$(CC) $(CFLAGS) -c tools.c

# компиляция статической библиотеки
s21_sprintf.a : s21_sprintf.o s21_string.o analysis.o print_various_types.o tools.o
	ar rc s21_sprintf.a s21_sprintf.o s21_string.o analysis.o print_various_types.o tools.o

clean :
	rm -rf *.o *.a test
	

rebuild : clean build
