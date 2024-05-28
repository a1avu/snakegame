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

int speed = 500; // �ӵ� �߰�
int food_x, food_y;



typedef struct {
    int score;
    char nickname[20];        //nickname�̶� score ���� ���� ����ü ����
} Player;

Player player[11];          //�߰� �Ǵ� ����� �׻� 11��° �ε����� ��ġ�ϰ� ���� (������ ����� 10������ �ۿ� �ȵǴ�)
int player_i = 0;            //nickname�� i�� ���� ��������


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

void inter_face();
void map(char* nickname, int score);
int moveSnake(Snake *snake, int *length, Direction direction);
void food(Snake *snake, int snakeLength);
void reset(Snake *snake, int *snakeLength);
void game_over();
void ranking_sort();
void load_scores();
void save_scores();

int main(void) {
    load_scores();
    Snake snake[100];
    Direction direction = RIGHT;
    int snakeLength = Initial_Length;

    inter_face();
    map(player[player_i].nickname, player[player_i].score);
    reset(snake, &snakeLength);

    DWORD lastMoveTime = GetTickCount();

    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 72: // ����Ű ����
                    if (direction != DOWN) direction = UP;
                    break;
                case 80: // ����Ű �Ʒ���
                    if (direction != UP) direction = DOWN;
                    break;
                case 75: // ����Ű ����
                    if (direction != RIGHT) direction = LEFT;
                    break;
                case 77: // ����Ű ������
                    if (direction != LEFT) direction = RIGHT;
                    break;
                case 112:   // pause 24.05.27
                    system("cls");
                    map(player[player_i].nickname, player[player_i].score);
                    gotoxy(8, 12, "press any key if you want resume this game");
                    if(_getch() == 'p'){
                        system("cls");
                        map(player[player_i].nickname, player[player_i].score);
                        food(snake, snakeLength);  
                        continue;
                    }
                case 27:          // esc ��ŷâ���θ� ���� exit
                    game_over();
                    save_scores();
                if(_getch() == 'e'){        //�̷��� �ؾ� getch�� �ѹ��� ����� ������ �𸣰���
                    exit(0);                        
                }                    
                    snakeLength = Initial_Length;
                    speed = 500;
                    player_i += 1;
                    inter_face();
                    map(player[player_i].nickname, player[player_i].score);
                    reset(snake, &snakeLength);
                    direction = RIGHT;
                    lastMoveTime = GetTickCount();
                    continue;
                case 101:         //�ƿ� ���� �������� exit (��ŷâ�� ����)
                    game_over();
                    save_scores();
                    exit(0);
                default:
                    break;
            }
        }
        
        if (GetTickCount() - lastMoveTime > 100) { // �̵� �ֱ⸦ 100ms�� ����
            if (moveSnake(snake, &snakeLength, direction) == 1){
                                                            //�浹��� �� �ٽ� ���� �� �ÿ� ���� ����
                game_over();
                if(_getch() == 'e'){
                    exit(0);                        
                }
                snakeLength = Initial_Length;
                speed = 500;
                player_i += 1;
                inter_face();
                map(player[player_i].nickname, player[player_i].score);
                reset(snake, &snakeLength);
                direction = RIGHT;
                lastMoveTime = GetTickCount();

            }
            lastMoveTime = GetTickCount();
        }
    }

    return 0;
}

void inter_face() {
    system("cls");
    int i, j;

    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y, "��");
    }
    for (j = MAP_Y + 1; j <= MAP_HEIGHT + MAP_Y; j++) {
        gotoxy(MAP_X, j, "��");
        gotoxy(MAP_X + MAP_WIDTH, j, "��");
    }
    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y + MAP_HEIGHT, "��");
    }

    gotoxy(15, 9, "�� ���� ���� ��");
    gotoxy(12, 11, "����Ű�� ���� �մϴ�.");
    gotoxy(8, 12, "�г����� �Է��ϸ� ������ ���۵˴ϴ�.");
    gotoxy(12, 15, "nickname : ");
    scanf("%s", player[player_i].nickname);
    system("cls");
}

