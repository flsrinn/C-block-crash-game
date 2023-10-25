#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <process.h>
#pragma comment(lib,"winmm.lib")

#define BLACK   0
#define BLUE1   1
#define GREEN1   2
#define CYAN1   3
#define RED1   4
#define MAGENTA1 5
#define YELLOW1   6
#define GRAY1   7
#define GRAY2   8
#define BLUE2   9
#define GREEN2   10
#define CYAN2   11
#define RED2   12
#define MAGENTA2 13
#define YELLOW2   14
#define WHITE   15

#define WIDTH 68
#define HEIGHT 48

#define LEFT 0x4b
#define RIGHT 0x4d
#define ESC 0x1b
#define SPACEBAR ' '

#define LEFT2 'a'
#define RIGHT2 'd'

typedef enum DIRECT {
    LEFT_TOP, LEFT_DOWN, RIGHT_DOWN, RIGHT_TOP
} DIRECT;

typedef struct Ball {
    int X;
    int Y;
    int last_X;
    int last_Y;
    DIRECT ball_direct;
}Ball;

typedef struct Paddle {
    int X[8];
    int Y;
}Paddle;

int ball_moving = 0;
int score = 0;

int num = 0;
int life;
int map[WIDTH][HEIGHT];
int color[HEIGHT];

Ball b;
Paddle p;

void gotoxy(double x, double y)
{
    COORD pos; // Windows.h 에 정의
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotoxy2(COORD pos)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void cls(int text_color, int bg_color)
{
    char cmd[100];
    system("cls");
    sprintf(cmd, "COLOR %x%x", bg_color, text_color);
    system(cmd);
}

void draw_box(int x1, int y1, int x2, int y2, char ch)
{
    int x, y;
    for (x = x1; x <= x2; x++) {
        gotoxy(x, y1);
        printf("%c", ch);
    }
    for (x = x1; x <= x2; x++) {
        gotoxy(x, y2);
        printf("%c", ch);
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x1, y);
        printf("%c", ch);
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x2, y);
        printf("%c", ch);
    }
}

void draw_box2(int x1, int y1, int x2, int y2)
{
    int x, y;
    for (x = x1; x <= x2; x += 2) {
        gotoxy(x, y1);
        printf("%s", "─");
    }
    for (x = x1; x <= x2; x += 2) {
        gotoxy(x, y2);
        printf("%s", "─");
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x1, y);
        printf("%s", "│");
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x2, y);
        printf("%s", "│");
    }
    gotoxy(x1, y1); printf("┌");
    gotoxy(x2, y1); printf("┐");
    gotoxy(x1, y2); printf("└");
    gotoxy(x2, y2); printf("┘");
}

void delete_block(int x, int y) {
    int i, j;
    score += 10;
    gotoxy(x, y);
    printf("        ");
    gotoxy(x, y + 1);
    printf("        ");
    gotoxy(x, y + 2);
    printf("        ");
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 3; j++)
        {
            map[x + i][y + j] = 0;
        }
    }
    check_clear();
}

void draw_block(int n, int x, int y) {
    textcolor(color[y], BLACK);
    if (n / 10 == 0) {
        gotoxy(x, y);
        printf("┌──┐");
        gotoxy(x, y + 1);
        printf("│ %d  │", n);
        gotoxy(x, y + 2);
        printf("└──┘");
    }
    else {
        gotoxy(x, y);
        printf("┌──┐");
        gotoxy(x, y + 1);
        printf("│ %d │", n);
        gotoxy(x, y + 2);
        printf("└──┘");
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            map[x + i][y + j] = n;
        }
    }
}

