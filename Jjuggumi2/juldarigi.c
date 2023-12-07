#include <stdio.h>
#include <stdbool.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define J_RUN 0
#define J_WAIT 1
#define J_DONE 2

int state_juldarigi = J_RUN;
bool revival_player[PLAYER_MAX] = { 0 }; // 탈락했던 플레이어들
bool dead_juldarigi[PLAYER_MAX] = { 0 };
int str = 0; // 힘!
bool skill_juldarigi[2] = { 0 };

void juldarigi_init(void);
void juldarigi_calculate_str(void);
void juldarigi_display(void);
void juldarigi_skill(int team);
void juldarigi_shift(int start, int end, int shift);
void juldarigi_dead(int id);
void juldarigi_turn(void);

void juldarigi_init(void) {
	map_init(3, 31);

	back_buf[0][15] = back_buf[2][15] = ' ';
	back_buf[1][14] = back_buf[1][15] = back_buf[1][16] = '-';

	int left = 13, right = 17;
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == 0) {
			back_buf[1][left] = i + '0';
			left--;
		}
		else {
			back_buf[1][right] = i + '0';
			right++;
		}
		
		if (!player[i].is_alive) {
			revival_player[i] = true;
			player[i].has_item = false; // 탈락한 플레이어는 아이템 몰수
			player[i].is_alive = true;
		}
	}

	juldarigi_calculate_str();
	tick = 0;
}

void juldarigi_calculate_str(void) {
	int left_str = 0, right_str = 0;
	for (int i = 0; i < n_player; i++) {
		if (!player[i].is_alive) {
			continue;
		}
		int* strp = i % 2 == 0 ? &left_str : &right_str;
		*strp += player[i].str;
		if (player[i].has_item) {
			*strp += player[i].item.str_buf;
		}
	}
	str = right_str - left_str;
}

void juldarigi_display(void) {
	display();

	// print str, skill
	gotoxy(N_ROW + 2, 0);
	printf("str: %5d", str);
	for (int i = 0; i < 2; i++) {
		if (skill_juldarigi[i]) {
			printf(", %s팀 눕기!", i == 0 ? "왼쪽" : "오른쪽");
		}
	}
	printf("                              \n");

	// display revival player
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive == true && revival_player[i]) {
			printxy('*', N_ROW + 6 + i, 16);
		}
	}
}

void juldarigi_skill(int team) {
	if (state_juldarigi != J_RUN || (team != 0 && team != 1) || skill_juldarigi[team]) {
		return;
	}

	// 스태미너 확인
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == team && player[i].stamina < 30) {
			return;
		}
	}

	// 스태미너 소모
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == team) {
			player[i].stamina -= 30;
		}
	}

	skill_juldarigi[team] = true;
}

void juldarigi_shift(int start, int end, int shift) {
	char save_buf[COL_MAX] = { 0 };
	memcpy_s(save_buf, COL_MAX, back_buf[1], COL_MAX);
	for (int col = start; col <= end; col++) {
		int dc = col + shift;
		if (dc < 1 || dc > 29) {
			continue;
		}
		back_buf[1][dc] = save_buf[col];
	}
}

void juldarigi_dead(int id) {
	dead_juldarigi[id] = true;

	PLAYER* p = &player[id];
	if (revival_player[id] == true) {
		p->is_alive = false;
	}
	else {
		p->has_item = false;
		p->intel /= 2;
		p->str /= 2;
	}
}

void juldarigi_turn(void) {
	if (str == 0) {
		return;
	}
	
	int team = str > 0;
	int shift = str > 0 ? 1 : -1;
	for (int i = 0; i < 1 + skill_juldarigi[team]; i++) {
		juldarigi_shift(1, 29, shift);

		if (back_buf[1][15] == '-') {
			continue;
		}

		for (int i = 0; i < n_player; i++) {
			if (back_buf[1][15] == i + '0') {
				juldarigi_dead(i);
				break;
			}
		}

		if (back_buf[1][14] == ' ' || back_buf[1][16] == ' ') {
			state_juldarigi = J_DONE;
		}

		if (shift == 1) { // if dead left
			juldarigi_shift(16, 29, -1);
		}
		else { // if dead right
			juldarigi_shift(1, 14, 1);
		}
	}
}

void juldarigi(void) {
	juldarigi_init();

	system("cls");
	juldarigi_display();
	
	// dialog("초 후 게임이 시작됩니다!");

	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key == K_Z || key == K_SLASH) {
			if (state_juldarigi == J_RUN) {
				str += key == K_Z ? -1 : 1;
			}
		}
		else if (key == K_X || key == K_DOT) {
			juldarigi_skill(key == K_X ? 0 : 1);
		}
		
		if (tick % 1000 == 0) {
			if (state_juldarigi == J_RUN) {
				state_juldarigi = J_WAIT;
				juldarigi_turn();
			}
			else if (state_juldarigi == J_WAIT || state_juldarigi == J_DONE) {
				dialog_dead(dead_juldarigi);
				memset(dead_juldarigi, 0, PLAYER_MAX);
				juldarigi_calculate_str();
				skill_juldarigi[0] = skill_juldarigi[1] = false;
				if (state_juldarigi == J_DONE) {
					break;
				}
				state_juldarigi = J_RUN;
			}
		}

		juldarigi_display();
		Sleep(10);
		tick += 10;
	}

	heal_stamina();
}