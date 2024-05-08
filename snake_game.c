#include<stdio.h>
#include<stdlib.h>  //rand()
#include<time.h>    // ??
#include<windows.h> // gotoxy
#include<conio.h> // 콘솔 입출력 getch()

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2


//gotoxy함수 만들고 포인터 이용해서 원하는 텍스트 찍을 수 있게 했음
void gotoxy(int x, int y, char* s){       
    COORD Pos;
    Pos.X = 2*x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    printf("%s", s);
}

void inter_face(void);
void map(void);

int main(void){
    inter_face();
    map();

    return 0;
}

// interface 일단 만들기만 했습니당
void inter_face(void){
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
    gotoxy(13, 9, "★ 게임 시작 ★");
    gotoxy(12, 11, "방향키로 조작 합니다.");
    gotoxy(8, 12, "아무키나 입력하면 게임이 시작됩니다.");
    gotoxy(12, 15,"nickname : ");
    scanf("%s",nickname);
    gotoxy(15, 16, nickname);
    gotoxy(MAP_WIDTH, MAP_WIDTH, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
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
    gotoxy(20, 15, "nickname: ");
    printf("%s", nickname);
}