void draw_stage(int n) {
    cls(WHITE, BLACK);
    srand(time(NULL));
    int i, j, r, num, last_color;
    last_color = 1;
    num = 1;
    switch (n) {
    case 1:
        life = 5;
        for (j = 6; j <= 9; j += 3)
        {
            color[j] = rand() % 13 + 1;
            if (color[j] == last_color) {
                continue;
            }
            else {
                last_color = color[j];
            }
            textcolor(color[j], BLACK);
            
            for (i = 2; i < WIDTH - 4; i += 8)
            {
                r = rand() % 100;
                if (j == 6) {
                    num = rand() % 3 + 1;
                    if (r < 70) {
                        draw_block(num, i, j);
                    }
                }
                else {
                    num = 1;
                    if (r < 60) {
                        draw_block(num, i, j);
                    }
                }
            }
        }
        break;
    case 2:
        life = 5;
        for (j = 12; j >= 6; j -= 3)
        {
            color[j] = rand() % 13 + 1;
            if (color[j] == last_color) {
                continue;
            }
            else {
                last_color = color[j];
            }
            textcolor(color[j], BLACK);
            for (i = 2; i < WIDTH - 4; i += 8)
            {
                r = rand() % 100;
                if (j == 12) {
                    num = rand() % 2 + 1;
                    if (r < 65) {
                        draw_block(num, i, j);
                    }
                }
                else if (j == 9) {
                    num = rand() % 3 + 2;
                    if (r < 70) {
                        draw_block(num, i, j);
                    }
                }
                else {
                    num = rand() % 3 + 4;
                    if (r < 75) {
                        draw_block(num, i, j);
                    }
                }
            }
        }
        break;
    case 3:
        life = 4;
        for (j = 12; j >= 6; j -= 3)
        {
            color[j] = rand() % 13 + 1;
            if (color[j] == last_color) {
                continue;
            }
            else {
                last_color = color[j];
            }
            textcolor(color[j], BLACK);
            for (i = 2; i < WIDTH - 4; i += 8)
            {
                r = rand() % 100;
                if (j == 12) {
                    num = rand() % 3 + 1;
                    if (r < 70) {
                        draw_block(num, i, j);
                    }
                }
                else if (j == 9) {
                    num = rand() % 3 + 4;
                    if (r < 75) {
                        draw_block(num, i, j);
                    }
                }
                else {
                    num = rand() % 3 + 6;
                    if (r < 80) {
                        draw_block(num, i, j);
                    }
                }
            }
        }
        break;
    case 4:
        life = 4;
        for (j = 15; j >= 6; j -= 3)
        {
            color[j] = rand() % 13 + 1;
            if (color[j] == last_color) {
                continue;
            }
            else {
                last_color = color[j];
            }
            textcolor(color[j], BLACK);
            for (i = 2; i < WIDTH - 4; i += 8)
            {
                r = rand() % 100;
                if (j == 15) {
                    num = rand() % 4 + 1;
                    if (r < 70) {
                        draw_block(num, i, j);
                    }
                }
                else if (j == 12) {
                    num = rand() % 3 + 4;
                    if (r < 70) {
                        draw_block(num, i, j);
                    }
                }
                else if (j == 9) {
                    num = rand() % 2 + 7;
                    if (r < 75) {
                        draw_block(num, i, j);
                    }
                }
                else {
                    num = rand() % 3 + 10;
                    if (r < 75) {
                        draw_block(num, i, j);
                    }
                }
            }
        }
        break;
    case 5:
        life = 3;
        for (j = 15; j >= 6; j -= 3)
        {
            color[j] = rand() % 13 + 1;
            if (color[j] == last_color) {
                continue;
            }
            else {
                last_color = color[j];
            }
            textcolor(color[j], BLACK);
            for (i = 2; i < WIDTH - 4; i += 8)
            {
                r = rand() % 100;
                if (j == 15) {
                    num = rand() % 3 + 4;
                    if (r < 70) {
                        draw_block(num, i, j);
                        map[i][j] = num;
                    }
                }
                else if (j == 12) {
                    num = rand() % 2 + 7;
                    if (r < 75) {
                        draw_block(num, i, j);
                        map[i][j] = num;
                    }
                }
                else if (j == 9) {
                    num = rand() % 3 + 10;
                    if (r < 75) {
                        draw_block(num, i, j);
                        map[i][j] = num;
                    }
                }
                else {
                    num = rand() % 3 + 14;
                    if (r < 80) {
                        draw_block(num, i, j);
                        map[i][j] = num;
                    }
                }
            }
        }
        break;
    }
}

