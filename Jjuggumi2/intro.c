#include <stdio.h>
#include "canvas.h"
#include "jjuggumi.h"

void intro(void) {
	char sharon[15][25] = {
		{"        ((   ))\n"},
		{"      \\(\\(   )/)/\n"},
		{"     \\((\\(   )/))/\n"},
		{"    ((\\(\\(( ))/)/))\n"},
		{"     (\\(\\\\\\ ///)/)\n"},
		{"      ((\\\\\\ ///))\n"},
		{"         (| |)         \n"},
		{"          | |          \n"},
		{"          | |          \n"},
		{"          | |          \n"},
		{"          | |          \n"},
		{"*************************"}
	};
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 25; j++) {
			printxy(sharon[i][j], i, j);
		}
	}
	printxy('#', 10, 1);
	Sleep(150);
	printxy(' ', 10, 1);
	printxy('#', 9, 2);
	Sleep(100);
	printxy(' ', 9, 2);
	printxy('#', 10, 3);
	Sleep(150);
	printxy(' ', 10, 3);
	printxy('#', 9, 4);
	Sleep(100);
	printxy(' ', 9, 4);
	printxy('#', 10, 5);
	Sleep(150);
	printxy(' ', 10, 5);
	printxy('#', 9, 6);
	Sleep(100);
	printxy(' ', 9, 6);
	printxy('#', 10, 7);
	Sleep(150);
	printxy(' ', 10, 7);
	printxy(' ', 8, 10);
	printxy('#', 9, 8);
	Sleep(100);
	printxy(' ', 9, 8);
	printxy('#', 8, 9);
	Sleep(150);
	printxy('|', 8, 10);
	printxy(' ', 8, 9);
	Sleep(300);
	for (int i = 0; i < 25; i++) {
		printxy(' ', 11, i);
		printxy('*', 12, i);
		Sleep(15);
		printxy(' ', 12, i);
		printxy('*', 13, i);
		Sleep(15);
		printxy(' ', 13, i);
		printxy('*', 14, i);
		Sleep(15);
		if (i == 11) {
			printxy('#', 11, i);
			printxy(' ', 11, i);
			Sleep(30);
			printxy('#', 12, i);
			printxy(' ', 12, i);
			Sleep(30);
			printxy('#', 13, i);
			Sleep(30);
		}
	}
	printxy('0', 13, 24);
	printxy('p', 11, 22);
	printxy('o', 11, 23);
	printxy('t', 11, 24);
	printxy('a', 11, 25);
	printxy('l', 11, 26);
	for (int i = 11; i < 25; i++) {
		printxy('#', 13, i);
		Sleep(50);
		printxy(' ', 13, i);
	}
}