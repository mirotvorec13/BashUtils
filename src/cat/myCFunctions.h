#ifndef MY_CFUNCTIONS_H
#define MY_CFUNCTIONS_H
#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int b;  // нумерует каждую не пустую строку исключает n
  int e;
  int n;  // нумерует строки
  int s;  // сжимает пустые строки
  int t;
  int v;  // Выводит все спец символы кроме \n \t в виде нотации ^
  int index;
  int print_line;
} Options;

int print_file(int argc, char **argv, Options *opt, int *err);
void pars_opt(int argc, char **argv, Options *opt, int *err);
void print_symb(int c, Options *opt, int *last);
void v_print(unsigned char c);

#endif