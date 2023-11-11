#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIALOG_DURATION_SEC		4

void printxy_kr(char, char, int, int);
void draw(void);
void draw_dialog(void);
void print_status(void);
void clone_buf(char[][COL_MAX], char[][COL_MAX]);

// (zero-base) row행, col열로 커서 이동
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row행, col열에 ch 출력
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

// row행, col열에 ch 출력 - korean 지원
void printxy_kr(char ch1, char ch2, int row, int col) {
	gotoxy(row, col);
	printf("%c%c", ch1, ch2);
}

void map_init(int n_row, int n_col) {
	// 두 버퍼를를 완전히 비우기
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// 대입문 이렇게 쓸 수 있는데 일부러 안 가르쳐줬음
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';
		}
	}
}
// a = b ? 1 : 2
// back_buf[row][col]이 이동할 수 있는 자리인지 확인하는 함수
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// 상단에 맵을, 하단에는 현재 상태를 출력
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	print_status();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

// 전체 맵 크기 버퍼를 draw 시킴, 한글 지원
void draw_dialog(void) {
	for (int row = 0; row < ROW_MAX; row++) {
		for (int col = 0; col < COL_MAX; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				if(front_buf[row][col] < 0) {
					front_buf[row][col + 1] = back_buf[row][col + 1];
					printxy_kr(front_buf[row][col], front_buf[row][col + 1], row, col);
					col++;
				}
				else {
					printxy(front_buf[row][col], row, col);
				}
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d \n", n_alive);
	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s\n", p, player[p] ? "alive" : "DEAD");		
	}
}

// buf를 target_buf로 복사 시킴
void clone_buf(char target_buf[][COL_MAX], char buf[][COL_MAX]) {
	for (int row = 0; row < ROW_MAX; row++) {
		for (int col = 0; col < COL_MAX; col++) {
			target_buf[row][col] = buf[row][col];
		}
	}
}

void dialog(char message[]) {
	// back_buf를 save_buf에 저장
	char save_buf[ROW_MAX][COL_MAX];
	clone_buf(save_buf, back_buf);

	// message[] 길이 구하기
	int length = strlen(message);
	
	// 위치 변수 설정
	int rs = (N_ROW - 3) / 2; // Row Start Point
	int cs = (N_COL - length - 5) / 2; // Column Start Point
	if (rs < 0) {
		rs = 1;
	}
	if (cs < 0) {
		cs = 1;
	}
	int re = rs + 2; // Row End Point
	int ce = cs + length + 5; // Column End Point
	int center = rs + 1; // Row for Time And Message
	if (N_ROW >= 9) {
		rs = (N_ROW - 5) / 2;
		re = rs + 4;
		center = rs + 2;
	}

	// back_buf에 dialog 작성
	for (int row = rs; row <= re; row++) {
		back_buf[row][cs] = back_buf[row][ce] = '*';

		for (int col = cs + 1; col < ce; col++) {
			back_buf[row][col] = (row == rs || row == re) ? '*' : ' ';
		}
	}

	for (int i = 0; i < length; i++) {
		back_buf[center][cs + 4 + i] = message[i];
	};

	// 시간
	int tick_dialog = 0;
	int left = DIALOG_DURATION_SEC;
	while (1) {
		get_key(); // 키 입력 무시
		if (tick_dialog % 1000 == 0) {
			if (left == 0) {
				break;
			}
			back_buf[center][cs + 2] = left + '0';
			draw_dialog();
			left--;
		}
		Sleep(10);
		tick_dialog += 10;
	}
	
	// save_buf를 back_buf에 불러오기
	clone_buf(back_buf, save_buf);
	draw_dialog();
}
