#include "myCFunctions.h"

int main(int argc, char **argv) {
  Options opt = {0};
  int err = 0;

  pars_opt(argc, argv, &opt, &err);
  if (err != 1) {
    print_file(argc, argv, &opt, &err);
  }

  return 0;
}
