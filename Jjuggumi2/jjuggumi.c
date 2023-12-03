// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"
#include "canvas.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
// intro()�� �ڵ尡 �� intro.c ���Ͽ�
void ending(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

// �÷��̾� ��ü ȸ�� 40
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
		return -1; // ���� ����, ���� �Լ����� ����
	}

	// �÷��̾� ������ load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i]; // �����ʿ�
		
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

	// ������ ������ laod
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
				printf("�����: %d\n", i);
				break;
			}
		}
	}
	else {
		printf("����ڸ� ������ ���߽��ϴ�.\n");
	}
	printf("\n\n");
	print_status();
}

int main(void) {
	int err = jjuggumi_init();
	if (err == -1) { // ���� ����
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
