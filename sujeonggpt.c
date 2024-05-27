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

int speed = 500; // 속도 추가
int score = 0;
int food_x, food_y;
char nickname[10][20];
int nickname_i = 0;            //nickname의 i값 받을 전역변수

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

void inter_face(char nickname[10][20]);
void map(char nickname[10][20]);
int moveSnake(Snake *snake, int *length, Direction direction);
void food(Snake *snake, int snakeLength);
void reset(Snake *snake, int *snakeLength);
void game_over();

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
                case 112:   // pause 24.05.27
                    system("cls");
                    map(nickname);
                    gotoxy(8, 12, "press any key if you want resume this game");
                    if(_getch()){
                        system("cls");
                        map(nickname);
                        food(snake, snakeLength);  
                        continue;
                    }
                case 27:          //랭킹창으로만 가는 exit
                    game_over();               
                    gotoxy(11, 21, "Press any key to restart the game");
                    _getch();
                    nickname_i += 1;
                    inter_face(nickname);
                    map(nickname);
                    reset(snake, &snakeLength);
                    continue;

                case 101:         //아예 게임 꺼버리는 exit (랭킹창은 나옴)
                    game_over();
                    exit(0);
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

void inter_face(char nickname[10][20]) {
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

    gotoxy(15, 9, "★ 게임 시작 ★");
    gotoxy(12, 11, "방향키로 조작 합니다.");
    gotoxy(8, 12, "닉네임을 입력하면 게임이 시작됩니다.");
    gotoxy(12, 15, "nickname : ");
    scanf("%s", nickname[nickname_i]);
    system("cls");
}

void map(char nickname[10][20]) {
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

    gotoxy(42, 4, "<조작법>");
    gotoxy(40, 6, "<방향키> : →, ←, ↑, ↓");
    gotoxy(40, 7, "<Esc> : exit");
    gotoxy(40, 8, " <P> : pause");
    gotoxy(40, 9, " <e> : real exit game");
    
    gotoxy(42, 13,"<STATUS>");
    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname[nickname_i]);
    gotoxy(40, 17, "score: ");
    printf("%d", score);
    gotoxy(40, 19, "speed: ");
    printf("%d", -(speed-500));
    gotoxy(38, 20, "<MAX SPEED : 500>");
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

//game over 쓸 곳이 많아서 함수로 만들었습니다  24.05.27
void game_over(){
    system("cls");
    map(nickname);
    gotoxy(16, 3, "game over");
    gotoxy(14, 5, "<top 10 ranker>");

    for(int i = 0; i<10; i++){
        if(i<3){
        gotoxy(10, 8+i,"");
        printf("**<%d>**      %s        \n", i+1, nickname[i]);
        }
        else{
        gotoxy(11, 8+i,"");            
        printf("<%d>      %s \n", i+1, nickname[i]);
        }
    }

    gotoxy(0, MAP_Y+MAP_HEIGHT, "");
}

int moveSnake(Snake *snake, int *length, Direction direction) {
    int preTailX, preTailY;
    int headX = snake[0].x;
    int headY = snake[0].y;

    // 벽 충돌
    if (snake[0].x == MAP_X || snake[0].x == (MAP_WIDTH + MAP_X) || snake[0].y == MAP_Y || snake[0].y == (MAP_HEIGHT + MAP_Y)) {
        game_over();
        return 1;
    }
    // 먹이 먹었을 때
    if (snake[0].x == food_x && snake[0].y == food_y) {
        score += 100;
        gotoxy(40, 17, "score: ");
        printf("%d", score);
        (*length)++;
        food(snake, *length);
        if(speed>0)
            speed -= 10;            //속도 증가 추가 24.05.27
        gotoxy(40, 19,"speed: ");
        printf("%d", -(speed-500));
    } else {
        preTailX = snake[*length - 1].x;
        preTailY = snake[*length - 1].y;
        gotoxy(preTailX, preTailY, " ");
    }

    //자기몸과 충돌했는지 검사 
    for(int i=1; i< *length; i++){
        if(snake[0].x==snake[i].x && snake[0].y==snake[i].y){
            game_over();
            return 1;
        }
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

    Sleep(speed); // 이동 주기를 줄여서 더 빠르게 움직이도록 함
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
