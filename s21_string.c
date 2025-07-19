#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *new_str = (const unsigned char *)str;
  unsigned char simbol = (unsigned char)c;

  s21_size_t i = 0;
  for (; i < n; i++)
    if (new_str[i] == simbol) break;

  return (i < n) ? (void *)(new_str + i) : S21_NULL;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *new_str1 = (const unsigned char *)str1;
  const unsigned char *new_str2 = (const unsigned char *)str2;

  s21_size_t i = 0;
  for (; i + 1 < n; i++)
    if (new_str1[i] != new_str2[i]) break;

  return (new_str1[i] - new_str2[i]);
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *new_dest = (unsigned char *)dest;
  const unsigned char *new_src = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) new_dest[i] = new_src[i];

  return new_dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  if (str == NULL) return NULL;
  unsigned char *new_str = (unsigned char *)str;
  unsigned char simbol = (unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) new_str[i] = simbol;

  return new_str;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;

  while (*ptr != '\0') ptr++;

  while (*src != '\0' && n > 0) {
    *ptr = *src;
    ptr++;
    src++;
    n--;
  }

  *ptr = '\0';

  return dest;
}

char *s21_strchr(const char *str, int c) {
  if (str == NULL) return NULL;
  const char *find = NULL;

  while (*str != '\0' && !find) {
    if (*str == (char)c) find = str;
    str++;
  }

  if ((char)c == '\0') find = str;

  return (char *)find;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int res = 0;

  while (*str1 != '\0' && *str2 != '\0' && n > 0 && !res) {
    if (*str1 != *str2) res = (unsigned char)*str1 - (unsigned char)*str2;
    str1++;
    str2++;
    n--;
  }

  if (n != 0 && !res) res = (unsigned char)*str1 - (unsigned char)*str2;

  return res;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  if (dest == NULL || src == NULL) return NULL;
  for (s21_size_t i = 0; i < n; i++) {
    dest[i] = *src;
    if (*src != '\0') src++;
  }

  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int find = 0;
  while (str1[len] != '\0' && !find) {
    const char *ptr = str2;
    while (*ptr != '\0' && !find) {
      if (str1[len] == *ptr) find = 1;
      ptr++;
    }
    if (!find) len++;
  }

  return len;
}


s21_size_t s21_strlen(const char *str) {
	return (s21_size_t) (s21_strchr(str, '\0') - str);
}

char *s21_strpbrk(const char *str1, const char *str2) {
  if (str1 == NULL || str2 == NULL) return NULL;
  int find = 0;
  while (*str1 != '\0' && !find) {
    char *ptr = (char *)str2;
    while (*ptr != '\0' && !find) {
      if (*str1 == *ptr) find = 1;
      ptr++;
    }
    if (!find) str1++;
  }

  return (find == 1) ? (char *)str1 : NULL;
}

char *s21_strrchr(const char *str, int c) {
  const char *res = NULL;

  for (; *str != '\0'; str++)
    if (*str == (char)c) res = str;

  if ((char)c == '\0') res = str;

  return (char *)res;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = NULL;
  if (*needle == '\0')
    res = (char *)haystack;
  else {
    for (s21_size_t i = 0; haystack[i] != '\0' && !res; i++) {
      s21_size_t j = 0;
      while (needle[j] != '\0' && haystack[i + j] == needle[j]) j++;
      if (needle[j] == '\0') res = (char *)(haystack + i);
    }
  }

  return res;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = NULL;
  char *token_start;

  if (str != NULL) last_token = str;

  while (*last_token != '\0' && strchr(delim, *last_token) != NULL)
    last_token++;

  if (*last_token == '\0') {
    token_start = NULL;
  } else {
    token_start = last_token;

    while (*last_token != '\0' && strchr(delim, *last_token) == NULL) {
      last_token++;
    }

    if (*last_token != '\0') {
      *last_token = '\0';
      last_token++;
    } else {
      last_token = NULL;
    }
  }

  return token_start;
}

