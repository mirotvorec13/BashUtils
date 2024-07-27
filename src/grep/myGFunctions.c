#include "myGFunctions.h"

void pars_opt(int argc, char **argv, Options *opt) {
  int vb;
  while ((vb = getopt(argc, argv, "ce:insvlhof:")) != -1) {
    switch (vb) {
      case 'e':
        regex_cat(opt, optarg);
        opt->e = 1;  // шаблон
        break;
      case 'i':
        opt->i = 1;  // Игнорирует различия регистра
        break;
      case 'n':
        opt->n =
            1;  // Предваряет каждую строку вывода номером строки из файла ввода
        break;
      case 's':
        opt->s = 1;  // Подавляет сообщения об ошибках о несуществующих или
                     // нечитаемых файлах
        break;
      case 'v':
        opt->v = 1;  // ивертирует смысл поиска соответствий
        break;
      case 'o':
        opt->o =
            1;  // Печатает только совпадающие (непустые) части совпавшей строки
        break;
      case 'c':
        opt->c = 1;  // Выводит только количество совпадающих строк
        opt->o = 0;
        break;
      case 'h':
        opt->h =
            1;  // Выводит совпадающие строки, не предваряя их именами файлов
        break;
      case 'f':
        regex_set_from_file(opt, optarg);  // Получает регулярные выражения
        break;
      case 'l':
        opt->l = 1;  // Выводит только совпадающие файлы
        break;
      case '?':
        opt->err = 1;
        fprintf(stderr, "Invalid options in %s\n", argv[0]);
    }
  }
  if (opt->l) opt->c = opt->o = 0;
  if (opt->c) opt->o = 0;
  if (opt->v && opt->o) opt->err = 1;
  if (opt->err) exit(EXIT_FAILURE);
}

void regex_cat(Options *opt, char *regex) {
  if (!opt->e && !opt->f) {
    strcpy(opt->pattern, regex);
  } else {
    strcat(opt->pattern, "|");
    strcat(opt->pattern, regex);
  }
}

int set_flags(Options *opt, int flags) {
  int flag = flags;
  if (opt->i) flag = flag | REG_ICASE;

  if (opt->e || opt->f || opt->o) flag = flag | REG_EXTENDED;
  return flag;
}

void regex_pat(Options *opt, char **argv) {
  if (!opt->e && !opt->f) {
    strcpy(opt->pattern, argv[optind]);
    optind++;
  }
}

void regex_set_from_file(Options *opt, char *filename) {
  FILE *fp = NULL;
  size_t t = 0;
  char *line = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
    if (!opt->s) {
      fprintf(stderr, "s21_grep: %s No such file or directory\n", filename);
    }
  } else {
    int size = 0;
    while ((size = getline(&line, &t, fp)) != -1) {
      if (line[size - 1] == '\n') line[size - 1] = '\0';
      regex_cat(opt, line);
      opt->f = 1;
    }
    if (line) free(line);
  }
  if (fp) fclose(fp);
}

int flag_o(Options *opt, char *line, int param, int num_line, char *file_name) {
  int count = 0;
  regex_t regex;
  // regoff_t off, len;

  if (regcomp(&regex, opt->pattern, param)) exit(EXIT_FAILURE);
  char *s = line;
  regmatch_t pmatch[100];
  count = regexec(&regex, s, 1, pmatch, 0);
  while (count == 0) {
    // off = pmatch[0].rm_so + (s - line);
    // len = (pmatch[0].rm_eo - pmatch[0].rm_so);

    // printf("\t\t\t%d, %d", off, len);
    if (opt->n && opt->num_files > 1 && !opt->h) {
      printf("%s:%d:%.*s\n", file_name, num_line,
             (int)(pmatch[0].rm_eo - pmatch[0].rm_so), s + pmatch[0].rm_so);
    } else {
      if (opt->n) {
        printf("%d:%.*s\n", num_line, (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
               s + pmatch[0].rm_so);
      } else {
        if (opt->num_files > 1 && count >= 0 && !opt->h) {
          printf("%s:%.*s\n", file_name,
                 (int)(pmatch[0].rm_eo - pmatch[0].rm_so), s + pmatch[0].rm_so);
        } else {
          printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                 s + pmatch[0].rm_so);
        }
      }
    }
    // printf("#%d:\n", i);
    // printf("offset = %jd; length = %jd\n", (intmax_t) off, (intmax_t) len);
    // printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so);
    s += pmatch[0].rm_eo;
    count = regexec(&regex, s, 1, pmatch, 0);
  }

  regfree(&regex);
  return count;
}

int grepfunc(char *file_name, Options *opt) {
  int param = set_flags(opt, REG_NEWLINE);
  FILE *fp = NULL;
  opt->matches_count = 0;
  if ((fp = fopen(file_name, "r")) == NULL) {
    if (!opt->s) {
      fprintf(stderr, "s21_grep: %s No such file or directory\n", file_name);
    }
    return 1;
  } else {
    int count = 0;
    count++;
    int num_line = 0;
    char *line = NULL;
    int size = 0;
    size_t t = 0;

    while ((size = getline(&line, &t, fp)) != -1) {
      num_line = num_line + 1;
      int matches = !regex_matches(opt, line, param);
      if ((opt->l && matches) || (opt->v && opt->l)) {
        printf("%s\n", file_name);
        break;
      }

      if (!opt->l && !opt->err && (opt->v ^ matches)) {  // первый отрабатываемы
        opt->matches_count++;
        if (opt->o) {
          flag_o(opt, line, param, num_line, file_name);
        }
        if (opt->n && !opt->l && !opt->o && !opt->c) {
          if (line[size - 1] == '\n') line[size - 1] = '\0';
          if (opt->num_files > 1 && count >= 0 && !opt->h) {
            printf("%s:%d:%s\n", file_name, num_line, line);
          } else {
            printf("%d:%s\n", num_line, line);
          }
        } else {
          if (opt->num_files > 1 && count == 0 && !opt->l) {
            printf("%s", file_name);
          } else if (!opt->c) {
            print_found(opt, line, file_name, count);
          }
        }

        if (!(opt->i || opt->f || opt->v || opt->o || opt->c || opt->n ||
              opt->e || opt->h || opt->l)) {
          if (opt->num_files > 1 && count >= 0) {
            printf("%s:%s", file_name, line);
          } else {
            printf("%s", line);
          }
        }
      }
    }
    if (line) free(line);
    if (opt->c) {
      if (opt->num_files > 1 && !opt->h) {
        printf("%s:%d\n", file_name, opt->matches_count);
      } else {
        printf("%d\n", opt->matches_count);
      }
    }
  }
  fclose(fp);
  return 0;
}

int regex_matches(Options *opt, char *line, int param) {
  regex_t regex;
  regmatch_t pmatch[100];
  if (regcomp(&regex, opt->pattern, param)) exit(EXIT_FAILURE);
  char *s = line;
  int count = regexec(&regex, s, 1, pmatch, 0);
  regfree(&regex);
  return count;
}

void print_found(Options *opt, char *line, char *file_name, int count) {
  if ((opt->h || opt->f || opt->e || opt->i || opt->v) && !opt->o) {
    if (opt->num_files > 1 && count >= 0 && !opt->n && !opt->h) {
      printf("%s:%s", file_name, line);
    } else {
      printf("%s", line);
    }
  }
  if ((line[strlen(line) - 1] != '\n') && !opt->o) printf("\n");
}