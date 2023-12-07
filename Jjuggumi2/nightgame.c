#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"

int rnd_arr1[3], rnd_arr2[3]; // I ������ ���� ����
int rnd1, rnd2,rnd_cnt = 0; 
char save_buf[40][80];
int save_arr[2];
int save_arr2[2];

#define	DATA_FILE	"jjuggumi.dat"
#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
void ngt_buf(char target_buf[ROW_MAX][COL_MAX], char buf[ROW_MAX][COL_MAX]);
void ngt_init(int row, int col);
void ngt_sample(void);
int print_dyn(int selt);
bool init_ngt_able(int row, int col);
bool ngt_able(int row, int col);
void set_item(int row, int col);
void print_item(int nx, int ny, int n);
void ngt_rndm(int player1, int dir);
void ngt_record(int player, int nx, int ny);
void ngt_move_manual(key_t key);
void print_player(int n1, int n2);

void nightgame() {
	ngt_sample();
}

void ngt_init(int row, int col) { // ����
	map_init(row, col);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!init_ngt_able(x, y));
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500);

		back_buf[px[i]][py[i]] = 'I';  // I ����.
		set_item(px[i],px[i]);
	}
	for (int i = 0; i < n_player; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!init_ngt_able(x, y));
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}
	tick = 0;
	
}

void ngt_sample(void) {
	ngt_init(20, 40);
	system("cls");
	display();
	while (1) {
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			ngt_move_manual(key);
		}

		// player 1 ���ʹ� �������� ������(8����)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				ngt_rndm(i, -1); // �������� ������ �� �з�
			}
		}

		display();
		Sleep(10);
		tick += 10;
	}
}

bool init_ngt_able(int row, int col)
{
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ' || back_buf[row][col] == 'I') {
		return false;
	}
	return true;
}

bool ngt_able(int row, int col)
{
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

void set_item(int row, int col) // I�� ������ �Ҵ�
{
	if (rnd_cnt == 0) // ù ��° I�� ���� ������ ����
	{
		rnd1 = rand() % 10; // ������ 1 ����
		rnd_arr1[0] = row;
		rnd_arr1[1] = col;
		rnd_arr1[2] = rnd1;
		rnd_cnt = 1;
	}
	else // �� ��° I�� ���� ������ ����
	{
		do
		{
			rnd2 = rand() % 10; // ������ 2 ����
		} while (rnd1 == rnd2);
		rnd_arr2[0] = row;
		rnd_arr2[1] = col;
		rnd_arr2[2] = rnd2;
	}
}



int print_dyn(int selt)
{
	ngt_buf(save_buf, back_buf);
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';
		}
	}
	draw();
	if (selt == 0) // �������� ��ȯ�� ���
	{
		char g_expl[20] = {"Sucess trade item"};
		for (int i = 0; i < 20; i++)
		{
			back_buf[3][i + 2] = g_expl[i];
			draw();
			Sleep(50);
		}
	}
	else if (selt == 1)
	{
		char g_expl[20] = { "Maintain item" };
		for (int i = 0; i < 18; i++)
		{
			back_buf[3][i + 4] = g_expl[i];
			draw();
			Sleep(50);
		}
	}
	else if (selt == 2)
	{
		char g_expl[20] = { "Get the first item" };
		for (int i = 0; i < 20; i++)
		{
			back_buf[3][i + 2] = g_expl[i];
			draw();
			Sleep(50);
		}
	}
	else if (selt == -1)
	{
		char g_expl[20] = { "Change Your item?" };
		char g_expl2[20] = { "Please press o or x" };
		for (int i = 0; i < 20; i++)
		{
			back_buf[3][i + 2] = g_expl[i];
			draw();
			Sleep(50);
		}
		for (int i = 0; i < 20; i++)
		{
			back_buf[4][i + 2] = g_expl2[i];
			draw();
			Sleep(50);
		}
		int alpha = _getch();
		ngt_buf(back_buf, save_buf);
		display();
		if (alpha == 111)
		{
			return -1;
		}
		else
		{
			return -2;
		}
		
	}
	ngt_buf(back_buf, save_buf);
	display();
}

