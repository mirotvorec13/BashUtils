GCC				=gcc
ARCHIVE			=ar rc
CFLAGS			=-c -Wextra -Wall -Werror --std=c11 --pedantic # -Ofast
DEBUG_FLAG		=-g3 -Wextra -Wall -Werror
TEST_DIR 		= tests

all: build

build: s21_cat

s21_cat: s21_cat.o myCFunctions.o
	$(GCC) s21_cat.o myCFunctions.o -o s21_cat

s21_cat.o: s21_cat.c
	$(GCC) $(CFLAGS) s21_cat.c -o s21_cat.o

myCFunctions.o: myCFunctions.c myCFunctions.h
	$(GCC) $(CFLAGS) myCFunctions.c -o myCFunctions.o

clear:
	clear

clean:
	rm -f *.o
	rm -f s21_cat
	clear

rebuild: clean build

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem --std=c11 *.c

val:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat
cl-i:
	clang-format -i *.c
	clang-format -i *.h

test: clear all
	chmod u=rwx $(TEST_DIR)/test_cat.sh
	bash $(TEST_DIR)/test_cat.sh