#ifndef TOOLS_H
#define TOOLS_H

#include "s21_sprintf.h"

int print_value(char *, void *, format_t, int);
s21_size_t print_number(char *, unsigned long, const int, const char);
int len_number(long unsigned, int);
s21_size_t print_sign(char *, long, char);

#endif
