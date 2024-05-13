#include<stdio.h>
#include<stdlib.h>  //rand()
#include<time.h>    // rand값 초기화
#include<windows.h> // gotoxy
#include<conio.h> // 콘솔 입출력 getch()
#include<unistd.h>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2
#define Initial_Length 5

int score = 0;
int food_x, food_y; //음식 최초 점수

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

void inter_face(char nickname[3][20]);
void map(char nickname[3][20]);
int moveSnake(Snake *snake, int length, Direction direction);
void food(Snake *snake, int snakeLength);



int main(void){
    char nickname[3][20];

    inter_face(nickname);
    map(nickname);
    
    //지렁이 초기 위치 및 몸 길이
    Snake snake[Initial_Length];
    for (int i = 0; i < Initial_Length; i++) {
        snake[i].x = MAP_WIDTH/2 - i; //초기에 머리가 가장 오른쪽에 위치 몸통이 왼쪽으로 이어지도록 하기 위함. i가 증가함에 따라 몸통의 좌표가 왼쪽으로 하나씩 이동
        snake[i].y = MAP_HEIGHT/2;
    }
    int snakeLength = Initial_Length; //나중에 먹이 먹어서 몸 길어질때 사용할 변수.
    Direction direction = RIGHT; //오른쪽으로 이동하게 함.
    
    food(snake, snakeLength);  //음식 생성
    
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
        if(moveSnake(snake, snakeLength, direction) == 1)
            break;
    }  

    return 0;
}

// interface 일단 만들기만 했습니당
void inter_face(char nickname[3][20]){
    system("cls");
    int i, j;

    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y, "■");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT+MAP_Y; j++){
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH, j, "■");
    }
    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT, "■");
    }

    gotoxy(13, 9, "★ 게임 시작 ★");
    gotoxy(12, 11, "방향키로 조작 합니다.");
    gotoxy(8, 12, "아무키나 입력하면 게임이 시작됩니다.");
    gotoxy(12, 15,"nickname : ");
    scanf("%s",nickname[0]);
    system("cls");
}

void map(char nickname[3][20]){  //interface에서 nickname 받아와서 사용
    system("cls");
    int i, j;

    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y, "■");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT+MAP_Y; j++){
        gotoxy(MAP_X, j, "■");
        gotoxy(MAP_X + MAP_WIDTH, j, "■");
    }
    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT, "■");
    }

    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname);
    gotoxy(40, 17, "score: ");
    printf("%d", score);
}

int moveSnake(Snake *snake, int length, Direction direction){
    int preTailX, preTailY;
    // 머리 위치 저장
    int headX = snake[0].x;
    int headY = snake[0].y;

    if(snake[0].x==MAP_X || snake[0].x==(MAP_WIDTH+MAP_X) || snake[0].y==MAP_Y || snake[0].y == (MAP_HEIGHT+MAP_Y)){
        system("cls");
        gotoxy(17, 11, "game over");
        return 1;  //return 1 할라고 int 형식으로 함수 바꿈
    }


    if(snake[0].x == food_x && snake[0].y == food_y){  
        //뱀이 먹이 먹었을 때 score 100점 오르고, food 다른 곳에 또 생성
        score+=100;
        gotoxy(40, 17, "score: ");
        printf("%d", score);
        length++;
        food(snake, length);
    }
    else {
        // 먹이를 먹지 않았을 때
        // 이전 꼬리의 위치
        preTailX = snake[length - 1].x;
        preTailY = snake[length - 1].y;

        // 꼬리를 한 칸 앞으로 이동
        for (int i = length - 1; i > 0; i--) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }

        // 이전 꼬리의 자취 지우기
        gotoxy(preTailX, preTailY, " ");
    }

    //방향키에 따라서 머리 위치 업데이트
    switch(direction) {
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
    // 머리의 새로운 위치 그리기
    gotoxy(headX, headY, "■");
    // 꼬리의 위치 업데이트
    snake[0].x = headX;
    snake[0].y = headY;
   
    // 몸통 그리기
    for (int i = 1; i <= length - 1; i++) {
        gotoxy(snake[i].x, snake[i].y, "■");
    }
    Sleep(500);
}

void food(Snake *snake, int snakeLength){ //먹이 생성
    int food_rand;  //음식이 몸이랑 겹치는지 위한 판별하기 위한 변수

    //뱀 머리랑 몸 쪽에 먹이 생기지 않게 하기
    do{
        food_rand = 0;
        srand(time(NULL));     //난수 지속적으로 초기화

        food_x = rand()%(MAP_WIDTH-1)+(MAP_X+1);  // X좌표에 난수 값 넣기
        food_y = rand()%(MAP_HEIGHT-1)+(MAP_Y+1); // y좌표에 난수 값 넣기
        
        for(int i=0; i<snakeLength; i++){
            if(food_x == snake[i].x && food_y == snake[i].y){
                food_rand = 1;
                break;
            }
        }

    }while(food_rand);     // 만약 food_rand값이 1이 되면 반복문 실행 하고 안되면 그냥 출력되게

    gotoxy(food_x, food_y, "@");
}