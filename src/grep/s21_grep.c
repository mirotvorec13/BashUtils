#include "myGFunctions.h"

int main(int argc, char **argv) {
  Options opt = {0};

  if (argc < 2) {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...");
    return 1;
  }
  pars_opt(argc, argv, &opt);
  regex_pat(&opt, argv);

  opt.num_files = argc - optind;

  while (optind < argc) {
    grepfunc(argv[optind], &opt);

    optind++;
  }
  return 0;
}
