project: 2048game.o
	cc 2048game.o -o project
2048game.o: 2048game.c 2048game.h
	cc -Wall -c 2048game.c