void gamestage(int num) {
    int r;
    char ch;
    cls(WHITE, BLACK);
    r = rand() % 15;
    switch (num) {
    case 1:
        while (1) {
            int r = rand() % 15 + 1;
            textcolor(r, BLACK);
            gotoxy(18, 7);
            printf(" ##  ###   #    #   ###      # ");
            gotoxy(18, 8);
            printf("#     #   # #  # #  #      # # ");
            gotoxy(18, 9);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 10);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 11);
            printf(" #    #   ###  ###  ###      # ");
            gotoxy(18, 12);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 13);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 14);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 15);
            printf("##    #   # #   #   ###      # ");
            textcolor(WHITE, BLACK);
            gotoxy(22, 19);
            printf("PRESS ");
            textcolor(BLUE2, BLACK);
            printf("SPACEBAR ");
            textcolor(WHITE, BLACK);
            printf("TO START");
            Sleep(400);
            if (kbhit()) {
                ch = getch();
                if (ch == SPACEBAR) break;
            }
        }
        break;
    case 2:
        while (1) {
            int r = rand() % 15 + 1;
            textcolor(r, BLACK);
            gotoxy(18, 7);
            printf(" ##  ###   #    #   ###    ### ");
            gotoxy(18, 8);
            printf("#     #   # #  # #  #        # ");
            gotoxy(18, 9);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 10);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 11);
            printf(" #    #   ###  ###  ###    ### ");
            gotoxy(18, 12);
            printf("  #   #   # #  # #  #      # ");
            gotoxy(18, 13);
            printf("  #   #   # #  # #  #      # ");
            gotoxy(18, 14);
            printf("  #   #   # #  # #  #      # ");
            gotoxy(18, 15);
            printf("##    #   # #   #   ###    ###");
            textcolor(WHITE, BLACK);
            gotoxy(22, 19);
            printf("PRESS ");
            textcolor(BLUE2, BLACK);
            printf("SPACEBAR ");
            textcolor(WHITE, BLACK);
            printf("TO START");
            Sleep(400);
            if (kbhit()) {
                ch = getch();
                if (ch == SPACEBAR) break;
            }
        }
        break;
    case 3:
        while (1) {
            int r = rand() % 15 + 1;
            textcolor(r, BLACK);
            gotoxy(18, 7);
            printf(" ##  ###   #    #   ###    ##  ");
            gotoxy(18, 8);
            printf("#     #   # #  # #  #        # ");
            gotoxy(18, 9);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 10);
            printf("#     #   # #  #    #        # ");
            gotoxy(18, 11);
            printf(" #    #   ###  ###  ###    ### ");
            gotoxy(18, 12);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 13);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 14);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 15);
            printf("##    #   # #   #   ###    ## ");
            textcolor(WHITE, BLACK);
            gotoxy(22, 19);
            printf("PRESS ");
            textcolor(BLUE2, BLACK);
            printf("SPACEBAR ");
            textcolor(WHITE, BLACK);
            printf("TO START");
            Sleep(400);
            if (kbhit()) {
                ch = getch();
                if (ch == SPACEBAR) break;
            }
        }
        break;
    case 4:
        while (1) {
            int r = rand() % 15 + 1;
            textcolor(r, BLACK);
            gotoxy(18, 7);
            printf(" ##  ###   #    #   ###    #   ");
            gotoxy(18, 8);
            printf("#     #   # #  # #  #      # ");
            gotoxy(18, 9);
            printf("#     #   # #  #    #      # #");
            gotoxy(18, 10);
            printf("#     #   # #  #    #      # #");
            gotoxy(18, 11);
            printf(" #    #   ###  ###  ###    #### ");
            gotoxy(18, 12);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 13);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 14);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 15);
            printf("##    #   # #   #   ###      # ");
            textcolor(WHITE, BLACK);
            gotoxy(22, 19);
            printf("PRESS ");
            textcolor(BLUE2, BLACK);
            printf("SPACEBAR ");
            textcolor(WHITE, BLACK);
            printf("TO START");
            Sleep(400);
            if (kbhit()) {
                ch = getch();
                if (ch == SPACEBAR) break;
            }
        }
        break;
    case 5:
        while (1) {
            int r = rand() % 15 + 1;
            textcolor(r, BLACK);
            gotoxy(18, 7);
            printf(" ##  ###   #    #   ###    ### ");
            gotoxy(18, 8);
            printf("#     #   # #  # #  #      # ");
            gotoxy(18, 9);
            printf("#     #   # #  #    #      #  ");
            gotoxy(18, 10);
            printf("#     #   # #  #    #      #  ");
            gotoxy(18, 11);
            printf(" #    #   ###  ###  ###    ### ");
            gotoxy(18, 12);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 13);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 14);
            printf("  #   #   # #  # #  #        # ");
            gotoxy(18, 15);
            printf("##    #   # #   #   ###    ##  ");
            textcolor(WHITE, BLACK);
            gotoxy(22, 19);
            printf("PRESS ");
            textcolor(BLUE2, BLACK);
            printf("SPACEBAR ");
            textcolor(WHITE, BLACK);
            printf("TO START");
            Sleep(400);
            if (kbhit()) {
                ch = getch();
                if (ch == SPACEBAR) break;
            }
        }
        break;
    }
    draw_stage(num);
}

