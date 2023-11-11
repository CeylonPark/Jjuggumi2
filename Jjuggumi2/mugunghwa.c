#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

#define G_SAY		0
#define G_TURN		1
#define G_WAIT		2

void mugunghwa_init(void);
void move_manual_mugunghwa(key_t);
void move_random_mugunghwa(int);
void move_player_mugunghwa(int, int);
void move_tail_mugunghwa(int, int, int);
void say_mugunghwa(void);
void turn_mugunghwa(void);
void print_dead_player(void);
void delete_dead_player(void);
bool is_hidden(int, int);
bool is_arrived(int);

bool state = false; // ���� ����, true: ���� ��, false: ���� ����
int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �� �÷��̾� ��ġ, �̵� �ֱ�
int n_left; // ���ӿ� �����ִ� �÷��̾� ��
bool cash_dead[PLAYER_MAX] = { 0 }; // Red Light �� ���� �÷��̾�
int turn = G_SAY; // G_SAY: ����ȭ ���ϱ�, G_TURN: ��, G_WAIT: 3�� ��ٸ���
int progress = 0; // ����ȭ �޽��� ���൵

void mugunghwa_init(void) {
	map_init(15, 40);

	back_buf[6][1] = back_buf[7][1] = back_buf[8][1] = '#';

	int a = (14 - n_alive) / 2;
	int order = a;
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			px[i] = order++;
			py[i] = 38;
			period[i] = randint(100, 500);
			back_buf[px[i]][py[i]] = i + '0';
		}
	}
	n_left = n_alive;

	tick = 0;
}

void move_manual_mugunghwa(key_t key) {
	if (player[0] == false || (px[0] == 0 && py[0] == 0)) {
		return;
	}

	int dir = 0; // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	}

	move_player_mugunghwa(0, dir);
}

void move_random_mugunghwa(int p) { // p: player
	if (player[p] == false || (px[p] == 0 && py[p] == 0)) {
		return;
	}

	int dir = 0;
	int randnum = randint(1, 100);
	
	// 10% Ȯ���� �����̱�
	if (turn != G_SAY && randnum > 10) {
		return;
	}

	if (randnum <= 70) { // 70% ��
		dir = 2;
	}
	else if (randnum <= 80) { // 10% ��
		dir = 0;
	}
	else if (randnum <= 90) { // 10% ��
		dir = 1;
	}
	else { // 10% ���ڸ�
		dir = -1;
	}
	move_player_mugunghwa(p, dir);
}

void move_player_mugunghwa(int p, int dir) {
	if (dir > 3 || dir < 0) {
		return;
	}

	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[p] + dx[dir];
	ny = py[p] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail_mugunghwa(p, nx, ny);

	// Red Light�� ������ ��
	if (turn == G_WAIT && (!is_hidden(px[p], py[p]) || !is_hidden(nx - dx[dir], ny - dx[dir]))) {
		player[p] = false;
		n_alive--;
		n_left--;
		cash_dead[p] = true;
		return;
	}

	// ������ ��
	if (is_arrived(p)) {
		back_buf[px[p]][py[p]] = ' ';
		px[p] = py[p] = 0;
		n_left--;
		if (n_left == 0) {
			state = false;
		}
	}
}

// back_buf[][]�� ���
void move_tail_mugunghwa(int player, int nx, int ny) {
	int p = player;
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

// ����ȭ �޽��� ���
void say_mugunghwa() {
	static char s[] = "����ȭ ���� �Ǿ����ϴ�.";
	static int n_s = 23;
	static int t[24] = { 1, 1, 800, 1, 800, 1, 1, 1000, 1, 1000, 1, 1, 200, 1, 200, 1, 200, 1, 200, 1, 200, 1, 100 };

	if (turn == G_SAY && tick % t[progress] == 0) {
		if (progress == 0) {
			tick = 0;
			gotoxy(N_ROW + 1, progress);
			for (int i = 0; i < n_s - 1; i++) {
				printf(" ");
			}
		}

		gotoxy(N_ROW + 1, progress);
		if (s[progress] < 0) {
			printf("%c%c", s[progress], s[progress + 1]);;
			progress += 2;
		}
		else {
			printf("%c", s[progress]);
			progress += 1;
		}

		if (progress > n_s - 1) {
			progress = 0;
			turn = G_TURN;
		}
	}
}

void turn_mugunghwa(void) {
	if (turn == G_TURN) {
		back_buf[6][1] = back_buf[7][1] = back_buf[8][1] = '@';
		turn = G_WAIT;
	}
	else if (turn == G_WAIT && tick % 3000 == 0) {
		print_dead_player();
		delete_dead_player();
		if (n_alive < 2 || n_left == 0) {
			state = false;
		}
		back_buf[6][1] = back_buf[7][1] = back_buf[8][1] = '#';
		turn = G_SAY;
	}
}

void print_dead_player(void) {
	char message[40] = "player ";
	int j = 7;
	for (int i = 0; i < n_player; i++) {
		if (cash_dead[i] == true && player[i] == false) {
			if (j == 7) {
				message[j++] = i + '0';
			}
			else {
				message[j++] = ',';
				message[j++] = ' ';
				message[j++] = i + '0';
			}
		}
	}
	message[j++] = ' ';
	message[j++] = 'd';
	message[j++] = 'e';
	message[j++] = 'a';
	message[j++] = 'd';
	message[j++] = '!';
	
	if (j != 13) {
		dialog(message);
	}
}

// ���� �÷��̾� ������� �ϱ�
void delete_dead_player(void) {
	for (int i = 0; i < n_player; i++) {
		if (cash_dead[i] == true) {
			back_buf[px[i]][py[i]] = ' ';
			px[i] = py[i] = 0;
			cash_dead[i] = false;
		}
	}
}

bool is_hidden(int px, int py) {
	for (int col = 1; col < py; col++) {
		if (back_buf[px][col] != '@' && back_buf[px][col] != ' ') {
			return true;
		}
	}
	return false;
}

bool is_arrived(int player) {
	for (int row = 1; row <= 13; row++) {
		int col = row == 6 || row == 7 || row == 8 ? 2 : 1;
		if (px[player] == row && py[player] == col) {
			return true;
		}
	}
	return false;
}

void mugunghwa(void) {
	mugunghwa_init();
	
	system("cls");
	display();
	dialog("�� �� ������ ���۵˴ϴ�!");

	state = true;
	while (1) {
		if (state == false) {
			break;
		}

		if (turn == G_SAY) {
			say_mugunghwa();
		}
		else {
			turn_mugunghwa();
		}

		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual_mugunghwa(key);
		}

		// �ٸ� �÷��̾� �������� ������
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				move_random_mugunghwa(i);
			}
		}

		display();
		Sleep(10);
		tick += 10;
	}
}