void print_item(int nx, int ny, int n) { // �� �κ� ����������!!
	// ������ ȹ�� �� ��ȯ �̺�Ʈ ���
	int a;
	PLAYER* p = &player[n];
	if (p->has_item == true)
	{
		if (n == 0) // 0�� �������� �ֿ��� ���
		{
			a = print_dyn(-1);
			if (a == -1)
			{
				print_dyn(0);
				if (rnd_arr1[1] == ny && rnd_arr1[0] == nx)
				{
					p->intel -= item[rnd_arr2[2]].intel_buf;
					p->str -= item[rnd_arr2[2]].str_buf;
					p->stamina -= item[rnd_arr2[2]].stamina_buf;

					p->intel += item[rnd_arr1[2]].intel_buf;
					p->item.intel_buf = item[rnd_arr1[2]].intel_buf - item[rnd_arr2[2]].intel_buf;
					p->str += item[rnd_arr1[2]].str_buf;
					p->item.str_buf = item[rnd_arr1[2]].str_buf - item[rnd_arr2[2]].str_buf;
					p->stamina += item[rnd_arr1[2]].stamina_buf;
					p->item.stamina_buf = item[rnd_arr1[2]].stamina_buf - item[rnd_arr2[2]].stamina_buf;
					rnd_arr1[0] = -1;
					rnd_arr1[1] = -1;
					rnd_arr2[0] = nx;
					rnd_arr2[1] = ny;
					save_arr[0] = n;
					save_arr[1] = rnd_arr1[2];
				}
				else
				{
					p->intel -= item[rnd_arr1[2]].intel_buf;
					p->str -= item[rnd_arr1[2]].str_buf;
					p->stamina -= item[rnd_arr1[2]].stamina_buf;

					p->intel += item[rnd_arr2[2]].intel_buf;
					p->item.intel_buf = item[rnd_arr2[2]].intel_buf - item[rnd_arr1[2]].intel_buf;
					p->str += item[rnd_arr2[2]].str_buf;
					p->item.str_buf = item[rnd_arr2[2]].str_buf - item[rnd_arr1[2]].str_buf;
					p->stamina += item[rnd_arr2[2]].stamina_buf;
					p->item.stamina_buf = item[rnd_arr2[2]].stamina_buf - item[rnd_arr1[2]].stamina_buf;
					rnd_arr2[0] = -1;
					rnd_arr2[1] = -1;
					rnd_arr1[0] = nx;
					rnd_arr1[1] = ny;
					save_arr2[0] = n;
					save_arr2[1] = rnd_arr2[2];
				}
				
			}
			else // �߸� �Է¹޴� ��쵵 ������ ��ȯ ����� ����
			{
				// I�� �����ߴ� ƨ���� ������ �׸�...
				print_dyn(1);
			}
		}
		else
		{
			int ran_int = rand() % 2;
			if (ran_int == 1)
			{
				print_dyn(0);
				if (rnd_arr1[1] == ny && rnd_arr1[0] == nx) // +- ǥ����� ���ľ� ��.
				{
					p->intel += item[rnd_arr1[2]].intel_buf;
					p->item.intel_buf = item[rnd_arr1[2]].intel_buf;
					p->str += item[rnd_arr1[2]].str_buf;
					p->item.str_buf = item[rnd_arr1[2]].str_buf;
					p->stamina += item[rnd_arr1[2]].stamina_buf;
					p->item.stamina_buf = item[rnd_arr1[2]].stamina_buf;
					rnd_arr1[0] = -1;
					rnd_arr1[1] = -1;
					rnd_arr2[0] = nx;
					rnd_arr2[1] = ny;
					save_arr[0] = n;
					save_arr[1] = rnd_arr1[2];
				}
				else
				{
					p->intel += item[rnd_arr2[2]].intel_buf;
					p->item.intel_buf = item[rnd_arr2[2]].intel_buf;
					p->str += item[rnd_arr2[2]].str_buf;
					p->item.str_buf = item[rnd_arr2[2]].str_buf;
					p->stamina += item[rnd_arr2[2]].stamina_buf;
					p->item.stamina_buf = item[rnd_arr2[2]].stamina_buf;
					rnd_arr2[0] = -1;
					rnd_arr2[1] = -1;
					rnd_arr1[0] = nx;
					rnd_arr1[1] = ny;
					save_arr2[0] = n;
					save_arr2[1] = rnd_arr2[2];
				}
			}
			else
			{
				print_dyn(1);
			}
		}

	}
	else if (p->has_item == false)
	{
		print_dyn(2);
		if (rnd_arr1[0] == nx && rnd_arr1[1] == ny) // rnd_arr1[0] == nx && rnd_arr1[1] == ny
		{
			p->intel += item[rnd_arr1[2]].intel_buf;
			p->item.intel_buf = item[rnd_arr1[2]].intel_buf;
			p->str += item[rnd_arr1[2]].str_buf;
			p->item.str_buf = item[rnd_arr1[2]].str_buf;
			p->stamina += item[rnd_arr1[2]].stamina_buf;
			p->item.stamina_buf = item[rnd_arr1[2]].stamina_buf;
			rnd_arr1[0] = -1;
			rnd_arr1[1] = -1;
			save_arr[0] = n;
			save_arr[1] = rnd_arr1[2];
		} 
		else // rnd_arr2[0] == nx && rnd_arr2[1] == ny
		{
			p->intel += item[rnd_arr2[2]].intel_buf;
			p->item.intel_buf = item[rnd_arr2[2]].intel_buf;
			p->str += item[rnd_arr2[2]].str_buf;
			p->item.str_buf = item[rnd_arr2[2]].str_buf;
			p->stamina += item[rnd_arr2[2]].stamina_buf;
			p->item.stamina_buf = item[rnd_arr2[2]].stamina_buf;
			rnd_arr2[0] = -1;
			rnd_arr2[1] = -1;
			save_arr2[0] = n;
			save_arr2[1] = rnd_arr2[2];
		}
		p->has_item = true; // ������ ���� ����
		ngt_record(0, nx, ny);
	}
}