void paddle() {
    textcolor(BLUE2, BLACK);
    for (int i = 0; i < 8; i++)
    {
        map[p.X[i]][p.Y] = 20;
    }
    gotoxy(p.X[0], 46);
    printf("■");
    gotoxy(p.X[1], 46);
    printf("■");
    gotoxy(p.X[2], 46);
    printf("■");
    gotoxy(p.X[3], 46);
    printf("■");
    gotoxy(p.X[4], 46);
    printf("■");
    gotoxy(p.X[5], 46);
    printf("■");
    gotoxy(p.X[6], 46);
    printf("■");
    gotoxy(p.X[7], 46);
    printf("■");
}

void show_score() {
    textcolor(BLUE2, BLACK);
    printf("SCORE");
    textcolor(WHITE, BLACK);
    printf(" : % d", score);
}

void show_life(int x, int y) {
    gotoxy(x, y);
    for (int i = x; i < x + 10; i++)
    {
        printf(" ");
    }
    gotoxy(x, y);
    printf("LIFE : ");
    textcolor(RED1, BLACK);
    for (int i = 0; i < life; i++)
    {
        printf("♥");
    }
}

void init_game() {
    int i, j;
    score = 0;
    num = 1;
    b.X = 33;
    b.Y = 45;
    p.X[0] = 26;
    p.X[1] = 28;
    p.X[2] = 30;
    p.X[3] = 32;
    p.X[4] = 34;
    p.X[5] = 36;
    p.X[6] = 38;
    p.X[7] = 40;
    p.Y = 46;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            map[j][i] = 0;
        }
    }
    for (i = 2; i < WIDTH - 4; i++)
    {
        gotoxy(i, p.Y);
        printf(" ");
    }

    for (i = 0; i < 8; i++)
    {
        map[p.X[i]][p.Y] = 20;
    }
}

