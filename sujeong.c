/* 140��° �ٱ��� ������ */
#include<stdio.h>
#include<stdlib.h>  //rand()
#include<time.h>    // rand�� �ʱ�ȭ
#include<windows.h> // gotoxy
#include<conio.h> // �ܼ� ����� getch()

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 2
#define Initial_Length 5

int score = 0;
int food_x, food_y; //���� ���� ����

//�������� ��ġ�� ��Ÿ���� ����ü
typedef struct{
    int x;
    int y;
    }Snake;
// �������� �̵������� ��Ÿ���� ������
typedef enum{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

//gotoxy�Լ� ����� ������ �̿��ؼ� ���ϴ� �ؽ�Ʈ ���� �� �ְ� ����
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
void reset(Snake *snake, int snakeLength);


int main(void){
    char nickname[3][20];
    Snake snake[100]; // �ִ���̸� 100���� ����
    Direction direction = RIGHT; // ���� ������ �ʱ�ȭ

    inter_face(nickname);
    map(nickname);
    int snakeLength = Initial_Length; //���߿� ���� �Ծ �� ������� ����� ����.
    reset(snake,snakeLength);

    while(1){
        if (_kbhit()) { // ����� �Է� ����
            char ch = _getch();
            switch(ch) {
                case 72: // ����Ű ����
                   direction = UP;
                    break;
                case 80: // ����Ű �Ʒ���
                    direction = DOWN;
                    break;
                case 75: // ����Ű ����
                    direction = LEFT;
                    break;
                case 77: // ����Ű ������
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

void inter_face(char nickname[3][20]){
    int i, j;

    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y, "��");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT+MAP_Y; j++){
        gotoxy(MAP_X, j, "��");
        gotoxy(MAP_X + MAP_WIDTH, j, "��");
    }
    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT, "��");
    }

    gotoxy(13, 9, "�� ���� ���� ��");
    gotoxy(12, 11, "����Ű�� ���� �մϴ�.");
    gotoxy(8, 12, "�ƹ�Ű�� �Է��ϸ� ������ ���۵˴ϴ�.");
    gotoxy(12, 15,"nickname : ");
    scanf("%s",nickname[0]);
    system("cls");
}

void map(char nickname[3][20]){  //interface���� nickname �޾ƿͼ� ���
    int i, j;

    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y, "��");
    }
    for(j= MAP_Y + 1; j<=MAP_HEIGHT+MAP_Y; j++){
        gotoxy(MAP_X, j, "��");
        gotoxy(MAP_X + MAP_WIDTH, j, "��");
    }
    for(i = MAP_X; i<=MAP_WIDTH+MAP_X; i++){
        gotoxy(i, MAP_Y + MAP_HEIGHT, "��");
    }

    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname);
    gotoxy(40, 17, "score: ");
    printf("%d", score);
}
void reset(Snake *snake, int snakeLength){
                    
    for (int i = 0; i < Initial_Length; i++) {
        snake[i].x = MAP_WIDTH/2 - i; //�ʱ⿡ �Ӹ��� ���� �����ʿ� ��ġ ������ �������� �̾������� �ϱ� ����. i�� �����Կ� ���� ������ ��ǥ�� �������� �ϳ��� �̵�
        snake[i].y = MAP_HEIGHT/2;
	gotoxy(snake[i].x, snake[i].y,"��");
    }
    gotoxy(snake[0].x,snake[0].y,"��"); 
    //�ϴ� �Ӹ��� ������ ������ ��(�ѱ�)�� �ٲ��.
    Direction direction = RIGHT; //���������� �̵��ϰ� ��.
    
    food(snake, snakeLength);  //���� ����
}
// �ϴ� ���� 140���� �ٱ��� ���� �����ص�!!!!!!

int moveSnake(Snake *snake, int length, Direction direction){
    int preTailX, preTailY;
    // �Ӹ� ��ġ ����
    int headX = snake[0].x;
    int headY = snake[0].y;

    if(snake[0].x==MAP_X || snake[0].x==(MAP_WIDTH+MAP_X) || snake[0].y==MAP_Y || snake[0].y == (MAP_HEIGHT+MAP_Y)){
        system("cls");
        gotoxy(17, 11, "game over");
        return 1;  //return 1 �Ҷ�� int �������� �Լ� �ٲ�
    }
    if(snake[0].x == food_x && snake[0].y == food_y){  
        //���� ���� �Ծ��� �� score 100�� ������, food �ٸ� ���� �� ����
        score+=100;
        gotoxy(40, 17, "score: ");
        printf("%d", score);
        length++;
        food(snake, length);
    }
    else {
        // ���̸� ���� �ʾ��� ��
        // ���� ������ ��ġ
        preTailX = snake[length - 1].x;
        preTailY = snake[length - 1].y;

        // ������ �� ĭ ������ �̵�
        for (int i = length - 1; i > 0; i--) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }

        // ���� ������ ���� �����
        gotoxy(preTailX, preTailY, " ");
    }

    //����Ű�� ���� �Ӹ� ��ġ ������Ʈ
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
    // �Ӹ��� ���ο� ��ġ �׸���
    gotoxy(headX, headY, "��");
    // ������ ��ġ ������Ʈ
    snake[0].x = headX;
    snake[0].y = headY;
   
    // ���� �׸���
    for (int i = 1; i <= length - 1; i++) {
        gotoxy(snake[i].x, snake[i].y, "��");
    }
    Sleep(500);
}

void food(Snake *snake, int snakeLength){ //���� ����
    int food_rand;  //������ ���̶� ��ġ���� ���� �Ǻ��ϱ� ���� ����

    //�� �Ӹ��� �� �ʿ� ���� ������ �ʰ� �ϱ�
    do{
        food_rand = 0;
        srand(time(NULL));     //���� ���������� �ʱ�ȭ

        food_x = rand()%(MAP_WIDTH-1)+(MAP_X+1);  // X��ǥ�� ���� �� �ֱ�
        food_y = rand()%(MAP_HEIGHT-1)+(MAP_Y+1); // y��ǥ�� ���� �� �ֱ�
        
        for(int i=0; i<snakeLength; i++){
            if(food_x == snake[i].x && food_y == snake[i].y){
                food_rand = 1;
                break;
            }
        }

    }while(food_rand);     // ���� food_rand���� 1�� �Ǹ� �ݺ��� ���� �ϰ� �ȵǸ� �׳� ��µǰ�

    gotoxy(food_x, food_y, "@");
}