void print_player(int n1,int n2) {
	// �������� ������ ���
	
	ngt_buf(save_buf, back_buf);
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';
		}
	}
	draw();
	int alpha = 0;
	if (n1 == 0 || n2 == 0)
	{
		char g_expl[20] = { "ȸ�� �õ� : p" };
		char g_expl2[20] = { "��Ż �õ� : a" };
		char g_expl3[20] = { "���� : �ƹ� Ű" };
		for (int i = 0; i < 20; i++)
		{
			back_buf[3][i + 2] = g_expl[i];
			draw();
			Sleep(50);
		}
		for (int i = 0; i < 20; i++)
		{
			back_buf[4][i + 2] = g_expl2[i];
			draw();
			Sleep(50);
		}
		for (int i = 0; i < 20; i++)
		{
			back_buf[5][i + 2] = g_expl3[i];
			draw();
			Sleep(50);
		}
		if (n2 == 0)
		{
			int temp = n2;
			n2 = n1;
			n1 = temp;
		}
		alpha = _getch();
	}
	else
	{
		if (save_arr[0] == n1 || save_arr2[0] == n1)
		{
			int rnd_cmd = rand() % 3;
			if (rnd_cmd == 0)
			{
				alpha = 97;
			}
			else if (rnd_cmd == 1)
			{
				alpha == 112;
			}
			else
				alpha = -1;
			char g_expl[9] = { "Behavier:" };
			for (int i = 0; i < 9; i++)
			{
				back_buf[3][i + 2] = g_expl[i];
				draw();
				Sleep(50);
			}
			back_buf[3][12] = n2; // ��Ż��.
			int tmp = 0; // ��Ż��, �ǰ�Ż�� ���� ����
			tmp = n1;
			n1 = n2;
			n2 = tmp;
			draw();
			Sleep(300);
		}
		else if (save_arr[0] == n2 || save_arr2[0] == n2)
		{
			int rnd_cmd = rand() % 3;
			if (rnd_cmd == 0)
			{
				alpha = 97;
			}
			else if (rnd_cmd == 1)
			{
				alpha == 112;
			}
			else
				alpha = -1;
			char g_expl[9] = { "Behavier:" };
			for (int i = 0; i < 9; i++)
			{
				back_buf[3][i + 2] = g_expl[i];
				draw();
				Sleep(50);
			}
			back_buf[3][12] = n1;
			draw();
			Sleep(300);
		}
	}
	PLAYER* p = &player[n1];
	PLAYER* q = &player[n2];
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';
		}
	}
	if (p->stamina >= 0)
	{
		if (alpha == 97) // ȸ�� �õ�
		{
			if (p->intel > q->intel)
			{
				int rnd_int = rand() % 2;
				if (rnd_int == 0)
				{
					// ������ ��Ż
					if (save_arr[0] == n2)
					{
						p->intel += item[save_arr[1]].intel_buf;
						p->str += item[save_arr[1]].str_buf;
						p->stamina += item[save_arr[1]].stamina_buf;
						q->intel -= item[save_arr[1]].intel_buf;
						q->str -= item[save_arr[1]].str_buf;
						q->stamina -= item[save_arr[1]].stamina_buf;
						save_arr2[0] = n1;
						q->has_item = false;
					}
					else if (save_arr2[0] == n2)
					{
						p->intel += item[save_arr2[1]].intel_buf;
						p->str += item[save_arr2[1]].str_buf;
						p->stamina += item[save_arr2[1]].stamina_buf;
						q->intel -= item[save_arr2[1]].intel_buf;
						q->str -= item[save_arr2[1]].str_buf;
						q->stamina -= item[save_arr2[1]].stamina_buf;
						save_arr2[0] = n1;
						q->has_item = false;
					}
					char expl1[20] = {"��Ż ����"};
					for (int i = 0; i < 20; i++)
					{
						back_buf[3][i + 2] = expl1[i];
						draw();
						Sleep(50);
					}
				}
				else
				{
					// ������ ��ȯ(�ִٸ�)
					if (save_arr[0] == n2 && save_arr2[0] == n1)
					{
						p->intel = p->intel + item[save_arr[1]].intel_buf- item[save_arr2[1]].intel_buf;
						p->str = p->str + item[save_arr[1]].str_buf - item[save_arr2[1]].str_buf;
						p->stamina = p->stamina + item[save_arr[1]].str_buf - item[save_arr2[1]].str_buf;
						q->intel = q->intel - item[save_arr[1]].intel_buf + item[save_arr2[1]].intel_buf;
						q->str = q->str - item[save_arr[1]].str_buf + item[save_arr2[1]].str_buf;
						q->stamina = q->stamina - item[save_arr[1]].stamina_buf + item[save_arr2[1]].stamina_buf;
						save_arr[0] = n1;
						save_arr[1] = n2;
					}
					else if (save_arr2[0] == n2 && save_arr[0] == n1)
					{
						q->intel = q->intel + item[save_arr[1]].intel_buf - item[save_arr2[1]].intel_buf;
						q->str = q->str + item[save_arr[1]].str_buf - item[save_arr2[1]].str_buf;
						q->stamina = q->stamina + item[save_arr[1]].stamina_buf - item[save_arr2[1]].stamina_buf;
						p->intel = p->intel - item[save_arr[1]].intel_buf + item[save_arr2[1]].intel_buf;
						p->str = p->str - item[save_arr[1]].str_buf + item[save_arr2[1]].str_buf;
						p->stamina = p->stamina - item[save_arr[1]].stamina_buf + item[save_arr2[1]].stamina_buf;
						save_arr[0] = n2;
						save_arr[1] = n1;
					}
					char expl2[20] = { "��ȯ ����" };
					for (int i = 0; i < 20; i++)
					{
						back_buf[3][i + 2] = expl2[i];
						draw();
						Sleep(50);
					}
				}
				p->stamina -= 20;
			}
			else
			{
				char expl6[20] = { "�����ߴ�." };
				for (int i = 0; i < 20; i++)
				{
					back_buf[3][i + 2] = expl6[i];
					draw();
					Sleep(50);
				}
				p->stamina -= 40;
			}
		}
		else if (alpha == 112) // ��Ż �õ�
		{
			if (p->str > q->str)
			{
				int rnd_int = rand() % 2;
				if (rnd_int == 0)
				{
					// ������ ��Ż
					if (save_arr[0] == n2)
					{
						p->intel += item[save_arr[1]].intel_buf;
						p->str += item[save_arr[1]].str_buf;
						p->stamina += item[save_arr[1]].stamina_buf;
						q->intel -= item[save_arr[1]].intel_buf;
						q->str -= item[save_arr[1]].str_buf;
						q->stamina -= item[save_arr[1]].stamina_buf;
						save_arr2[0] = n1;
						q->has_item = false;
					}
					else if (save_arr2[0] == n2)
					{
						p->intel += item[save_arr2[1]].intel_buf;
						p->str += item[save_arr2[1]].str_buf;
						p->stamina += item[save_arr2[1]].stamina_buf;
						q->intel -= item[save_arr2[1]].intel_buf;
						q->str -= item[save_arr2[1]].str_buf;
						q->stamina -= item[save_arr2[1]].stamina_buf;
						save_arr2[0] = n1;
						q->has_item = false;
					}
					char expl3[20] = { "��Ż ����" };
					for (int i = 0; i < 20; i++)
					{
						back_buf[3][i + 2] = expl3[i];
						draw();
						Sleep(50);
					}
				}
				else
				{
					// ������ ��ȯ(�ִٸ�)
					if (save_arr[0] == n2)
					{
						p->intel = p->intel + item[save_arr[1]].intel_buf - item[save_arr2[1]].intel_buf;
						p->str = p->str + item[save_arr[1]].str_buf - item[save_arr2[1]].str_buf;
						p->stamina = p->stamina + item[save_arr[1]].stamina_buf - item[save_arr2[1]].stamina_buf;
						q->intel = q->intel - item[save_arr[1]].intel_buf + item[save_arr2[1]].intel_buf;
						q->str = q->str - item[save_arr[1]].str_buf + item[save_arr2[1]].str_buf;
						q->stamina = q->stamina - item[save_arr[1]].stamina_buf + item[save_arr2[1]].stamina_buf;
						save_arr[0] = n1;
						save_arr[1] = n2;
					}
					else if (save_arr2[0] == n2)
					{
						q->intel = q->intel + item[save_arr[1]].intel_buf - item[save_arr2[1]].intel_buf;
						q->str = q->str + item[save_arr[1]].str_buf - item[save_arr2[1]].str_buf;
						q->stamina = q->stamina + item[save_arr[1]].stamina_buf - item[save_arr2[1]].stamina_buf;
						p->intel = p->intel - item[save_arr[1]].intel_buf + item[save_arr2[1]].intel_buf;
						p->str = p->str - item[save_arr[1]].str_buf + item[save_arr2[1]].str_buf;
						p->stamina = p->stamina - item[save_arr[1]].stamina_buf + item[save_arr2[1]].stamina_buf;
						save_arr[0] = n2;
						save_arr[1] = n1;
					}
					char expl4[20] = { "��ȯ ����" };
					for (int i = 0; i < 20; i++)
					{
						back_buf[3][i + 2] = expl4[i];
						draw();
						Sleep(50);
					}
				}
				p->stamina -= 40;
			}
			else
			{
				char expl5[20] = { "�����ߴ�." };
				for (int i = 0; i < 20; i++)
				{
					back_buf[3][i + 2] = expl5[i];
					draw();
					Sleep(50);
				}
				p->stamina -= 60;
			}
		}
	}
	
	ngt_buf(back_buf, save_buf);
	display();
}