void direct() {
    int n, x, y, i, j, l;
    char ch;

    x = b.last_X = b.X;
    y = b.last_Y = b.Y;

    if (b.X == 2) {
        switch (b.ball_direct) {
        case LEFT_TOP:
            b.ball_direct = RIGHT_TOP;
            break;
        case LEFT_DOWN:
            b.ball_direct = RIGHT_DOWN;
            break;
        }
    }

    else if (b.X == WIDTH - 4) {
        switch (b.ball_direct) {
        case RIGHT_TOP:
            b.ball_direct = LEFT_TOP;
            break;
        case RIGHT_DOWN:
            b.ball_direct = LEFT_DOWN;
            break;
        }
    }

    if (b.Y == 6) {
        switch (b.ball_direct) {
        case LEFT_TOP:
            b.ball_direct = LEFT_DOWN;
            break;
        case RIGHT_TOP:
            b.ball_direct = RIGHT_DOWN;
            break;
        }
    }

    if (b.Y >= 47) {  //패들에서 떨어졌을 경우
        PlaySound(TEXT("1.wav"), 0, SND_FILENAME | SND_ASYNC);
        life--;
        show_life(46, 1);
        if (life == 0) {
            ball_moving = 0;
            return;
        }
        else {
            Sleep(1000);
            n = 26;
            for (i = 4; i < WIDTH - 4; i++)
            {
                printf(" ");
                map[i][p.Y] = 0;
            }
            textcolor(WHITE, BLACK);
            draw_box2(0, 5, 66, 48);
            for (i = 0; i < 8; i++)
            {
                gotoxy(p.X[i], p.Y);
                printf("    ");
            }
            gotoxy(b.X, b.Y);
            printf("  ");
            for (int i = 0; i < 8; i++)
            {
                p.X[i] = n;
                n += 2;
            }
            paddle();
            b.X = 33;
            b.Y = 45;
            textcolor(WHITE, BLACK);
            gotoxy(b.X, b.Y);
            printf("●");
            while (1) {
                if (kbhit()) {
                    ch = getch();
                    switch (ch) {
                    case LEFT2:
                        b.ball_direct = LEFT_TOP;
                        break;
                    case RIGHT2:
                        b.ball_direct = RIGHT_TOP;
                        break;
                    default:
                        continue;
                    }
                    break;
                }
            }
        }
        return;
    }

    //패들과 부딪혔을때
    if (map[b.X][b.Y + 1] == 20 || map[b.X][b.Y] == 20 || map[b.X + 1][b.Y] == 20 || map[b.X - 1][b.Y] == 20 || map[b.X - 1][b.Y + 1] == 20 || map[b.X + 1][b.Y] == 20) {
        if (map[b.X][b.Y] == 20) {
            b.Y--;
        }
        switch (b.ball_direct) {
        case LEFT_DOWN:
            b.ball_direct = LEFT_TOP;
            break;
        case RIGHT_DOWN:
            b.ball_direct = RIGHT_TOP;
            break;
        }
    }

    if (map[b.X][b.Y + 1] >= 1 && map[b.X][b.Y + 1] < 20) {
        PlaySound(TEXT("block_break.wav"), 0, SND_FILENAME | SND_ASYNC);
        x = b.X - 2;
        y = b.Y + 1;
        l = x % 8;
        if (l != 8) {
            x -= l;
        }
        x += 2;
        for (j = 0; j < 3; j++)
        {
            for (i = 0; i < 8; i++)
            {
                map[x + i][y + j]--;
            }
        }
        switch (b.ball_direct) {
        case LEFT_DOWN:
            b.ball_direct = LEFT_TOP;
            break;
        case RIGHT_DOWN:
            b.ball_direct = RIGHT_TOP;
            break;
        }
        if (map[x][y] <= 0) {
            delete_block(x, y);
        }
        else {
            draw_block(map[x][y], x, y);
        }
    }

    if (map[b.X][b.Y - 1] >= 1 && map[b.X][b.Y - 1] < 20) {
        PlaySound(TEXT("block_break.wav"), 0, SND_FILENAME | SND_ASYNC);
        x = b.X - 2;
        y = b.Y - 3;
        l = x % 8;
        if (l != 8) {
            x -= l;
        }
        x += 2;
        for (j = 0; j < 3; j++)
        {
            for (i = 0; i < 8; i++)
            {
                map[x + i][y + j]--;
            }
        }
        switch (b.ball_direct) {
        case LEFT_TOP:
            b.ball_direct = LEFT_DOWN;
            break;
        case RIGHT_TOP:
            b.ball_direct = RIGHT_DOWN;
            break;
        }
        if (map[x][y] <= 0) {
            delete_block(x, y);
        }
        else {
            draw_block(map[x][y], x, y);
        }
    }

    if (map[b.X + 1][b.Y] >= 1 && map[b.X + 1][b.Y] < 20) {
        PlaySound(TEXT("block_break.wav"), 0, SND_FILENAME | SND_ASYNC);
        x = b.X + 1;
        y = b.Y;
        l = y % 3;
        if (l == 2) {
            y -= 2;
        }
        else if (l == 1) {
            y -= 1;
        }

        for (j = 0; j < 3; j++)
        {
            for (i = 0; i < 8; i++)
            {
                map[x + i][y + j]--;
            }
        }
        switch (b.ball_direct) {
        case RIGHT_DOWN:
            b.ball_direct = LEFT_DOWN;
            break;
        case RIGHT_TOP:
            b.ball_direct = LEFT_TOP;
            break;
        }
        if (map[x][y] <= 0) {
            delete_block(x, y);
        }
        else {
            draw_block(map[x][y], x, y);
        }
    }

    if (map[b.X - 1][b.Y] >= 1 && map[b.X - 1][b.Y] < 20) {
        PlaySound(TEXT("block_break.wav"), 0, SND_FILENAME | SND_ASYNC);
        x = b.X - 8;
        y = b.Y;
        l = y % 3;
        if (l == 2) {
            y -= 2;
        }
        else if (l == 1) {
            y -= 1;
        }
        for (j = 0; j < 3; j++)
        {
            for (i = 0; i < 8; i++)
            {
                map[x + i][y + j]--;
            }
        }
        switch (b.ball_direct) {
        case LEFT_DOWN:
            b.ball_direct = RIGHT_DOWN;
            break;
        case LEFT_TOP:
            b.ball_direct = RIGHT_TOP;
            break;
        }
        if (map[x][y] <= 0) {
            delete_block(x, y);
        }
        else {
            draw_block(map[x][y], x, y);
        }
    }

    switch (b.ball_direct) {
    case LEFT_TOP:
        b.X--;
        b.Y--;
        break;
    case LEFT_DOWN:
        b.X--;
        b.Y++;
        break;
    case RIGHT_DOWN:
        b.X++;
        b.Y++;
        break;
    case RIGHT_TOP:
        b.X++;
        b.Y--;
        break;
    }
}

