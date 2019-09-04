#include <iostream>
#include <windows.h>
#include <chrono>
#include <stdio.h>
#define HEIGHT 10
#define WIDTH 10

enum directions{
	U,
	UR,
	R,
	RD,
	D,
	DL,
	L,
	LU,
};

enum materials{
	OBSTACLE=-2,
	EMPTY,
	DESTINATION,
};

enum algorithm{
	FOUR_WAY,
	EIGHT_WAY,
};

int path[HEIGHT][WIDTH];
char newScreen[HEIGHT][WIDTH];
char screen[HEIGHT][WIDTH];
char map[HEIGHT][WIDTH+1]={
"##########",
"# # #    #",
"#   #    #",
"###      #",
"#     ####",
"#        #",
"#####    #",
"#        #",
"#        #",
"##########"
};


void GoToXY(short x, short y)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position= {y,x};
    SetConsoleCursorPosition(h,position);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void SetScreen(int xx, int yy)
{
    for(int x=0; x<xx; x++)
    {
        for(int y=0; y<yy; y++)
        {
            if(screen[x][y]!=map[x][y])
            {
                screen[x][y]=map[x][y];
            }
        }
    }
}

void PrintScreen(int height, int width)
{
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            if(newScreen[x][y]!=screen[x][y])
            {
                newScreen[x][y]=screen[x][y];
                GoToXY(x, y);
                std::cout<<newScreen[x][y];
            }
        }
    }
}

void printPath(int _x, int _y){
	for(int x=0;x<_x;x++){
		for(int y=0;y<_y;y++){
			//std::cout<<path[y][x];	
			printf("%3d",path[x][y]);
		}
		std::cout<<std::endl;
	}
}

void fillPath(int _x, int _y, int dX, int dY){
	for(int x=0;x<_x;x++){
		for(int y=0;y<_y;y++){
			if(map[y][x]==' '){
				path[y][x]=EMPTY;
			}
			else{
				path[y][x]=OBSTACLE;
			}
		}
	}
	path[dY][dX]=DESTINATION;
}

void solve(int _x, int _y){
	int pathLength = DESTINATION+1;
	for(int repeat=0;repeat<100;repeat++){
		for(int x=0;x<_x;x++){
			for(int y=0;y<_y;y++){
				if(path[y][x]==pathLength-1){
					if(path[y+1][x]==EMPTY&&y<_y){
						path[y+1][x]= pathLength;
					}	
					if(path[y][x+1]==EMPTY&&x<_x){
						path[y][x+1]= pathLength;
					}	
					if(path[y-1][x]==EMPTY&&y>0){
						path[y-1][x]= pathLength;
					}
					if(path[y][x-1]==EMPTY&&x>0){
						path[y][x-1]= pathLength;
					}
				}
			}
		}
		pathLength++;
	}
}

float AIX=1.0f;
float AIY=1.0f;
float AI_Speed=5.0f;

int destinationX=8;
int destinationY=8;

bool moveable[8];
int distance;


int main() {
	ShowConsoleCursor(false);
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	fillPath(HEIGHT, WIDTH, destinationX, destinationY);
	GoToXY(HEIGHT+1,0);
	solve(HEIGHT, WIDTH);
	printPath(HEIGHT, WIDTH);
	distance=path[(int)AIX][(int)AIY]-1;
	while(1){
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();
	   	if((int)AIX!=destinationX&&(int)AIY!=destinationY){
	   		//path[(int)AIY][(int)AIX]
	   		for(int xx=0;xx<WIDTH;xx++){
	   			for(int yy=0;yy<HEIGHT;yy++){
					 if(path[xx][yy]==distance){
					 	if(AIX==xx&&AIY==yy){
					 		distance--;
						 }
					 	
					 	while((int)AIX>xx){
					 		AIX-=AI_Speed*fElapsedTime;	
						}
						while((int)AIX<xx){
					 		AIX+=AI_Speed*fElapsedTime;	
						}
						while((int)AIY>yy){
					 		AIY+=AI_Speed*fElapsedTime;	
						}
						while((int)AIY>yy){
					 		AIY-=AI_Speed*fElapsedTime;	
						}
					 	//AIX=xx;
					 	//AIY=yy;
					 	
					 }			
				}	
			}	
		}
		SetScreen(HEIGHT, WIDTH);
		screen[(int)AIY][(int)AIX]='P';
		PrintScreen(HEIGHT, WIDTH);
		GoToXY(0,0);	
	}

	return 0;
}
