#ifndef ANALYSIS_H
#define ANALYSIS_H


#include "s21_sprintf.h"

format_t analysis_format(const char * );
s21_size_t analysis_flags(format_t *, const char *);
s21_size_t analysis_width(format_t *, const char *);
s21_size_t analysis_accuracy(format_t *, const char *);
void analysis_length (format_t *, char);
void analysis_specifier(format_t *, char);
int s21_is_digit(char);


#endif
