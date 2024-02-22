#include <iostream>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

using namespace std;

#define screen_width 120
#define screen_height 35
#define win_width 80
#define menu_width 20
#define gap_size 7
#define pipe_dif 40

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipe_Pos[3];
int gap_Pos[3];
int pipe_Flag[3];
char TheBird[2][4] = {'\\', ' ', ' ', '//',
					  ' ', '<o', 'o>', ' ' };
int birdPos = 6;
int score = 0;

//atts in this func is to configure win consol visual. (from windows.h library)
void goXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
	if(size == 0) size = 20;
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void Border()
{
	for (int i = 0; i < screen_width; ++i)
	{
		goXY(i, 0);
		cout << "[]";
		goXY(i, screen_height);
		cout << "[]"; 
	}

	for (int i = 0; i < screen_height; ++i)
	{
		goXY(0, i);
		cout << "[]";
		goXY(screen_width, i);
		cout << "[]";
	}

	for (int i = 0; i < screen_height; ++i)
	{
		goXY(win_width, i);
		cout << "[]";
	}
}

void Pipegeneration(int x)
{
	gap_Pos[x]  = 3 + rand()%14;
}

void PipeDraw (int x)
{
	if (pipe_Flag[x] == true )
	{
		for (int i = 0; i < gap_Pos[x]; ++i)
		{
			goXY(win_width - pipe_Pos[x], i+1);
			cout << "***";
		}

		for (int i = gap_Pos[x] + gap_size; i < screen_height - 1; ++i)
		{
			goXY(win_width - pipe_Pos[x], i+1);
			cout << "***";
		}
	}
}

void erasaPipes(int x)
{
	if (pipe_Flag[x] == true)
	{
		for (int i = 0; i < gap_Pos[x]; ++i)
		{
			goXY(win_width - pipe_Pos[x], i+1);
			cout << "   ";
		}

		for (int i = gap_Pos[x] + gap_size; i < screen_height - 1; ++i)
		{
			goXY(win_width - pipe_Pos[x], i+1);
			cout << "   ";
		}
		
	}
}

void drawBird()
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			goXY(j + 2, i + birdPos);
			cout << TheBird[i][j];
		}
	}
}

void deleteBird()
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			goXY(j + 2, i + birdPos);
			cout << " ";
		}
	}
}

int crash()
{
	if (pipe_Pos[0] >= 61)
	{
		if (birdPos < gap_Pos[0] || birdPos > gap_Pos[0] + gap_size)
		{
			// cout << "HIT";
			// getch();
			return 1;
		}
	}
}

// void errorchecking()
// {
// 	goXY(screen_width + 3, 4);
// 	cout << "pipe position is: " << pipe_Pos[0];
// }

void GameOver()
{
	system("cls");
	cout << endl;
	cout << "\t\t==================" << endl;
	cout << "\t\t====Game Over!====" << endl;
	cout << "\t\t==================" << endl << endl;
	cout << "\t\t===Press any key==";
	getch();
}

void scoreUpdate()
{
	goXY(win_width + 7, 5);
	cout << "Score: " << score << endl;
}

void guide()
{
	system("cls");
	cout << "Instructions" << endl;
	cout << "==========================" << endl;
	cout << "====Spacebar for flying===" << endl;
	cout << "Any key To go back to menu" << endl;
	cout << "==========================" << endl;
	getch();
}

void playTheGame()
{
	birdPos = 6;
	score = 0;
	pipe_Flag[0] = 1;
	pipe_Flag[1] = 0;
	pipe_Pos[0] = pipe_Pos[1] = 4;

	system("cls");
	Border();
	Pipegeneration(0);
	scoreUpdate();

	goXY(win_width + 5, 2); cout << "Flappy Birb";
	goXY(win_width + 6, 4); cout << "-----------";
	goXY(win_width + 6, 6); cout << "-----------";
	goXY(win_width + 7, 12); cout << "--Control: ";
	goXY(win_width + 7, 13); cout << "-----------";
	goXY(win_width + 2, 14); cout << "Spacebar = jump";

	goXY(10, 5); 
	cout << "Press any key to start!";
	getch();
	goXY(10, 5);
	cout << "                       ";

	while(1)
	{
		if (kbhit()) //recent keyboard key check
		{
			char c = getch();
			if (c == 32)
			{
				if (birdPos > 3)
				{
					birdPos -= 3;
				}
			}

			if (c == 27)
			{
				break;
			}
		}

		drawBird();
		PipeDraw(0);
		PipeDraw(1);
		//errorchecking();

		if (crash() == 1)
		{
			GameOver();
			return;
		}

		Sleep(100); //from windows.h
		deleteBird();
		erasaPipes(0);
		erasaPipes(1);
		birdPos += 1;

		if (birdPos > screen_height - 2)
		{
			GameOver();
			return;
		}

		if (pipe_Flag[0] == 1)
		{
			pipe_Pos[0] += 2;
		}

		if (pipe_Flag[1] == 1)
		{
			pipe_Pos[1] += 2;
		}

		if (pipe_Pos[0] >= 40 && pipe_Pos[0] < 42)
		{
			pipe_Flag[1] = 1;
			pipe_Pos[1] = 4;
			Pipegeneration(1);
		}

		if (pipe_Pos[0] > 68)
		{
			score++;
			scoreUpdate();
			pipe_Flag[1] = 0;
			pipe_Pos[0] = pipe_Pos[1];
			gap_Pos[0] = gap_Pos[1];
		}
	}
}

int main()
{
	setcursor(0, 0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		goXY(10,5); cout << " ===================== ";
		goXY(10,6); cout << "|     Flappy Birb    | ";
		goXY(10,7); cout << " ===================== ";
		goXY(10,9); cout << " =====1.start game==== ";
		goXY(10,10); cout << " ===2. Instructions=== ";
		goXY(10,11); cout << " =======3. Quit======= ";
		goXY(10,13); cout << " ====Select Option==== ";
		char op = getche();

		if (op == '1')
		{
			playTheGame();
		}else if (op == '2')
		{
			guide();
		}else if (op == '3')
		{
			exit(0);
		}
	}while(1);

	return 0;
}
















