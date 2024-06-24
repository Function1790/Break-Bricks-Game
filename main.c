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
#define FOR_BALLS for(m=0; m<ballLength; m++)

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
	Vector pos;		//공의 정수 위치
	Vector_D vel;	//공의 속도
	Vector_D imPos;	//공의 실수 위치
	Vector nextPos;	//공의 다음 위치
};

int i, j, m, k;
static int score = 0;
static double ballSpeed = 0.3;
static Vector pos;
static Ball ballList[100]; //자료구조: list
static int ballLength=0;
static char* PIXEL_SHAPE = "   @";
static int map[SIZEOF_MAP_Y][SIZEOF_MAP_X]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0},
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
//a ~ b 무작위 정수 추출
int random(int a, int b){
	return rand() % (a - b + 1) + a;
}

//벡터 설정
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

//콘솔에서 커서 안보이게(출력 설정)
void SwitchingConsoleCursor(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//콘솔 커서 위치 이동
void gotoxy(int x, int y)
{
 COORD pos={x,y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

//콘솔 커서에서의 색깔 변경
void setcolor(int color, int bgcolor)
{
    color &= 0xf;
    bgcolor &= 0xf;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

//display map & objects
void render() {
	//모든 블록에 대하여
	FOR(i, SIZEOF_MAP_Y){
		FOR(j, SIZEOF_MAP_X){
			//블록 출력크기 2
			//한칸은 작아서 잘 안보임
			gotoxy(j*2, i);
			if( j>=pos.x && j<pos.x+STICK_LENGTH && pos.y==i){ // player
				setcolor(7,6);
				print("  ");
			}  else if(map[i][j]==1){ // 벽돌일 경우
				setcolor(7,1);
				print("  ");
			} else { // 공백일 경우
				setcolor(7,0);
				print("  ");
			}
				
			//모든 공에대해서
			FOR_BALLS{
				//현재 좌표가 공의 좌표일 경우
				if(ballList[m].pos.x==j && ballList[m].pos.y==i){
					gotoxy(j*2, i);
					setcolor(7,4);
					print("  ");
					break;
				}
			}
		}
		print("\n");
	}
	setcolor(7,0); // 원색으로 복구
}

//is 0<=x<max
int isBetween(int x, int max){
	if(0<=x && x<max){
		return 1;
	}
	return 0;
}

//좌표가 맵 안에 있는가
int isOutside(int x, int y){
	if(isBetween(x, SIZEOF_MAP_X) && isBetween(y, SIZEOF_MAP_Y)){
		//state: inside
		return 0;
	}
	//state: outside
	return 1;
}

//플레이어 움직이기
int nextPosX;
void moveStick(int dx){
	nextPosX = pos.x + dx;
	if(0<=nextPosX && nextPosX + STICK_LENGTH < SIZEOF_MAP_X){
		pos.x += dx;
	}
}

//실수의 부호 반환
int sym(double x){
	if(x<0){
		return -1;
	}
	return 1;
}

//공 객체 생성 및 반환
Ball newBall(){
	Ball ball;
	setVector(&ball.pos, 7, SIZEOF_MAP_Y - 2);
	setVector_D(&ball.imPos, 7,  SIZEOF_MAP_Y - 2);
	setVector_D(&ball.vel, 0, -ballSpeed);
	return ball;
}

//ballList에 공 추가
void pushBall(Ball ball){
	ballList[ballLength]=ball;
	ballLength++;
}


//----------------[GAME]----------------
//초기화
void init(){
	SetConsoleTitle("Break the Bricks");
	srand(time(NULL));
	setVector(&pos, 7, STICK_Y);
}

//__stdcall -> use Pascal protocall
//Pascal protocall is ...
//delete arg varaibles before return called func
//플레이어 조종 함수 by Thread
unsigned __stdcall move(void* arg) {
	char inputKey;
	while(1){	
		//키 입력
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

//공에 대한 물리엔진 처리
Vector nextBallPos; //공의 다음 위치
double ballDeg; //튕겨질 공의 각도
int throughX , throughY, betweenX, betweenY;
//반환값 
//1: 공 삭제 | 0: 공 유지
int moveBall(Vector *nextBallPos, Vector_D *ballImPos, Vector_D *ballVel, Vector *ballPos){
	(*nextBallPos).x = (*ballImPos).x + (*ballVel).x;
    (*nextBallPos).y = (*ballImPos).y + (*ballVel).y;
    // 벽돌과 충돌
    if(map[(*nextBallPos).y][(*nextBallPos).x]==1 && isBetween((*nextBallPos).x, SIZEOF_MAP_X -1) && isBetween((*nextBallPos).y, SIZEOF_MAP_Y -1)){    
        throughY = (*ballImPos).y>=(*nextBallPos).y+1 && (*ballImPos).y + (*ballVel).y <= (*nextBallPos).y+1 || (*ballImPos).y<=(*nextBallPos).y && (*ballImPos).y + (*ballVel).y >= (*nextBallPos).y;
        throughX = (*ballImPos).x>=(*nextBallPos).x+1 && (*ballImPos).x + (*ballVel).x <= (*nextBallPos).x+1 || (*ballImPos).x<=(*nextBallPos).x && (*ballImPos).x + (*ballVel).x >= (*nextBallPos).x;
        betweenX = (*nextBallPos).x<=(*ballImPos).x || (*ballImPos).x < (*nextBallPos).x + 1 && (*nextBallPos).x+ (*ballVel).x<=(*ballImPos).x || (*ballImPos).x+ (*ballVel).x < (*nextBallPos).x + 1;
        betweenY = (*nextBallPos).y<=(*ballImPos).y || (*ballImPos).y < (*nextBallPos).y + 1 && (*nextBallPos).y+ (*ballVel).y<=(*ballImPos).y || (*ballImPos).y+ (*ballVel).y < (*nextBallPos).y + 1;
        if(throughY && betweenX){ //y방향 관통 and 공이 충돌한 벽의 x좌표 사이에 존재
            (*ballVel).y *= -1;
        }
        if(throughX && betweenY){ //x방향 관통 and 공이 충돌한 벽의 y좌표 사이에 존재
            (*ballVel).x *= -1;
        }
        map[(*nextBallPos).y][(*nextBallPos).x] = 0;
        score++;
    }
    //맵에 충돌할 경우(좌 우)
    if(!isBetween((*nextBallPos).x, SIZEOF_MAP_X)){
        (*ballVel).x *= -1;
    }
	//맵 밖으로 나갈 경우(하)
    if((*nextBallPos).y>=SIZEOF_MAP_Y){
		return 1;
	}
	//맵에 충돌할 경우(상 하)
    if(!isBetween((*nextBallPos).y, SIZEOF_MAP_Y)){
        (*ballVel).y *= -1;
    }
    //공 움직이기
    (*ballImPos).x += (*ballVel).x;
    (*ballImPos).y += (*ballVel).y;
    setVector(ballPos, (int)(*ballImPos).x, (int)(*ballImPos).y);

    //공이 플레이어와 충돌
    if ((*nextBallPos).y==STICK_Y - 1 && (*nextBallPos).x >= pos.x && (*nextBallPos).x < pos.x+STICK_LENGTH){
		//새로운 공의 각도 랜덤 생성(25~90도)
        ballDeg = random(250,900) * deg / 10;
        (*ballVel).x = sym((*ballVel).x)*ballSpeed*cos(ballDeg);
        (*ballVel).y = -ballSpeed*sin(ballDeg);
    }
    return 0;
}

//물리엔진 처리
int destoryIndex;
void processPhysics(){
	destoryIndex=-1; //삭제할 공의 인덱스
	FOR_BALLS{
		if(moveBall(&ballList[m].nextPos, &ballList[m].imPos, &ballList[m].vel, &ballList[m].pos)){
			destoryIndex=m;
		}
	}
	
	if(destoryIndex!=-1){
		//삭제할 인덱스의 다음거부터 앞으로 한칸씩 이동
		for(k=destoryIndex+1; k<ballLength; k++){
			ballList[k-1] = ballList[k];
		}
		ballLength--;
	}
}


int main(void){
	//출력 설정
	SwitchingConsoleCursor(false);
	//초기화
	init();
	pushBall(newBall());
	pushBall(newBall());
	pushBall(newBall());
	pushBall(newBall());
	//threading
    _beginthreadex(NULL, 0, move, 0, 0, NULL);
	//게임 실행
    while(1){
    	//물리엔진
    	processPhysics();

    	//랜더링
    	render();
		//변수 상태
    	print("Balls: %d\t", ballLength);
    	print("play P(%d, %d)\t", pos.x, pos.y);
    	gotoxy(SIZEOF_MAP_X*2+2, 3);
    	print("Score: %d", score);

		//남아 있는 공이 없다면 게임 종료
    	if(ballLength==0){
    		break;
		}
	}
	print("\nEND GAME\n");
    return 0;
}
