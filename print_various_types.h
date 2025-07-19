#ifndef PRINT_VARIOUS_TYPES_H
#define PRINT_VARIOUS_TYPES_H

#include "s21_sprintf.h"

void print_string(char *, char *, format_t, s21_size_t *);
void print_unsigned(char *, long unsigned, format_t, s21_size_t *);
void print_float(char *, long double, format_t, s21_size_t *);
void print_int(char *, long, format_t, s21_size_t *);

void print_float_e(char *, long double, format_t, s21_size_t *);
void print_float_g(char *, long double, format_t, s21_size_t *);
int print_number_for_e(char *, long double, format_t, s21_size_t *);

#endif
