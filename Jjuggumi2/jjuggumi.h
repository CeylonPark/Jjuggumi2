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

	// �ɷ�ġ: ����, ��, ���¹̳�
	int intel, str, stamina;

	// ���� ����
	bool is_alive;	// Ż�������� false
	bool has_item;	// �������� ������ true
	ITEM item;		// ������ 1�� ���� ����
} PLAYER;

ITEM item[ITEM_MAX];
PLAYER player[PLAYER_MAX];

int n_item, n_player, n_alive;
int tick;  // �ð�

int randint(int low, int high);
void heal_stamina();

// ��Ʈ��
void intro(void);

// �̴ϰ���
void sample(void);
void mugunghwa(void);
//void nightgame(void);
void juldarigi(void);
void jebi(void);

#endif
