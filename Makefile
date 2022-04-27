## Author - Emeka Umeozo
#
#CC = gcc
#CFLAGS = -g
#CPPFLAGS = -std=gnu99 -Wall -pedantic
#
#.PHONY: ALL clean
#
#ALL: router
#
#helper.o: router/helper.h router/imports.h
#	$(CC) $(CFLAGS) $(CPPFLAGS) -c helper.c -o helper.o
#
#router: router.o helper.o
#	$(CC) helper.o router.o -o router
#
#router.o: router/router.c router/helper.h
#	$(CC) $(CFLAGS) $(CPPFLAGS) -c router.c -o router.o
#
#clean:
#	rm *.o router
