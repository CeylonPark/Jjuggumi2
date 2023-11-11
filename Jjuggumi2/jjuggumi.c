// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
// intro()는 코드가 길어서 intro.c 파일에
void ending(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("플레이어 수: ");
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
		printf("우승자를 가리지 못했습니다.\n");
	}
	else if (n_alive == 0) {
		printf("우승자를 가리지 못했습니다.\n");
	}
	else if (n_alive == 1) {
		bool state = false;
		for (int i = 0; i < n_player; i++) {
			if (player[i] == true) {
				printf("우승자: %d\n", i);
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
