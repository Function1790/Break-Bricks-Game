#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <math.h>


// https://m.blog.naver.com/qwaszs/140123447303
// 맵 입출력 

/*
conio	-> getch
windows	-> system
procexss	-> _beginthreadex
math	-> sin, cos, tan
*/

//define Const
#define PI 3.14159265 
#define deg PI/180
#define STICK_LENGTH 4
#define SIZEOF_MAP_X 20
#define SIZEOF_MAP_Y 25
#define STICK_Y 24

//define Mecro
#define FOR(x, n) for(int x=0;x<(n);x++) 
#define print printf

//----------------[Var]----------------
typedef struct vector {
	int x = 0;
	int y = 0;
} Vector;

typedef struct vector_double {
	double x = 0;
	double y = 0;
} Vector_D;

struct Ball {
	Vector pos;
	Vector_D vel;
	Vector_D imPos;
};

int i, j, m;
static int score = 0;
static Vector ballPos;
static Vector_D ballVel;
static Vector_D ballImPos; // ball Imagine Pos
static double ballSpeed = 0.3;
static Vector pos;
static Ball ballList[100];
static int ballLength=0;
static char* PIXEL_SHAPE = "   @";
static int map[SIZEOF_MAP_Y][SIZEOF_MAP_X]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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

void setVector_D(Vector_D* a, double x, double y){
	(*a).x = x;
	(*a).y = y;
}

void addVector(Vector* a, Vector b){
	(*a).x += b.x;
	(*a).y += b.y;
}

void SwitchingConsoleCursor(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
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
	FOR(i, SIZEOF_MAP_Y){
		FOR(j, SIZEOF_MAP_X){
			gotoxy(j*2, i);
			if( j>=pos.x && j<pos.x+STICK_LENGTH && pos.y==i){ // player
				setcolor(7,6);
				print("  ");
			} else if(ballPos.x==j && ballPos.y==i){ // ball
				setcolor(7,4);
				print("  ");
			} else {
				if(map[i][j]==1){
					setcolor(7,1);
					print("  ");
				} else {
					setcolor(7,0);
					print("  ");
				}
				
			}
		}
		print("\n");
	}
	setcolor(7,0);
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

int sym(double x){
	if(x<0){
		return -1;
	}
	return 1;
}

Ball newBall(){
	Ball ball;
	setVector(&(ball.pos), 7, STICK_Y);
	setVector_D(&(ball.imPos), 7,  SIZEOF_MAP_Y - 2);
	setVector_D(&(ball.vel), 0, -1);
}

void pushBall(Ball ball){
	ballList[ballLength-1]=ball;
	ballLength++;
}


//----------------[GAME]----------------
void init(){
	SetConsoleTitle("Break the Bricks");
	srand(time(NULL));
	setVector(&pos, 7, STICK_Y);
	setVector_D(&ballImPos, 7,  SIZEOF_MAP_Y - 2);
	setVector_D(&ballVel, 0, -1);
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
double ballDeg;
int throughX , throughY, betweenX, betweenY;
void moveBall(){
	nextBallPos.x = ballImPos.x + ballVel.x;
	nextBallPos.y = ballImPos.y + ballVel.y;
	// Hit
	if(map[nextBallPos.y][nextBallPos.x]==1 && isBetween(nextBallPos.x, SIZEOF_MAP_X -1) && isBetween(nextBallPos.y, SIZEOF_MAP_Y -1)){
		throughY = ballImPos.y>=nextBallPos.y+1 && ballImPos.y + ballVel.y <= nextBallPos.y+1 || ballImPos.y<=nextBallPos.y && ballImPos.y + ballVel.y >= nextBallPos.y;
		throughX = ballImPos.x>=nextBallPos.x+1 && ballImPos.x + ballVel.x <= nextBallPos.x+1 || ballImPos.x<=nextBallPos.x && ballImPos.x + ballVel.x >= nextBallPos.x;
		betweenX = nextBallPos.x<=ballImPos.x || ballImPos.x < nextBallPos.x + 1 && nextBallPos.x+ ballVel.x<=ballImPos.x || ballImPos.x+ ballVel.x < nextBallPos.x + 1;
		betweenY = nextBallPos.y<=ballImPos.y || ballImPos.y < nextBallPos.y + 1 && nextBallPos.y+ ballVel.y<=ballImPos.y || ballImPos.y+ ballVel.y < nextBallPos.y + 1;
		if(throughY && betweenX){
			ballVel.y *= -1;
		}
		if(throughX && betweenY){
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
	}
	// Move
	ballImPos.x += ballVel.x;
	ballImPos.y += ballVel.y;
	setVector(&ballPos, (int)ballImPos.x, (int)ballImPos.y);
	
	// Hit Bar
	if (nextBallPos.y==STICK_Y - 1 && nextBallPos.x >= pos.x && nextBallPos.x < pos.x+STICK_LENGTH){
		ballDeg = random(250,900) * deg / 10;
		ballVel.x = sym(ballVel.x)*ballSpeed*cos(ballDeg);
		ballVel.y = -ballSpeed*sin(ballDeg);
	}	
}

void processPhysics(){
	moveBall();
}
int main(void){
	SwitchingConsoleCursor(false);
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
    	print("V(%.2f, %.2f)\n", ballVel.x, ballVel.y);
    	print("play P(%d, %d)\t", pos.x, pos.y);
    	gotoxy(SIZEOF_MAP_X*2+2, 3);
    	print("Score: %d", score);
    	//Sleep(0);	
	}
    return 0;
}
