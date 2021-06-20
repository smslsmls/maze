#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<string.h>
#define TITLE_SELECTIONS 3
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

typedef enum {
	TITLE,
	MAP
}sprint;

//변수
//스테이지
int stage_N[] = { 9,9,17,23,35,43,49,55,71,93,201 };
int stage_M[] = { 9,15,33,43,55,71,93,93,93,93,93 };
//방향
int dir = 2;
//맵
int maze[10000][10000];
int visit[10000][10000];
//크기
int maze_N, maze_M;
//좌표
COORD player = { 1,1 };
COORD zero = { 0,0 };
COORD mouse;
COORD pos;
int X[] = { 2,0,-2,0 };
int x[] = { 1,0,-1,0 };
int Y[] = { 0,2,0,-2 };
int y[] = { 0,1,0,-1 };
//타이틀 선택지
char title[TITLE_SELECTIONS + 1][20] = { "미로만들기","첼린지모드","종료" };
int selected;
//시간
clock_t cl;
//마우스 입력
DWORD insize;
//모드
DWORD old_mode;
DWORD new_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;
//핸들
HANDLE h_in, h_out;
//함수 원형
int dfs_maze(COORD, int);
void print(sprint, int);
void make_maze();
void set_NM();
void init();
void CursorView();
void play();
void printp(int, int, int);
void set_out();
int dfs(COORD, COORD);
int select_title();

int main() {
	init();
	selected = select_title();
	if (!selected)
		return 0;
	if (selected == 1) {
		system("cls");
		set_NM();
		make_maze();
		print(MAP, 0);
		cl = clock();
		play();
		set_out();
		printf("%.3lf초", ((double)clock() - (double)cl) / 1000);
	}
	if (selected == 2) {
		cl = clock();
		for (int i = 0; i < 11; i++)
		{
			system("cls");
			maze_N = stage_N[i];
			maze_M = stage_M[i];
			make_maze();
			print(MAP, 0);
			play();
			set_out();
		}
		printf("%.3lf초", ((double)clock() - (double)cl) / 1000);
	}
}

void make_maze() {
	COORD grid;
	for (int i = 0; i < maze_N; i++)
	{
		for (int j = 0; j < maze_M; j++)
		{
			if (i == 0 || i == maze_N - 1 || j == 0 || j == maze_M - 1 || (i % 2 == 0 && j % 2 == 0))
				maze[i][j] = 1;
			else
				maze[i][j] = 0;
		}
	}
	srand((unsigned int)time((NULL)));
	memset(visit, 0, sizeof(visit));
	grid.X = 1;
	grid.Y = 1;
	system("cls");
	dfs_maze(grid, 4);
	player.X = 1;
	player.Y = 1;
}

int dfs_maze(COORD grid, int n) {
	COORD now;
	int in[5] = { 0, };
	int r;
	int re = 1;
	in[n] = 1;
	visit[grid.Y][grid.X] = 1;
	if (grid.X == maze_M - 2 && grid.Y == maze_N - 2)
		return 1;
	if (n == 4) {
		r = rand() % 4;
		in[r] = 1;
		now.X = grid.X + X[r];
		now.Y = grid.Y + Y[r];
		if (!(now.X < 0 || now.Y < 0 || now.X >= maze_M || now.Y >= maze_N))
			dfs_maze(now, (r + 2) % 4);
	}
	for (int i = 0; i < 3; i++)
	{
		r = rand() % 4;
		if (in[r]) {
			i--;
			continue;
		}
		in[r] = 1;
		now.X = grid.X + X[r];
		now.Y = grid.Y + Y[r];
		if (now.X < 0 || now.Y < 0 || now.X >= maze_M || now.Y >= maze_N)
			continue;
		if (visit[now.Y][now.X]) {
			maze[grid.Y + y[r]][grid.X + x[r]] = 1;
			//print(MAP, 0);
			continue;
		}
		re &= dfs_maze(now, (r + 2) % 4);
	}
	return re;
}

int dfs(COORD grid, COORD end) {
	COORD next = grid;
	maze[grid.Y][grid.X] = 4;
	if (grid.Y == end.Y && grid.X == end.X)
		return 1;
	for (int i = 0; i < 4; i++)
	{
		if (maze[grid.Y + y[i]][grid.X + x[i]] != 1 && maze[grid.Y + y[i]][grid.X + x[i]] != 4) {
			next.Y += y[i];
			next.X += x[i];
			if (dfs(next, end))
				return 1;
			next = grid;
		}
	}
	maze[grid.Y][grid.X] = 0;
	return 0;
}