void ngt_record(int player, int nx, int ny) { // I�� ������ �ٸ� �������� ���� I�� ���̰ų� I�� �÷��̾ ȹ���ϵ��� ����
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void ngt_rndm(int player1, int dir) {
	PLAYER* q = &player[player1];
	int p = player1;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	
	do
	{
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
		
	} while (back_buf[nx][ny] == '#');
	if (back_buf[nx][ny] == 'I')
	{
		print_item(nx, ny, p);
	}
	else if (back_buf[nx][ny] != ' ' && back_buf[nx][ny] != 'I' && back_buf[nx][ny] != '#')
	{
		print_player(p, back_buf[nx][ny]);
	}
	else
	{
		ngt_record(p, nx, ny);
	}
	// print_player ȣ���ؾߵ�.

	
}

void ngt_move_manual(key_t key) { // only for 0
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!ngt_able(nx, ny)) { // �������� ���ϴ� ��� )) �������� ������ ��, �������� ������ �� ����� �� ������ ��.
		if (back_buf[nx][ny] == 'I') // 0�� ������ ��ġ�� �������� ���
		{
			print_item(nx,ny,0);
		}
		else if (back_buf[nx][ny] != ' ' && back_buf[nx][ny] != 'I' && back_buf[nx][ny] != '#') // 0�� �ٸ� �÷��̾�� �������� ���, �� �κ� �����۵��ϵ��� ����
		{
			if (back_buf[nx][ny] == 1) 
			{
				print_player(0, 1);
			}
			else if (back_buf[nx][ny] == 2)
			{
				print_player(0, 2);
			}
			
		}
		return;
	}
	else
		ngt_record(0, nx, ny);
}


void ngt_buf(char target_buf[ROW_MAX][COL_MAX], char buf[ROW_MAX][COL_MAX]) {
	for (int row = 0; row < ROW_MAX; row++) {
		for (int col = 0; col < COL_MAX; col++) {
			target_buf[row][col] = buf[row][col];
		}
	}
}
