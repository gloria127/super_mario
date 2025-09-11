#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define mapWidth 120
#define mapHeight 30

typedef struct SObject{
	float x,y;
	float width, height;
	float vertSpeed;
	float horizSpeed;
	bool isFly;
	char cType;
} TObject;

char map[mapHeight][mapWidth+1];

TObject mario;
TObject *brick = NULL;
TObject *moving = NULL;
TObject *GetNewMoving();
TObject *GetNewBrick();

int brickLength;
int movingLength;

int level =1;
int score;
int maxLevel;

void setCur(int x, int y);
void hide_cursor();
void clearMap();
void showMap();
void putObjectOnMap();
void putObjectOnMap(TObject obj);
void setObjectPos(TObject *obj, float xPos, float yPos);
void initObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
void playerDead();
void createLevel(int level);
void vertMoveObject (TObject *obj);
void deletedMoving(int i);
void marioCollision();

bool IsPosInMap(int x, int y);
bool IsCollision(TObject o1, TObject o2);

void setCur(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hide_cursor(){
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void clearMap(){
    for(int i = 0; i<mapWidth; i++){
        map[0][i] = ' ';
    }
    map[0][mapWidth]='\0';
    for (int j=1;j<mapHeight; j++){
        sprintf(map[j],map[0]);
    }
}

void showMap(){
    map[mapHeight-1][mapWidth-1]='\0';
    for (int j=0; j<mapHeight; j++){
        printf("%s",map[j]);
    }
}

void putObjectOnMap(){
	char c[30];
	sprintf(c, "score: %d", score);
	int len = strlen(c);
	for (int i = 0; i < len; i++){
		map[1][i+5] = c[i];
	}
}

void putObjectOnMap(TObject obj){
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	int iWidth = (int)round(obj.width);
	int iHeight = (int)round(obj.height);
	
	for (int i = ix; i < (ix + iWidth); i++){
		for (int j = iy; j < (iy + iHeight); j++){
			if (IsPosInMap(i,j)){
				map[j][i] = obj.cType;
			}
	//map[iy][ix] = '@';
		}
	}
}

void setObjectPos(TObject *obj, float xPos, float yPos){
	(*obj).x = xPos;
	(*obj).y = yPos;
}

void initObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType){
	setObjectPos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
	(*obj).vertSpeed = 0;
	(*obj).cType = inType;
	(*obj).horizSpeed = 0.2;
}

void playerDead(){
    system("color 4F");
    Sleep(500);
    createLevel(level);   // теперь компилятор знает про createLevel
}

void createLevel(int level){
	system("color 9F");
	
	brickLength = 0;
	brick = (TObject*) realloc( brick,0);
	movingLength = 0;
	moving = (TObject*) realloc( moving, 0);
	
	initObject(&mario, 39, 10, 3, 3, '@');
	score = 0;
	
	if (level == 1){
		initObject(GetNewBrick(), 20, 20, 40, 5, '#');
			initObject(GetNewBrick(), 30, 10, 5, 3, '?');
			initObject(GetNewBrick(), 50, 10, 5, 3, '?');
		initObject(GetNewBrick(), 60, 15, 40, 10, '#');
			initObject(GetNewBrick(), 60, 5, 10, 3, '-');
			initObject(GetNewBrick(), 70, 5, 5, 3, '?');
			initObject(GetNewBrick(), 75, 5, 5, 3, '-');
			initObject(GetNewBrick(), 80, 5, 5, 3, '?');
			initObject(GetNewBrick(), 85, 5, 10, 3, '-');
		initObject(GetNewBrick(), 100, 20, 20, 5, '#');
		initObject(GetNewBrick(), 120, 15, 10, 10, '#');
		initObject(GetNewBrick(), 150, 20, 40, 5, '#');
		initObject(GetNewBrick(), 210, 15, 10, 10, '+');
		
		initObject(GetNewMoving(), 25, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 80, 10, 3, 2, 'o');
	}
		
	if (level == 2){
		brick = (TObject*)realloc( brick, sizeof(*brick) * brickLength );
		initObject(GetNewBrick(), 20, 20, 40, 5, '#');
		initObject(GetNewBrick(), 60, 15, 10, 10, '#');
		initObject(GetNewBrick(), 80, 20, 20, 5, '#');
		initObject(GetNewBrick(), 120, 15, 10, 10, '#');
		initObject(GetNewBrick(), 150, 20, 40, 5, '#');
		initObject(GetNewBrick(), 210, 15, 10, 10, '+');

		initObject(GetNewMoving(), 25, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 80, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 65, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 120, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 160, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 175, 10, 3, 2, 'o');
	}
		
		
	if (level == 3){
		brick = (TObject*) realloc( brick, sizeof(*brick) * brickLength );
		initObject(GetNewBrick(), 20, 20, 40, 5, '#');
		initObject(GetNewBrick(), 80, 20, 15, 5, '#');
		initObject(GetNewBrick(), 120, 15, 15, 10, '#');
		initObject(GetNewBrick(), 160, 10, 15, 15, '#');

		initObject(GetNewMoving(), 25, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 50, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 80, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 90, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 120, 10, 3, 2, 'o');
		initObject(GetNewMoving(), 130, 10, 3, 2, 'o');
	}
	maxLevel = 3;
}

void vertMoveObject (TObject *obj){
	(*obj).isFly = TRUE;
	(*obj).vertSpeed += 0.25;
	setObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);
	
	for (int i = 0; i < brickLength; i++){
		if (IsCollision( *obj, brick[i])){
			if (obj[0].vertSpeed > 0){
				obj[0].isFly = FALSE;
			}
			if ((brick[i].cType == '?') && (obj[0].vertSpeed < 0) && (obj == &mario)){
				brick[i].cType = '-';
				initObject(GetNewMoving(), brick[i].x, brick[i].y-3, 3, 2, '$');
				moving[movingLength - 1].vertSpeed = -0.7;
			}
			(*obj).y -= ( *obj).vertSpeed;
			(*obj).vertSpeed = 0;

			if (brick[i].cType == '+'){
				if (level > maxLevel){
					level =1;
					system("color 2F");
					Sleep(500);
					createLevel(level);
				}
			}
			break;
		}
	}
}