void print(sprint select, int title_select) {
	int len;
	int big = 10;
	SetConsoleCursorPosition(h_out, zero);
	switch (select)
	{
	case TITLE:
		printf(" ");
		for (int i = 0; i < 14; i++)
		{
			printf("-");
		}
		zero.Y++;
		SetConsoleCursorPosition(h_out, zero);
		for (int i = 0; i < TITLE_SELECTIONS; i++)
		{
			len = strlen(title[i]);
			printf("|");
			for (int j = 0; j < (big - len) / 2 + (big - len) % 2; j++)
			{
				printf(" ");
			}
			if (i + 1 == title_select)
				printf("->");
			else
				printf("  ");
			printf("%s", title[i]);
			if (i + 1 == title_select)
				printf("<-");
			else
				printf("  ");
			for (int j = 0; j < (big - len) / 2; j++)
			{
				printf(" ");
			}
			printf("|");
			zero.Y++;
			SetConsoleCursorPosition(h_out, zero);
		}
		printf(" ");
		for (int i = 0; i < 14; i++)
		{
			printf("-");
		}
		break;
	case MAP:
		for (int i = 0; i < maze_N; i++)
		{
			for (int j = 0; j < maze_M; j++)
			{
				printp(i, j, 1);
			}
			zero.Y++;
			SetConsoleCursorPosition(h_out, zero);
		}
		break;
	default:
		break;
	}
	zero.Y = 0;
	SetConsoleTextAttribute(h_out, 7);
}

void set_NM() {
	maze_N = -1;
	maze_M = -1;
	SetConsoleMode(h_in, old_mode);
	while (maze_N < 7 || maze_M < 7 || maze_N % 2 == 0 || maze_M % 2 == 0 || maze_M>93) {
		system("cls");
		printf("미로를 만들 크기를 설정해 주세요.\n가로(93이하)(홀수) : ");
		scanf("%d", &maze_M);
		printf("세로(49이하 권장)(홀수) : ");
		scanf("%d", &maze_N);
	}
	SetConsoleMode(h_in, new_mode);
	pos.Y = maze_N + 2;
}

void init() {
	h_in = GetStdHandle(STD_INPUT_HANDLE);
	h_out = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(h_in, &old_mode);
	CursorView();
	system("mode con | title 미로찾기");
	SetConsoleDisplayMode(h_out, CONSOLE_FULLSCREEN_MODE, 0);
}

int select_title() {
	INPUT_RECORD in_buf[100];
	SetConsoleMode(h_in, new_mode);
	print(TITLE, 0);
	while (1)
	{
		CursorView();
		ReadConsoleInput(h_in, in_buf, 100, &insize);
		for (DWORD i = 0; i < insize; i++)
		{
			switch (in_buf[i].EventType)
			{
			case MOUSE_EVENT:
				switch (in_buf[i].Event.MouseEvent.dwEventFlags)
				{
				case 0:
					if (in_buf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						if (mouse.X < 15 && mouse.X>0) {
							if (mouse.Y == 1)
								return 1;
							else if (mouse.Y == 2)
								return 2;
							else if (mouse.Y == 3)
								return 0;
						}
					}
				case MOUSE_MOVED:
					mouse = in_buf[i].Event.MouseEvent.dwMousePosition;
					if (mouse.X < 15 && mouse.X>0) {
						if (mouse.Y == 1)
							print(TITLE, 1);
						else if (mouse.Y == 2)
							print(TITLE, 2);
						else if (mouse.Y == 3)
							print(TITLE, 3);
						else
							print(TITLE, 0);
					}
					else
						print(TITLE, 0);
				default:
					break;
				}
			}
		}
	}
}

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(h_out, &cursorInfo);
	if (cursorInfo.bVisible) {
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(h_out, &cursorInfo);
	}
}

void play() {
	int ch;
	while (player.Y != maze_N - 2 || player.X != maze_M - 2) {
		CursorView();
		if (_kbhit()) {
			ch = _getch();
			if (ch == 224) {
				ch = _getch();
				player.X *= 2;
				SetConsoleCursorPosition(h_out, player);
				player.X /= 2;
				printp(player.Y, player.X, 0);
				switch (ch)
				{
				case LEFT:
					if (maze[player.Y][player.X - 1] != 1)
						player.X--;
					dir = 0;
					break;
				case RIGHT:
					if (maze[player.Y][player.X + 1] != 1)
						player.X++;
					dir = 1;
					break;
				case UP:
					if (maze[player.Y - 1][player.X] != 1)
						player.Y--;
					dir = 2;
					break;
				case DOWN:
					if (maze[player.Y + 1][player.X] != 1)
						player.Y++;
					dir = 3;
					break;
				default:
					break;
				}
				player.X *= 2;
				SetConsoleCursorPosition(h_out, player);
				player.X /= 2;
				printp(player.Y, player.X, 1);
				SetConsoleCursorPosition(h_out, pos);
				SetConsoleTextAttribute(h_out, 7);
			}
			if (ch == 'd')
				print(MAP, 0);
		}
	}
}

void printp(int i, int j, int n) {
	int back = 0;
	if (maze[i][j] == 0)
		back = 7;
	if (maze[i][j] == 1)
		back = 8;
	if (maze[i][j] == 4)
		back = 10;
	if (i == 1 && j == 1)
		back = 11;
	if (i == maze_N - 2 && j == maze_M - 2)
		back = 12;
	SetConsoleTextAttribute(h_out, back | back << 4);
	if (i == player.Y && j == player.X && n == 1) {
		SetConsoleTextAttribute(h_out, 9 | back << 4);
		if (dir == 0)
			printf("◀");
		if (dir == 1)
			printf("▶");
		if (dir == 2)
			printf("▲");
		if (dir == 3)
			printf("▼");
	}
	else
		printf("■");
}

void set_out() {
	SetConsoleCursorPosition(h_out, pos);
	SetConsoleTextAttribute(h_out, 7);
}