#include<stdio.h>
#include<stdlib.h>  //rand()
#include<time.h>    //srand()
#include<windows.h> // gotoxy
#include<conio.h> // 콘솔 입출력 getch()

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2


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
void moveSnake(Snake *snake, Direction direction);

int main(void){
    char nickname[20];
    inter_face(nickname);
    map();

    Snake snake = {MAP_WIDTH/2, MAP_HEIGHT/2}; // 시작위치를 설정, 맵의 가로 중앙과 세로 중앙에서 시작
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
    moveSnake(&snake, direction);
    map();
    return 0;
}
}

// interface 일단 만들기만 했습니당
void inter_face(char *nickname){
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
    gotoxy(12, 15,"nickname : ");
    scanf("%s",nickname);
    gotoxy(15, 16, nickname);
    gotoxy(MAP_WIDTH, MAP_WIDTH, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    system("cls");
}
void map(void){
    system("cls");
    int i, j;
    char nickname[3][10];
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
    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname);
    gotoxy(MAP_WIDTH, MAP_WIDTH, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
void moveSnake(Snake *snake, Direction direction){
     int preX = snake -> x;
     int preY = snake -> y;
     switch(direction) {
        case UP:
            snake->y--;
            break;
        case DOWN:
            snake->y++;
            break;
        case LEFT:
            snake->x--;
            break;
        case RIGHT:
            snake->x++;
            break;
        default:
            break;
}
gotoxy(preX, preY, " "); //이전에 있던 지렁이의 위치를 지움
gotoxy(snake ->x, snake  -> y,"■"); //새로운 위치의 지렁이를 그림.
}