#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <math.h>

// https://m.blog.naver.com/qwaszs/140123447303

/*
conio	-> getch
windows	-> system
process	-> _beginthreadex
math	-> sin, cos, tan
*/

//define Const
#define PI 3.14159265 
#define deg PI/180
#define STICK_Y 19
#define STICK_LENGTH 3
#define SIZEOF_MAP_X 20
#define SIZEOF_MAP_Y 20

//define Mecro
#define FOR(x, n) for(int x=0;x<(n);x++) 
#define print printf

//----------------[Var]----------------
typedef struct vector {
	int x = 0;
	int y = 0;
} Vector;

int i, j;
static int score = 0;
static Vector ballPos;
static Vector ballVel;
static Vector pos;
static char* PIXEL_SHAPE = "   @";
static int map[SIZEOF_MAP_Y][SIZEOF_MAP_X]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//----------------[Func]----------------
int random(int a, int b){
	return rand() % (a - b + 1) + a;
}

void setVector(Vector* a, int x, int y){
	(*a).x = x;
	(*a).y = y;
}

void addVector(Vector* a, Vector b){
	(*a).x += b.x;
	(*a).y += b.y;
}

void gotoxy(int x, int y)
{
 COORD pos={x,y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void setcolor(int color, int bgcolor)
{
    color &= 0xf;
    bgcolor &= 0xf;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

//display map & objects
void render() {
	FOR(i, 20){
		FOR(j, 20){
			gotoxy(j, i);
			if( j>=pos.x && j<pos.x+STICK_LENGTH && pos.y==i){ // player
				setcolor(7,6);
				print("%c", PIXEL_SHAPE[2]);
			} else if(ballPos.x==j && ballPos.y==i){ // ball
				setcolor(4,0);
				print("%c", PIXEL_SHAPE[3]);
			} else {
				setcolor(7,map[i][j]);
				print("%c", PIXEL_SHAPE[map[i][j]]);
			}
		}
		print("\n");
	}
}

int isBetween(int x, int max){
	if(0<=x && x<max){
		return 1;
	}
	return 0;
}

int isOutside(int x, int y){
	if(isBetween(x, SIZEOF_MAP_X) && isBetween(y, SIZEOF_MAP_Y)){
		//state: inside
		return 0;
	}
	//state: outside
	return 1;
}

int nextPosX;
void moveStick(int dx){
	nextPosX = pos.x + dx;
	if(0<=nextPosX && nextPosX+STICK_LENGTH<SIZEOF_MAP_X){
		pos.x += dx;
	}
}

//----------------[GAME]----------------
void init(){
	SetConsoleTitle("Break the Bricks");
	srand(time(NULL));
	setVector(&pos, 7, STICK_Y);
	setVector(&ballPos, 7, STICK_Y - 1);
	setVector(&ballVel, 1, 1);
}

//__stdcall -> Pascal protocall
//delete arg varaibles before return called func
unsigned __stdcall move(void* arg) {
	char inputKey;
	while(1){	
		inputKey = getch();
		switch(inputKey) {
			case 'a':
				moveStick(-1);
				break;
			case 'd':
				moveStick(1);
				break;
		}
	}
}

Vector nextBallPos;
void moveBall(){
	nextBallPos.x = ballPos.x + ballVel.x;
	nextBallPos.y = ballPos.y + ballVel.y;
	// Hit
	//부딪히는 부분 
	if(map[nextBallPos.y][nextBallPos.x]==1){
		if(map[nextBallPos.y][ballPos.x]){
			ballVel.y *= -1;
		}
		if(map[ballPos.y][nextBallPos.x]){
			ballVel.x *= -1;
		}
		map[nextBallPos.y][nextBallPos.x] = 0;
		score++;
	}
	// Bounce
	if(!isBetween(nextBallPos.x, SIZEOF_MAP_X)){
		ballVel.x *= -1;
	}
	if(!isBetween(nextBallPos.y, SIZEOF_MAP_Y)){
		ballVel.y *= -1;
	} else if (nextBallPos.y)
	// Move
	addVector(&ballPos, ballVel);
	
}

void processPhysics(){
	moveBall();
}

int main(void){
	init();
	//threading
    _beginthreadex(NULL, 0, move, 0, 0, NULL);
    while(1){
    	//system("cls");
    	//engine
    	processPhysics();
    	
    	//display
    	render();
    	print("ball P(%d, %d)\t", ballPos.x, ballPos.y);
    	print("V(%d, %d)\n", ballVel.x, ballVel.y);
    	print("play P(%d, %d)\t", pos.x, pos.y);
    	print("S: %d", score);
    	Sleep(100);	
	}
    return 0;
}
