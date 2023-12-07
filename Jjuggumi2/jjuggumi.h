#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define ITEM_MAX		10
#define PLAYER_MAX		10

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;

typedef struct {
	int id;
	char name[100];

	// 능력치: 지능, 힘, 스태미나
	int intel, str, stamina;

	// 현재 상태
	bool is_alive;	// 탈락했으면 false
	bool has_item;	// 아이템이 있으면 true
	ITEM item;		// 아이템 1개 장착 가능
} PLAYER;

ITEM item[ITEM_MAX];
PLAYER player[PLAYER_MAX];

int n_item, n_player, n_alive;
int tick;  // 시계

int randint(int low, int high);
void heal_stamina();

// 인트로
void intro(void);

// 미니게임
void sample(void);
void mugunghwa(void);
//void nightgame(void);
void juldarigi(void);
void jebi(void);

#endif