void move_ball() {
    ball_moving = 1;
    textcolor(WHITE, BLACK);
    gotoxy(b.last_X, b.last_Y);
    printf("  ");
    gotoxy(b.X, b.Y);
    printf("●");
    Sleep(70);
    direct();
}

int check_clear() {
    for (int j = 2; j < WIDTH - 4; j += 8)
    {
        for (int i = 6; i <= 15; i += 3)
        {
            if (map[j][i] >= 1) {
                return 0;
            }
        }
    }
    return 1;
}

int game_over() {
    cls(WHITE, BLACK);
    PlaySound(TEXT("game_over.wav"), 0, SND_FILENAME | SND_ASYNC);
    int c1, c2;
    char ch;
    while (1) {
        textcolor(WHITE, BLACK);
        gotoxy(12, 18);
        printf("===============================================");
        gotoxy(12, 20);
        printf("===================GAME OVER===================");
        gotoxy(12, 22);
        printf("===============================================");
        gotoxy(29, 24);
        show_score();
        gotoxy(23, 26);
        textcolor(CYAN2, BLACK);
        printf("PRESS ");
        do {
            c1 = rand() % 16;
            c2 = rand() % 15 + 1;
        } while (c1 == c2);
        textcolor(c1, c2);
        printf("SPACEBAR");
        textcolor(CYAN2, BLACK);
        printf(" TO REPLAY");
        gotoxy(27, 28);
        textcolor(WHITE, BLACK);
        printf("PRESS ");
        textcolor(CYAN2, BLACK);
        printf("N ");
        textcolor(WHITE, BLACK);
        printf("TO QUIT");
        if (kbhit()) {
            ch = getch();
            switch (ch) {
            case SPACEBAR:
                return 1;
            case 'n':
            case 'N':
                return 0;
            }
        }
        Sleep(300);
    }
}

int game_clear() {
    cls(WHITE, BLACK);
    PlaySound(TEXT("game_clear.wav"), 0, SND_FILENAME | SND_ASYNC);
    int c1, c2;
    char ch;
    while (1) {
        textcolor(WHITE, BLACK);
        gotoxy(12, 18);
        printf("================================================");
        gotoxy(12, 20);
        printf("================== GAME CLEAR ==================");
        gotoxy(12, 22);
        printf("================================================");
        gotoxy(29, 24);
        show_score();
        gotoxy(23, 26);
        textcolor(CYAN2, BLACK);
        printf("PRESS ");
        do {
            c1 = rand() % 16;
            c2 = rand() % 15 + 1;
        } while (c1 == c2);
        textcolor(c1, c2);
        printf("SPACEBAR");
        textcolor(CYAN2, BLACK);
        printf(" TO REPLAY");
        gotoxy(27, 28);
        textcolor(WHITE, BLACK);
        printf("PRESS ");
        textcolor(CYAN2, BLACK);
        printf("N ");
        textcolor(WHITE, BLACK);
        printf("TO QUIT");
        if (kbhit()) {
            ch = getch();
            switch (ch) {
            case SPACEBAR:
                return 1;
            case 'n':
            case 'N':
                return 0;
            }
        }
        Sleep(300);
    }
}

