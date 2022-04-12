# Author - Emeka Umeozo

CC = gcc
CFLAGS = -g
CPPFLAGS = -std=gnu99 -Wall -pedantic

.PHONY: ALL clean

ALL: router

helper.o: helper.h imports.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c helper.c -o helper.o

router: router.o helper.o
	$(CC) helper.o router.o -o router

router.o: router.c helper.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c router.c -o router.o

clean:
	rm *.o router
