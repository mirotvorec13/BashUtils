#ifndef MY_GFUNCTIONS_H
#define MY_GFUNCTIONS_H
#define _GNU_SOURCE
#define BUF_SIZE 1024

#include <getopt.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int c;  // Выводит только количество совпадающих строк
  int e;  // шаблон
  int i;  // Игнорирует различия регистра
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах
  int v;  // ивертирует смысл поиска соответствий
  int l;  // Выводит только совпадающие файлы
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки
  int f;  // Получает регулярные выражения
  int num_files;
  int matches_count;
  char pattern[BUF_SIZE];
  int err;
} Options;

void print_found(Options *opt, char *line, char *file_name, int count);
int flag_o(Options *opt, char *line, int param, int num_line, char *file_name);
int set_flags(Options *opt, int flags);
void regex_pat(Options *opt, char **argv);
int regex_matches(Options *opt, char *line, int param);
void regex_cat(Options *opt, char *regex);
void regex_set_from_file(Options *opt, char *filename);
int grepfunc(char *file_name, Options *opt);
void pars_opt(int argc, char **argv, Options *opt);

#endif
