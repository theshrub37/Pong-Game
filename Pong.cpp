#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <ctime>
using namespace std;

char border = '*';

void HideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

bool gameOver = false;
const int width = 50;
const int height = 30;
int pos1 = height / 2;
int pos2 = height / 2;
int radius = 2;

class Score
{
public:
    int score1 = 0;
    int score2 = 0;
    int print()
    {
        cout << "Player 1 Score: " << score1 << endl;
        cout << "Player 2 Score: " << score2 << endl;
    }
};
Score score;

class Ball
{
public:
    int x, y;
    int dx, dy;
    int ox, oy;
    Ball(int bx, int by, int bdx, int bdy)
    {
        x = bx;
        y = by;
        ox = bx;
        oy = by;
        dx = bdx;
        dy = bdy;
    }
    void move()
    {
        if (y + dy <= 0 || y + dy >= height)
        {
            dy = -dy;
        }
        if ((x + dx <= 0 && y + dy >= pos1 - radius - 1 && y + dy <= pos1 + radius + 1) || (x + dx >= width && y + dy >= pos2 - radius && y + dy <= pos2 + radius))
        {
            dx = -dx;
        }
        if (x < 0)
        {
            respawn(1);
        }
        else if (x > width)
        {
            respawn(2);
        }
        x += dx;
        y += dy;
    }
    void respawn(int player)
    {
        if (player == 1)
            score.score2++;
        if (player == 2)
            score.score1++;
        Sleep(500);
        x = ox;
        y = oy;
        pos1 = height / 2;
        pos2 = height / 2;
    }
};

class Position
{
public:
    void drawBoard(Ball ball)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == ball.y && j == ball.x)
                {
                    cout << "#";
                }
                else if (((pos1 >= i - radius && pos1 <= i + radius) && j == 0) ||
                         ((pos2 >= i - radius && pos2 <= i + radius) && j == width - 1))
                {
                    cout << "|";
                }
                else if (i == 0 || i == height - 1)
                    cout << "*";
                else
                    cout << " ";
            }
            cout << endl;
        }
    }
};

void checkEndGame()
{
    if (score.score1 == 11)
    {
        system("cls");
        cout << "Player 1 Wins!";
        gameOver = true;
    }
    else if (score.score2 == 11)
    {
        system("cls");
        cout << "Player 2 Wins!";
        gameOver = true;
    }
}

enum eDirection
{
    STOP = 0,
    UP,
    DOWN
};
eDirection dir1;
eDirection dir2;

int randDir()
{
    return (rand() % 2) == 0 ? -1 : 1;
}

void Setup()
{
    gameOver = false;
    dir1 = STOP;
    dir2 = STOP;
}
void Draw(Ball ball)
{
    system("cls");

    Position obj;
    obj.drawBoard(ball);
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            dir1 = UP;
            break;
        case 's':
            dir1 = DOWN;
            break;
        case 'i':
            dir2 = UP;
            break;
        case 'k':
            dir2 = DOWN;
            break;
        case 'q':
            gameOver = true;
            break;
        }
    }
    else
    {
        dir1 = STOP;
        dir2 = STOP;
    }
}

void Logic()
{
    switch (dir1)
    {
    case UP:
        if (pos1 - radius - 1 > 0)
            pos1--;
        break;
    case DOWN:
        if (pos1 + radius + 2 < height)
            pos1++;
        break;
    default:
        break;
    }
    switch (dir2)
    {
    case UP:
        if (pos2 - radius - 1 > 0)
            pos2--;
        break;
    case DOWN:
        if (pos2 + radius + 2 < height)
            pos2++;
        break;
    default:
        break;
    }
}

int main()
{
    srand(time(0));
    Setup();
    HideCursor();
    int inc = 0;
    Ball ball(width / 2, height / 2, randDir() * 2, randDir());
    while (!gameOver)
    {
        Draw(ball);
        Input();
        Logic();
        if (inc % 2 == 1)
            ball.move();
        score.print();
        inc++;
        checkEndGame();
    }
}
