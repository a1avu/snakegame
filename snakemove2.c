#include<stdio.h>
#include<stdlib.h>  //rand()
#include<time.h>    //srand()
#include<windows.h> // gotoxy
#include<conio.h> // 콘솔 입출력 getch()
#include <unistd.h> //Sleep() 쓰려면 추가

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2
#define Initial_Length 5

//지렁이의 위치를 나타내는 구조체
typedef struct{
    int x;
    int y;
    }Snake;
// 지렁이의 이동방향을 나타내는 열거형
typedef enum{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;


//gotoxy함수 만들고 포인터 이용해서 원하는 텍스트 찍을 수 있게 했음
void gotoxy(int x, int y, char* s){
    COORD Pos;
    Pos.X = 2*x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    printf("%s", s);
}

void inter_face(char *nickname);
void map(void);
void moveSnake(Snake *snake, int length, Direction direction);

int main(void){
    char nickname[20];
    inter_face(nickname);
    map();

    //지렁이 초기 위치 및 몸 길이
    Snake snake[Initial_Length];
    for (int i = 0; i < Initial_Length; i++) {
        snake[i].x = MAP_WIDTH/2 - i; //초기에 머리가 가장 오른쪽에 위치 몸통이 왼쪽으로 이어지도록 하기 위함. i가 증가함에 따라 몸통의 좌표가 왼쪽으로 하나씩 이동
        snake[i].y = MAP_HEIGHT/2;
    }
    int snakeLength = Initial_Length; //나중에 먹이 먹어서 몸 길어질때 사용할 변수.
    Direction direction = RIGHT; //오른쪽으로 이동하게 함.

    while(1){
        if (_kbhit()) { // 사용자 입력 감지
            char ch = _getch();
            switch(ch) {
                case 72: // 방향키 위쪽
                    direction = UP;
                    break;
                case 80: // 방향키 아래쪽
                    direction = DOWN;
                    break;
                case 75: // 방향키 왼쪽
                    direction = LEFT;
                    break;
                case 77: // 방향키 오른쪽
                    direction = RIGHT;
                    break;
                default:
                    break;
            }
        }
        moveSnake(snake, snakeLength, direction);
    }
    return 0;
}


// interface 일단 만들기만 했습니당
void inter_face(char *nickname){
    system("cls");
    int i, j;
    for(i = MAP_X; i<=MAP_WIDTH; i++){
        gotoxy(i, MAP_Y, "■");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT; j++){
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH - 3, j, "■");
    }
    for(i = MAP_X; i<=MAP_WIDTH; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT - 1, "■");
    }
    gotoxy(13, 9, "★ 게임 시작 ★");
    gotoxy(12, 11, "방향키로 조작 합니다.");
    gotoxy(8, 12, "아무키나 입력하면 게임이 시작됩니다.");
    gotoxy(12, 15, "nickname : ");
    scanf("%s", nickname);
    gotoxy(12 +10, 15 , nickname); // 입력받은 닉네임 출력
    system("cls");
}
void map(void){
    int i, j;
    for(i = MAP_X; i<=MAP_WIDTH; i++){
        gotoxy(i, MAP_Y, "■");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT; j++){
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH - 3, j, "■");
    }
    for(i = MAP_X; i<=MAP_WIDTH; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT - 1, "■");
    }
}
void moveSnake(Snake *snake, int length, Direction direction){
    // 현재 지렁이의 머리 위치
    int preX = snake[0].x;
    int preY = snake[0].y;
    //방향키에 따라서 머리 위치 업데이트
    switch(direction) {
        case UP:
            snake[0].y--;
            break;
        case DOWN:
            snake[0].y++;
            break;
        case LEFT:
            snake[0].x--;
            break;
        case RIGHT:
            snake[0].x++;
            break;
        default:
            break;
    }
    /*충돌 검사
    for (int i = 1; i < length; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            // 충돌 발생 시 게임 종료 등의 처리를 해주어야 합니다.
            // 여기서는 간단하게 프로그램 종료로 처리합니다.
            exit(0);
        }
    }*/
    // 이전 꼬리의 위치
    int preTailX = snake[length - 1].x;
    int preTailY = snake[length - 1].y;

    gotoxy(preTailX, preTailY, " "); // 이전 꼬리 지우기

    // 몸통 이동
    for (int i = length - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    // 새로운 머리 그리기
    gotoxy(snake[0].x, snake[0].y ,"■");

    // 몸통 그리기
    for (int i = 1; i <= length - 1; i++) {
        gotoxy(snake[i].x, snake[i].y, "■");
    }
    Sleep(1000);
}

/* <해야하는 코드 수정>
  1.먹이가 랜덤으로 나오게하는 것 추가
  2.먹이를 먹으면 몸의 길이가 길어지게 하는 것 추가
  3.머리가 벽에 부딪히면 게임이 종료되는 코드 추가 */