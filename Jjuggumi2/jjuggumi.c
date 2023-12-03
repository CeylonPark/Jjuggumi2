// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"
#include "canvas.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
// intro()는 코드가 길어서 intro.c 파일에
void ending(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

// 플레이어 전체 회복 40
void heal_stamina() {
	for (int i = 0; i < n_player; i++) {
		int ns = player[i].stamina + 40;
		player[i].stamina = ns > 100 ? 100 : ns;
	}
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // 파일 에러, 메인 함수에서 종료
	}

	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i]; // 수정필요
		
		p->id = i;
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel),
			&(p->str)
		);
		p->stamina = 100; // 100%
		p->is_alive = true;
		p->has_item = false;
	}

	// 아이템 데이터 laod
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		ITEM* ip = &item[i];
		fscanf_s(fp, "%s%d%d%d",
			ip->name, (unsigned int)sizeof(ip->name),
			&(ip->intel_buf),
			&(ip->str_buf),
			&(ip->stamina_buf)
		);
	}
	fclose(fp);

	n_alive = n_player;
	return 0;
}

void ending(void) {
	system("cls");
	printf("\n");
	if (n_alive == 1) {
		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive == true) {
				printf("우승자: %d\n", i);
				break;
			}
		}
	}
	else {
		printf("우승자를 가리지 못했습니다.\n");
	}
	printf("\n\n");
	print_status();
}

int main(void) {
	int err = jjuggumi_init();
	if (err == -1) { // 파일 에러
		return -1;
	}

	// intro();
	sample();
	// mugunghwa();
	
	// TODO
	//nightgame();
	//juldarigi();
	//jebi();
	
	ending();

	return 0;
}
