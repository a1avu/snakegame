#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2
#define Initial_Length 5

int score = 0;
int food_x, food_y;
char nickname[3][20];

typedef struct {
    int x;
    int y;
} Snake;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

void gotoxy(int x, int y, char* s) {
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    printf("%s", s);
}

void inter_face(char nickname[3][20]);
void map(char nickname[3][20]);
int moveSnake(Snake *snake, int *length, Direction direction);
void food(Snake *snake, int snakeLength);
void reset(Snake *snake, int *snakeLength);

int main(void) {
    Snake snake[100];
    Direction direction = RIGHT;
    int snakeLength = Initial_Length;

    inter_face(nickname);
    map(nickname);
    reset(snake, &snakeLength);

    DWORD lastMoveTime = GetTickCount();

    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 72: // 방향키 위쪽
                    if (direction != DOWN) direction = UP;
                    break;
                case 80: // 방향키 아래쪽
                    if (direction != UP) direction = DOWN;
                    break;
                case 75: // 방향키 왼쪽
                    if (direction != RIGHT) direction = LEFT;
                    break;
                case 77: // 방향키 오른쪽
                    if (direction != LEFT) direction = RIGHT;
                    break;
                default:
                    break;
            }
        }

        if (GetTickCount() - lastMoveTime > 100) { // 이동 주기를 100ms로 설정
            if (moveSnake(snake, &snakeLength, direction) == 1)
                break;
            lastMoveTime = GetTickCount();
        }
    }

    return 0;
}

void inter_face(char nickname[3][20]) {
    system("cls");
    int i, j;

    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y, "■");
    }
    for (j = MAP_Y + 1; j <= MAP_HEIGHT + MAP_Y; j++) {
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH, j, "■");
    }
    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y + MAP_HEIGHT, "■");
    }

    gotoxy(13, 9, "★ 게임 시작 ★");
    gotoxy(12, 11, "방향키로 조작 합니다.");
    gotoxy(8, 12, "아무키나 입력하면 게임이 시작됩니다.");
    gotoxy(12, 15, "nickname : ");
    scanf("%s", nickname[0]);
    system("cls");
}

void map(char nickname[3][20]) {
    int i, j;

    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y, "■");
    }
    for (j = MAP_Y + 1; j <= MAP_HEIGHT + MAP_Y; j++) {
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH, j, "■");
    }
    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y + MAP_HEIGHT, "■");
    }

    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname);
    gotoxy(40, 17, "score: ");
    printf("%d", score);
}

void reset(Snake *snake, int *snakeLength) {
    for (int i = 0; i < Initial_Length; i++) {
        snake[i].x = MAP_WIDTH / 2 - i;
        snake[i].y = MAP_HEIGHT / 2;
        gotoxy(snake[i].x, snake[i].y, "ㅇ");
    }
    gotoxy(snake[0].x, snake[0].y, "ㅎ");
    Direction direction = RIGHT;
    food(snake, *snakeLength);
}

int moveSnake(Snake *snake, int *length, Direction direction) {
    int preTailX, preTailY;
    int headX = snake[0].x;
    int headY = snake[0].y;

    if (snake[0].x == MAP_X || snake[0].x == (MAP_WIDTH + MAP_X) || snake[0].y == MAP_Y || snake[0].y == (MAP_HEIGHT + MAP_Y)) {
        system("cls");
        map(nickname);
        gotoxy(15, 12, "game over");
        gotoxy(0, MAP_HEIGHT + MAP_Y + 2, "\n");
        return 1;
    }

    if (snake[0].x == food_x && snake[0].y == food_y) {
        score += 100;
        gotoxy(40, 17, "score: ");
        printf("%d", score);
        (*length)++;
        food(snake, *length);
    } else {
        preTailX = snake[*length - 1].x;
        preTailY = snake[*length - 1].y;
        gotoxy(preTailX, preTailY, " ");
    }

    for (int i = *length - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    switch (direction) {
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        default:
            break;
    }

    // 머리의 이전 위치를 몸통으로 표시
    gotoxy(snake[0].x, snake[0].y, "ㅇ");

    // 머리의 새로운 위치 그리기
    gotoxy(headX, headY, "ㅎ");
    snake[0].x = headX;
    snake[0].y = headY;

    for (int i = 1; i <= *length - 1; i++) {
        gotoxy(snake[i].x, snake[i].y, "ㅇ");
    }

    Sleep(50); // 이동 주기를 줄여서 더 빠르게 움직이도록 함
    return 0;
}

void food(Snake *snake, int snakeLength) {
    int food_rand;

    do {
        food_rand = 0;
        srand(time(NULL));

        food_x = rand() % (MAP_WIDTH - 1) + (MAP_X + 1);
        food_y = rand() % (MAP_HEIGHT - 1) + (MAP_Y + 1);

        for (int i = 0; i < snakeLength; i++) {
            if (food_x == snake[i].x && food_y == snake[i].y) {
                food_rand = 1;
                break;
            }
        }

    } while (food_rand);

    gotoxy(food_x, food_y, "@");
}
