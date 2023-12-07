#include <stdio.h>
#include <stdbool.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

void jebi_init(void);
void jebi_reset(void);
void jebi_view(void);
void jebi_display(void);
void jebi_move(key_t key);
void jebi_select(void);

int round = 0;
int turn_player = 0;
int jebi_list[PLAYER_MAX] = { 0 }; // 0: nothing, 1: jebi, 2: fail
int cursor = 0;
int order[PLAYER_MAX] = { 0 };

void jebi_init(void) {
	map_init(5, 32);

	jebi_reset();

	tick = 0;
}

void jebi_reset(void) {
	if (n_alive < 2) {
		return;
	}
	
	int fail = randint(0, n_alive - 1);
	for (int i = 0; i < PLAYER_MAX; i++) {
		jebi_list[i] = i == fail ? 2 : i < n_alive ? 1 : 0;
	}

	jebi_view();
	
	int o = 0;
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive) {
			order[o] = i;
			o++;
		}
	}
	turn_player = 0;
	round++;
}

void jebi_view(void) {
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (jebi_list[i] == 0) {
			back_buf[2][3 * (i + 1)] = ' ';
			break;
		}
		back_buf[2][3 * (i + 1)] = '?';
	}

	back_buf[2][3] = '@';
	cursor = 0;
}

void jebi_display(void) {
	display();

	// print round, turn
	gotoxy(N_ROW + 1, 0);
	printf("round %2d, turn: player %2d\n", round, order[turn_player]);
}

void jebi_move(key_t key) {
	int dir = 0;
	switch (key) {
	case K_LEFT: dir = -1; break;
	case K_RIGHT: dir = 1; break;
	default: return;
	}
	if ((dir == -1 && cursor == 0) || jebi_list[cursor + dir] == 0) {
		return;
	}

	back_buf[2][3 * (cursor + 1)] = '?';
	cursor += dir;
	back_buf[2][3 * (cursor + 1)] = '@';
}

void jebi_select(void) {
	char message[40] = "";
	if (jebi_list[cursor] == 1) {
		sprintf_s(message, 40, "player %d pass!", order[turn_player]);
		// shift
		for (int i = cursor + 1; i < PLAYER_MAX; i++) {
			jebi_list[i - 1] = jebi_list[i];
		}
		jebi_list[PLAYER_MAX - 1] = 0;
		turn_player++;
		jebi_view();
	} else if (jebi_list[cursor] == 2) {
		sprintf_s(message, 40, "player %d fail!", order[turn_player]);
		player[order[turn_player]].is_alive = false;
		n_alive--;
		jebi_reset();
	}
	else {
		return;
	}
	dialog(message);
}

void jebi(void) {
	jebi_init();

	system("cls");
	jebi_display();
	while (1) {
		if (n_alive == 1) {
			break;
		}

		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key == K_SPACE) {
			jebi_select();
		}
		else if (key != K_UNDEFINED) {
			jebi_move(key);
		}

		jebi_display();
		Sleep(10);
		tick += 10;
	}
}