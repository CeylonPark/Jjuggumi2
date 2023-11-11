// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
// intro()�� �ڵ尡 �� intro.c ���Ͽ�
void ending(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("�÷��̾� ��: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i] = true;
	}
	return 0;
}

void ending(void) {
	system("cls");
	printf("\n");
	if (n_alive > 1) {
		printf("����ڸ� ������ ���߽��ϴ�.\n");
	}
	else if (n_alive == 0) {
		printf("����ڸ� ������ ���߽��ϴ�.\n");
	}
	else if (n_alive == 1) {
		bool state = false;
		for (int i = 0; i < n_player; i++) {
			if (player[i] == true) {
				printf("�����: %d\n", i);
				break;
			}
		}
	}
	printf("\n");
	printf("\n");
	printf("no. of players left: %d\n", n_alive);
	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s\n", p, player[p] ? "alive" : "DEAD");
	}
}

int main(void) {
	jjuggumi_init();
	intro();
	sample();
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	ending();

	return 0;
}