void map(char *nickname, int score) {       //map�Լ��� �Ķ���� �־��༭ gameover �ÿ��� status â ��ü �ʱ�ȭ
    int i, j;

    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y, "��");
    }
    for (j = MAP_Y + 1; j <= MAP_HEIGHT + MAP_Y; j++) {
        gotoxy(MAP_X, j, "��");
        gotoxy(MAP_X + MAP_WIDTH, j, "��");
    }
    for (i = MAP_X; i <= MAP_WIDTH + MAP_X; i++) {
        gotoxy(i, MAP_Y + MAP_HEIGHT, "��");
    }

    gotoxy(42, 4, "<���۹�>");
    gotoxy(40, 6, "<����Ű> : ��, ��, ��, ��");
    gotoxy(40, 7, "<Esc> : exit");
    gotoxy(40, 8, " <P> : pause");
    gotoxy(40, 9, " <e> : real exit game");
    
    gotoxy(42, 13,"<STATUS>");
    gotoxy(40, 15, "nickname: ");
    printf("%s", nickname);
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
        gotoxy(snake[i].x, snake[i].y, "��");
    }
    gotoxy(snake[0].x, snake[0].y, "��");
    Direction direction = RIGHT;
    food(snake, *snakeLength);
}

//game over �� ���� ���Ƽ� �Լ��� ��������ϴ�  24.05.27
void game_over(){
    system("cls");
    speed = 500;
    map("",0);

    ranking_sort();
    save_scores();
    gotoxy(16, 3, "game over");
    gotoxy(14, 5, "<top 10 ranker>");

    for(int i = 0; i<10; i++){
        if(i<3){
        gotoxy(10, 8+i,"");
        printf("**<%d>**", i+1);
        gotoxy(15 , 8+i, "");
        printf("%s",player[i].nickname);
        gotoxy(22, 8+i, "");
        printf("%d", player[i].score);
        }
        else{
        gotoxy(11, 8+i,"");            
        printf("<%d>", i+1);
        gotoxy(15 , 8+i, "");
        printf("%s",player[i].nickname);
        gotoxy(22, 8+i, "");
        printf("%d", player[i].score);
        }
    }
    gotoxy(5, 21, "Press any key to restart the game (<e> to real exit)");
}

int moveSnake(Snake *snake, int *length, Direction direction) {
    int preTailX, preTailY;
    int headX = snake[0].x;
    int headY = snake[0].y;

    // �� �浹
    if (snake[0].x == MAP_X || snake[0].x == (MAP_WIDTH + MAP_X) || snake[0].y == MAP_Y || snake[0].y == (MAP_HEIGHT + MAP_Y)) {
        return 1;  //1�� �����ص� gameover�� �� main�Լ����� �޾���
    }
    // ���� �Ծ��� ��
    if (snake[0].x == food_x && snake[0].y == food_y) {
        player[player_i].score += 100;
        gotoxy(40, 17, "score: ");
        printf("%d", player[player_i].score);
        (*length)++;
        food(snake, *length);
        if(speed>0)
            speed -= 10;            //�ӵ� ���� �߰� 24.05.27
        gotoxy(40, 19,"speed: ");
        printf("%d", -(speed-500));
    }
    else {
        preTailX = snake[*length - 1].x;
        preTailY = snake[*length - 1].y;
        gotoxy(preTailX, preTailY, " ");
    }

    //�ڱ���� �浹�ߴ��� �˾ƺ���
    for(int i=1; i< *length; i++){
        if(snake[0].x==snake[i].x && snake[0].y==snake[i].y){
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

    // �Ӹ��� ���� ��ġ�� �������� ǥ��
    gotoxy(snake[0].x, snake[0].y, "��");

    // �Ӹ��� ���ο� ��ġ �׸���
    gotoxy(headX, headY, "��");
    snake[0].x = headX;
    snake[0].y = headY;

    for (int i = 1; i <= *length - 1; i++) {
        gotoxy(snake[i].x, snake[i].y, "��");
    }

    Sleep(speed); // �̵� �ֱ⸦ �ٿ��� �� ������ �����̵��� ��
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

void ranking_sort(){
    Player temp;
    for(int i = 0; i<10; i++){
        for(int j = 0; j< 10 - i; j++){
                if(player[j].score<player[j+1].score){
                    temp = player[j];
                    player[j] = player[j+1];
                    player[j+1] = temp;
                }
        }
    }
}

void load_scores(){
    FILE *fp = fopen("score.txt", "r");
    if(fp == NULL){
        return;
    }
    player_i = 0;
    while(fscanf(fp, "%s %d",player[player_i].nickname, &player[player_i].score)!=EOF && player_i <10){
        player_i++;
    }
    fclose(fp);
}
void save_scores(){
    FILE *fp = fopen("score.txt", "a"); //w���� a�� �ٲ㺽
    if(fp == NULL){
        printf("Error opening file!\n");
        return;
    }
    for(int i= 0; i<10 && player[i].score > 0; i++){
        fprintf(fp, "%s %d\n",player[i].nickname,player[i].score);
    }
    fclose(fp);
}
//��¥�� �س�.