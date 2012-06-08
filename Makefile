noragel: dreadlock

all: ragel dreadlock

dreadlock: dreadlock.c parse.c dreadlock.h
	gcc -o dreadlock -O2 -Wall -Werror -g -fno-strict-aliasing parse.c dreadlock.c -ltask

ragel: parse.rl
	ragel parse.rl

install:
	cp dreadlock /usr/local/bin

memcheck:
	valgrind --leak-check=full ./dreadlock

clean:
	rm ./dreadlock
