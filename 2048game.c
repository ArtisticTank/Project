/*
==============================================================================================================================================
 * Name        : 2048.c
 * Author      : Vinay Karande (S.Y.Btech in Computer Engineering from CoEP)
 * Description : Console version of the game "2048" for GNU/Linux

 * Copyright (C) Vinay Karande	 ID : karandevinay@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
==============================================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "2048game.h"

void print(int tiles[SIZE][SIZE]) {
	int x, y;
	printf("\033[H");
	printf("a - Left, w - Up, d - Right, s - Down  and Press Enter or  Ctrl + Z to Quit\n\n");
	printf("Score : %d pts\n\n", score);
	for (y = 0; y < SIZE; y++) {
		printf("\n");
		for (x = 0; x < SIZE; x++) {
			if (tiles[x][y] != 0) {
				char s[8];
				snprintf(s ,8 ,"%u" ,(int)1<<tiles[x][y]);
				int t = 7 - strlen(s);
				printf("%*s%s%*s", t - t/2,"", s, t/2, "");
			} else {
				printf("   -   ");
			}
		}
		printf("\n");
		printf("\n");
	}
	printf("\n");
	printf("\033[A"); // one line up
}

int findTarget(int array[SIZE], int x, int stop) {
	int t;
	// if the position is already on the first, don't evaluate
	if (x == 0) {
		return x;
	}
	for( t = x - 1; t >= 0; t--) {
		if (array[t] != 0) {
			if (array[t] != array[x]) {
				// merge is not possible, take next position
				return t + 1;
			}
			return t;
		} 
		else {
			// we should not slide further, return this one
			if (t == stop) {
				return t;
			}
		}
	}
	return x;
}

int merge(int array[SIZE]) {
	int flag = 0;
	int x ,t , stop = 0;

	for (x = 0; x < SIZE; x++) {
		if (array[x] != 0) {
			t = findTarget(array, x, stop);
			// if target is not original position, then move or merge
			if (t != x) {
				// if target is zero, this is a move
				if (array[t] == 0) {
					array[t] = array[x];
				}
				else if (array[t] == array[x]) {
					// merge (increase power of two)
					array[t]++;
					// increase score
					score += (int)1 << array[t];
					// set stop to avoid double merge
					stop = t + 1;
				}
				array[x] = 0;
				flag = 1;
			}
		}
	}
	return flag;
}

void rotatetiles(int tiles[SIZE][SIZE]) {
	int i, j, n = SIZE;
	int tmp;
	for (i = 0; i < n/2; i++) {
		for (j = i; j < n-i-1; j++) {
			tmp = tiles[i][j];
			tiles[i][j] = tiles[j][n-i-1];
			tiles[j][n-i-1] = tiles[n-i-1][n-j-1];
			tiles[n-i-1][n-j-1] = tiles[n-j-1][i];
			tiles[n-j-1][i] = tmp;
		}
	}
}

int moveUp(int tiles[SIZE][SIZE]) {
	int flag = 0;
	int x;
	for (x = 0; x < SIZE; x++) {
		flag |= merge(tiles[x]);
	}
	return flag;
}

int moveLeft(int tiles[SIZE][SIZE]) {
	int flag;
	rotatetiles(tiles);
	flag = moveUp(tiles);
	rotatetiles(tiles);
	rotatetiles(tiles);
	rotatetiles(tiles);
	return flag;
}

int moveDown(int tiles[SIZE][SIZE]) {
	int flag;
	rotatetiles(tiles);
	rotatetiles(tiles);
	flag = moveUp(tiles);
	rotatetiles(tiles);
	rotatetiles(tiles);
	return flag;
}

int moveRight(int tiles[SIZE][SIZE]) {
	int flag;
	rotatetiles(tiles);
	rotatetiles(tiles);
	rotatetiles(tiles);
	flag = moveUp(tiles);
	rotatetiles(tiles);
	return flag;
}

int findPairDown(int tiles[SIZE][SIZE]) {
	int flag = 0;
	int x, y;
	for (x = 0; x < SIZE; x++) {
		for (y = 0; y < SIZE-1; y++) {
			if (tiles[x][y] == tiles[x][y+1]) 
				return 1;
		}
	}
	return flag;
}

int countEmpty(int tiles[SIZE][SIZE]) {
	int x, y;
	int count = 0;
	for (x = 0;x < SIZE; x++) {
		for (y = 0; y < SIZE; y++) {
			if (tiles[x][y] == 0) {
				count++;
			}
		}
	}
	return count;
}

int gameEnded(int tiles[SIZE][SIZE]) {
	int ended = 1;
	if (countEmpty(tiles) > 0) 
		return 0;
	if (findPairDown(tiles)) 
		return 0;
	rotatetiles(tiles);
	if (findPairDown(tiles)) 
		ended = 0;
	rotatetiles(tiles);
	rotatetiles(tiles);
	rotatetiles(tiles);
	return ended;
}

void addRandom(int tiles[SIZE][SIZE]) {
	static int initialized = 0;
	int x, y, r, len = 0, n, list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = 1;
	}

	for (x = 0;x < SIZE; x++) {
		for (y = 0; y < SIZE; y++) {
			if (tiles[x][y] == 0) {
				list[len][0] = x;
				list[len][1] = y;
				len++;
			}
		}
	}

	if (len > 0) {
		r = rand() % len;
		x = list[r][0];
		y = list[r][1];
		n = (rand() % 10) / 9+1;
		tiles[x][y] = n;
	}
}

void inittiles(int tiles[SIZE][SIZE]) {
	int x, y;
	for (x = 0; x < SIZE; x++) {
		for (y = 0; y < SIZE; y++) {
			tiles[x][y] = 0;
		}
	}
	addRandom(tiles);
	addRandom(tiles);
	print(tiles);
	score = 0;
}

int main() {
	int tiles[SIZE][SIZE];
	char c;
	int flag;
	printf("\033[?25l\033[2J");
	inittiles(tiles);
	while (1) {
		scanf("%c", &c);
		switch(c) {
			case 'a':	// 'a' key
				flag = moveLeft(tiles);  
				break;
			case 'd':	// 'd' key
				flag = moveRight(tiles); 
				break;
			case 'w':	// 'w' key
				flag = moveUp(tiles);    
				break;
			case 's':	// 's' key
				flag = moveDown(tiles);  
				break;
			default: 
				flag = 0;
				break;
		}
		if (flag) {
			print(tiles);
			addRandom(tiles);
			print(tiles);
			if (gameEnded(tiles)) {
				printf("         GAME OVER          \n");
				break;
			}
		}
	}
	printf("\033[?25h\033[m");
	return 0;
}