void deletedMoving(int i){
	movingLength--;
	moving[i] = moving[movingLength];
	moving = (TObject*) realloc( moving, sizeof(*moving) * movingLength );
}


void marioCollision(){
	for (int i = 0; i < movingLength; i++){
		if (IsCollision (mario, moving[i])){
			if (moving[i].cType == 'o'){
				if ( (mario.isFly == TRUE) && (mario.vertSpeed > 0) && (mario.y + mario.height < moving[i].y + moving[i].height * 0.5)){
					score += 50;
					deletedMoving(i);
					i--;
					continue;
				}else 
					playerDead();
			}
			if (moving[i].cType == '$'){
				score += 100;
				deletedMoving(i);
				i--;
				continue;
			}
		}
	}	
}

void HorizonMoveMapObject (TObject *obj){
	obj[0].x += obj[0].horizSpeed;	
	for (int i = 0; i < brickLength; i++){
		if ( IsCollision(obj[0], brick[i])){
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
			return;
		}	
	}	
	if (obj[0].cType == 'o'){
		TObject tmp = *obj;
		vertMoveObject(&tmp);
		if (tmp.isFly == TRUE){
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;		
		}
	}
}

void HorizonMoveMap(float dx){
	mario.x -= dx;
	for (int i = 0; i < brickLength; i++){
		if (IsCollision(mario, brick[i])){
			mario.x += dx;
			return;
		}
	}
	mario.x += dx;
	
	for (int i = 0; i < brickLength; i++){
		brick[i].x += dx;
	}
	for (int i = 0; i < movingLength; i++){
		moving[i].x += dx;
	}
}

bool IsPosInMap(int x, int y){
	return ( (x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight) );
}

bool IsCollision(TObject o1, TObject o2){
	return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
		   ((o1.y + o1.height) >o2.y) && (o1.y < (o2.y + o2.height));
}	

TObject *GetNewMoving(){
	movingLength++;
	moving = (TObject*) realloc( moving, sizeof(TObject) * movingLength); //sizeof(brick)
	return moving + movingLength - 1;
}

TObject *GetNewBrick(){
	brickLength++;
	brick = (TObject*) realloc( brick, sizeof(TObject) * brickLength); //sizeof(brick) 
	return brick + brickLength - 1;
}


int main(){
	
	hide_cursor();
	createLevel(level);
	//system("color 9F");
	
	
	do{
		clearMap();
		
		if ((mario.isFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) {
			mario.vertSpeed = -2.0;
		}
		if (GetKeyState('A') < 0){
				HorizonMoveMap(1);
		}
		if (GetKeyState('D') < 0){
				HorizonMoveMap(-1);
		}
		
		if (mario.y > mapHeight) {
			playerDead();
		}
		
		vertMoveObject(&mario);
		marioCollision();
		
		for ( int i = 0; i <  brickLength; i++){
			putObjectOnMap(brick[i]);
		}
		
		for ( int i = 0; i <  movingLength; i++){
			vertMoveObject (moving + i);
			HorizonMoveMapObject (moving + i);
			if (moving[i].y > mapHeight){
				deletedMoving(i);
				i--;
				continue;
			}
			putObjectOnMap (moving[i]);
		}
		putObjectOnMap(mario);
		putObjectOnMap();
		
		setCur(0,0);
		showMap();
		
		Sleep(10);
	}
	while (GetKeyState(VK_ESCAPE) >= 0);
	setObjectPos(&mario, 20,10);   
    clearMap();
	putObjectOnMap(mario);
    showMap();
    return 0;
}