int main() {
    srand(time(NULL));
    int i, j, r, time, x, num, paddle_moving, oldx, oldy, newx, newy, stage, play, continue_game;
    char ch, ch1, ch2;
    cls(WHITE, BLACK);
    removeCursor();
    PlaySound(TEXT("game_start.wav"), 0, SND_FILENAME | SND_ASYNC);
    textcolor(BLUE2, BLACK);
    draw_box(0, 0, WIDTH, HEIGHT, '*');
    textcolor(WHITE, BLACK);
    draw_box2(6, 2, 60, 4);
    gotoxy(29, 3);
    printf("PLAY GAME");
    textcolor(BLUE2, BLACK);
    gotoxy(10, 7);
    printf("##   #     #    ##  # #   ##  ##    #    ##  # #");
    for (i = 8; i <= 10; i++)
    {
        gotoxy(10, i);
        printf("# #  #    # #  #    # #  #    # #  # #  #    # #");
    }
    gotoxy(10, 11);
    printf("###  #    # #  #    ##   #    ##   ###   #   ###");
    textcolor(BLUE1, BLACK);
    for (i = 12; i <= 14; i++)
    {
        gotoxy(10, i);
        printf("# #  #    # #  #    # #  #    # #  # #    #  # #");
    }
    gotoxy(10, 15);
    printf("##   ###   #    ##  # #   ##  # #  # #  ##   # #");

    int c1, c2;
    while (1) {
        do {
            c1 = rand() % 16;
            c2 = rand() % 15 + 1;
        } while (c1 == c2);
        textcolor(c1, c2);
        gotoxy(13, 26);
        printf("         GAME START : s   END : n         ");
        Sleep(300);
        if (kbhit()) {
            ch = getch();
            if (ch == 'n' || ch == 'N') return;
            else if (ch == 's' || ch == 'S') break;
        }
    }

    cls(WHITE, BLACK);
    for (time = 15; time > 0; time--)
    {
        if (kbhit()) {
            ch = getch();
            if (ch == 'q' || ch == 'Q') break;
        }
        textcolor(WHITE, BLACK);
        gotoxy(21, 6);
        printf("%d초 뒤 게임이 시작됩니다.", time);
        gotoxy(26, 7);
        printf("즉시 시작 : ");
        textcolor(BLUE2, BLACK);
        printf("q");
        gotoxy(26, 10);
        textcolor(WHITE, BLACK);
        printf("< 게임 방법 >");
        gotoxy(11, 12);
        printf("블럭에 써있는 ");
        textcolor(BLUE2, BLACK);
        printf("숫자");
        textcolor(WHITE, BLACK);
        printf("만큼 ");
        textcolor(BLUE2, BLACK);
        printf("공");
        textcolor(WHITE, BLACK);
        printf("으로 깨는 게임입니다.");
        gotoxy(15, 14);
        textcolor(BLUE2, BLACK);
        printf("패들");
        textcolor(WHITE, BLACK);
        printf("을 이용해 ");
        textcolor(BLUE2, BLACK);
        printf("바닥");
        textcolor(WHITE, BLACK);
        printf("에 떨어지지 않게 조종 !");
        gotoxy(8, 16);
        textcolor(BLUE2, BLACK);
        printf("W");
        textcolor(WHITE, BLACK);
        printf("(UP) / ");
        textcolor(BLUE2, BLACK);
        printf("A");
        textcolor(WHITE, BLACK);
        printf("(LEFT) / ");
        textcolor(BLUE2, BLACK);
        printf("D");
        textcolor(WHITE, BLACK);
        printf("(RIGHT) / ");
        textcolor(WHITE, BLACK);
        printf(": 처음 공 발사 방향 지정");
        gotoxy(20, 18);
        textcolor(BLUE2, BLACK);
        printf("방향키 ");
        textcolor(WHITE, BLACK);
        printf("( <-  -> )");
        printf(" : 패들 이동");
        gotoxy(16, 20);
        textcolor(BLUE2, BLACK);
        printf("SCORE");
        textcolor(WHITE, BLACK);
        printf(" : ");
        textcolor(BLUE2, BLACK);
        printf("블록");
        textcolor(WHITE, BLACK);
        printf("을 하나씩 부술 때마다 +10");
        gotoxy(26, 22);
        textcolor(BLUE2, BLACK);
        printf("LIFE ");
        textcolor(WHITE, BLACK);
        printf(": 생명(");
        textcolor(RED1, BLACK);
        printf("♥");
        textcolor(WHITE, BLACK);
        printf(")");
        gotoxy(18, 24);
        printf("공이 ");
        textcolor(BLUE2, BLACK);
        printf("바닥");
        textcolor(WHITE, BLACK);
        printf("에 닿았을 경우 ");
        textcolor(RED1, BLACK);
        printf("♥ ");
        textcolor(WHITE, BLACK);
        printf("-1");
        Sleep(1000);
    }
    cls(WHITE, BLACK);
    num = 1;
    stage = 0;
START:
    init_game();
    cls(WHITE, BLACK);
    gotoxy(22, 7);
    textcolor(BLUE2, BLACK);
    printf(" ##  ###   #    #   ###");
    gotoxy(22, 8);
    printf("#     #   # #  # #  #  ");
    gotoxy(22, 9);
    printf("#     #   # #  #    #  ");
    gotoxy(22, 10);
    printf("#     #   # #  #    #  ");
    gotoxy(22, 11);
    printf(" #    #   ###  ###  ###");
    textcolor(BLUE1, BLACK);
    gotoxy(22, 12);
    printf("  #   #   # #  # #  #  ");
    gotoxy(22, 13);
    printf("  #   #   # #  # #  #  ");
    gotoxy(22, 14);
    printf("  #   #   # #  # #  #  ");
    gotoxy(22, 15);
    printf("##    #   # #   #   ###");
    gotoxy(21, 22);
    textcolor(WHITE, BLACK);
    printf("CHOOSE STAGE (1 ~ 5) >> ");
    scanf("%d", &stage);
    while (1) {
        if (stage < 1 || stage > 5) {
            gotoxy(54, 22);
            printf(" ");
            gotoxy(15, 22);
            printf("PLEASE CHOOSE NUMBER BETWEEN 1 TO 5 >> ");
            scanf("%d", &stage);
            continue;
        }
        else {
            gamestage(stage);
            break;
        }
    }
    paddle();
    textcolor(WHITE, BLACK);
    gotoxy(33, 45);
    printf("●");
    score = 0;

    textcolor(WHITE, BLACK);
    draw_box2(0, 5, 66, 48);

    gotoxy(27, 1);
    show_score();
    show_life(46, 1);
    gotoxy(26, 2);
    textcolor(BLUE2, BLACK);
    printf("ESC ");
    textcolor(WHITE, BLACK);
    printf(": 강제 종료");
    gotoxy(30, 4);
    textcolor(CYAN2, BLACK);
    printf("STAGE %d", stage);
    
    b.last_X = 33;
    b.last_Y = 45;
    play = 0;
    continue_game = 0;
    paddle();
    paddle_moving = 0;
    while (1) {
        if (kbhit()) {
            ch2 = getch();
            switch (ch2) {
            case LEFT2:
                b.ball_direct = LEFT_TOP;
                break;
            case RIGHT2:
                b.ball_direct = RIGHT_TOP;
                break;
            default:
                continue;
            }
            break;
        }
    }

    while (1) {
        paddle();
        gotoxy(27, 1);
        show_score();
        gotoxy(33, 45);
        move_ball();
        if (kbhit()) {
            ch1 = getch();
            switch (ch1) {
            case ESC:
                cls(WHITE, BLACK);
                draw_box2(0, 0, 66, 48);
                for (time = 5; time > 0; time--) {
                    textcolor(BLACK, WHITE);
                    gotoxy(29, 20);
                    show_score();
                    gotoxy(24, 23);
                    printf("게임을 강제 종료합니다.");
                    gotoxy(29, 25);
                    printf("%d초 뒤 종료", time);
                    Sleep(1000);
                }
                return;
            case LEFT:
            case RIGHT:
                paddle_moving = 1;
                break;
            default:
                paddle_moving = 0;
                break;
            }
            if (paddle_moving) {
                switch (ch1) {
                case RIGHT:
                    if (p.X[7] < WIDTH - 4) {
                        map[p.X[0]][p.Y] = 0;
                        gotoxy(p.X[0], p.Y);
                        printf("  ");
                        p.X[0]+=2;
                        p.X[1]+=2;
                        p.X[2]+=2;
                        p.X[3]+=2;
                        p.X[4]+=2;
                        p.X[5]+=2;
                        p.X[6]+=2;
                        p.X[7]+=2;
                    }
                    else {
                        break;
                    }
                    break;
                case LEFT:
                    if (p.X[0] > 2) {
                        map[p.X[7]][p.Y] = 0;
                        gotoxy(p.X[7], p.Y);
                        printf("  ");
                        p.X[0]-=2;
                        p.X[1]-=2;
                        p.X[2]-=2;
                        p.X[3]-=2;
                        p.X[4]-=2;
                        p.X[5]-=2;
                        p.X[6]-=2;
                        p.X[7]-=2;
                    }
                    else {
                        break;
                    }
                    break;
                }
                paddle();
            }
        }
        if (check_clear() == 1) {
            if (game_clear() == 1) {
                goto START;
            }
            else {
                cls(WHITE, BLACK);
                gotoxy(25, 24);
                printf("게임을 종료합니다.");
                return;
            }
        }
        if (ball_moving == 0) {
            Sleep(1000);
            continue_game = game_over();
            if (continue_game == 1) {
                goto START;
            }
            else {
                cls(WHITE, BLACK);
                gotoxy(25, 24);
                printf("게임을 종료합니다.");
                Sleep(1000);
                return;
            }
        }
    }
}
