#include "myCFunctions.h"

void pars_opt(int argc, char **argv, Options *opt, int *err) {
  int vb;

  static struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((vb = getopt_long(argc, argv, "beEnstTv", long_options, NULL)) != -1) {
    if (vb == 'n') opt->n = 1;
    if (vb == 's') opt->s = 1;
    if (vb == 'b') opt->b = 1;
    if (vb == 'v') opt->v = 1;
    if (vb == 'T') opt->t = 1;
    if (vb == 'E') opt->e = 1;
    if (vb == 't') {
      opt->t = 1, opt->v = 1;
    }
    if (vb == 'e') {
      opt->e = 1, opt->v = 1;
    }
    if (vb == '?') *err = 1;
  }
}

int print_file(int argc, char **argv, Options *opt, int *err) {
  int last = '\n';
  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (fp != NULL) {
      int c = fgetc(fp);

      while (c != EOF) {
        print_symb(c, opt, &last);
        c = fgetc(fp);
      }
      fclose(fp);

    } else
      *err = 1;
  }
  exit(*err);
}

void print_symb(int c, Options *opt, int *last) {
  if (!(opt->s && *last == '\n' && c == '\n' && opt->print_line)) {
    if (*last == '\n' && c == '\n')
      opt->print_line = 1;
    else
      opt->print_line = 0;

    if (((opt->n == 1 && opt->b != 1) || (opt->b == 1 && c != '\n')) &&
        *last == '\n') {
      opt->index = opt->index + 1;
      printf("%6d\t", opt->index);
    }

    if (opt->e == 1 && c == '\n') printf("$");

    if (opt->t == 1 && c == '\t') {
      printf("^");
      c = '\t' + 64;
    }

    if (opt->v == 1) {
      v_print(c);
    } else {
      fputc(c, stdout);
    }
  }
  *last = c;
}

void v_print(unsigned char c) {
  if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c >= 32 && c < 127) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 128 + 32) {
    printf("M-");
    (c < 128 + 127) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 128 + 64) : printf("^%c", c + 64);
  }
}