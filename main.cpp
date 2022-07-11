#include<stdio.h>
#include<time.h>
#include<Windows.h>
#include<graphics.h>
#include<stdlib.h>

#define WIDTH 480
#define HEIGHT 640

//板子间隔
#define BOARD_SPACE 100

IMAGE img_board[3];
IMAGE img_mm;

//板子结构
struct Board
{
	int x;
	int y;
	int len;
	int type;
	bool isShow;
};
struct Board board[10];

//玩家
struct Role
{
	int x;
	int y;
	int h;
	int index;
};
struct Role mm;

//定时器
bool Timer(clock_t ms, int id)
{
	//只初始化一次
	static clock_t t[10] = { 0 };

	//程序运行到调用该函数所经过的毫秒数
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}

void gameInit()
{
	//板子背景初始化
	loadimage(img_board, "./img/board1.bmp");
	loadimage(img_board + 1, "./img/board2.bmp");
	loadimage(img_board + 2, "./img/board6.bmp");
	loadimage(&img_mm, "./img/man0.png");

	for (int i = 0; i < 10; i++)
	{
		if (i == 0)
		{
			board[i].y = rand() % (HEIGHT % 3) + 50;
		}
		else
		{
			board[i].y = BOARD_SPACE + board[i - 1].y;
		}
		board[i].x = rand() % (WIDTH - 75);
		board[i].len = 75;
		board[i].isShow = true;
		board[i].type = rand() % 3;
	}

	//游戏开始玩家站在第一块板
	mm.h = 109;
	mm.x = board[0].x + board[0].len / 2 - mm.h / 2;
	mm.y = board[0].y - mm.h;

	mm.index = -1;
}

//游戏绘制
void gameDraw()
{
	for (int i = 0; i < 10; i++)
	{
		/*solidrectangle(board[i].x, board[i].y, board[i].x + board[i].len, board[i].y + 5);*/
		switch (board[i].type)
		{
		case 0:
			putimage(board[i].x, board[i].y, img_board);
			break;
		case 1:
			putimage(board[i].x, board[i].y, img_board + 1);
			break;
		case 2:
			putimage(board[i].x, board[i].y, img_board + 2);
			break;
		}
	}
	putimage(mm.x, mm.y, &img_mm);
}

//板子移动
void boardMove()
{
	for (int i = 0; i < 10; i++)
	{
		board[i].y -= 2;
		if (board[i].y < 0)
		{
			board[i].y = 10 * BOARD_SPACE;
			board[i].x = rand() % (WIDTH - 75);
			board[i].type = rand() % 3;
		}
	}
}

//玩家移动
void mmMove()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		mm.x -= 1;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		mm.x += 1;
	}

	//判断玩家位置
	for (int i = 0; i < 10; i++)
	{
		int x = mm.x + mm.h / 2;
		int y = mm.y + mm.h;
		if (x >= board[i].x && x<board[i].x + board[i].len && y>board[i].y - 10 && y < board[i].y + 10)
		{
			mm.y = board[0].y - mm.h;
			mm.index = i;
			break;
		}
		else
		{
			mm.index = -1;
		}
	}
	if (mm.index == -1)
	{
		mm.y += 5;
	}
}

int main()
{
	initgraph(WIDTH, HEIGHT);
	gameInit();
	while (1)
	{
		BeginBatchDraw();
		cleardevice();
		gameDraw();
		EndBatchDraw();
		if (Timer(50, 0))
		{
			boardMove();
		}
		mmMove();
	}
	getchar();
	return 0;
}