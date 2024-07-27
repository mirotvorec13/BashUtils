GCC				=gcc
ARCHIVE			=ar rc
CFLAGS			=-c -Wextra -Wall -Werror --std=c11 --pedantic # -Ofast
DEBUG_FLAG		=-g3 -Wextra -Wall -Werror
TEST_DIR 		= tests

all: build

build: s21_grep # valgrind cpp

s21_grep: s21_grep.o myGFunctions.o
	$(GCC) s21_grep.o myGFunctions.o -o s21_grep

s21_grep.o: s21_grep.c
	$(GCC) $(CFLAGS) s21_grep.c -o s21_grep.o

myCFunctions.o: myGFunctions.c myGFunctions.h
	$(GCC) $(CFLAGS) myGFunctions.c -o myGFunctions.o

clear:
	clear

clean:
	rm -f *.o
	rm -f s21_grep
	clear

rebuild: clean build

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem --std=c11 *.c

val:
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all -s ./s21_grep -e intel -e int -i -n test_files/test_7_grep.txt
cl-i:
	clang-format -i *.c
	clang-format -i *.h

test: clear all
	chmod u=rwx tests.sh
	bash tests.sh
