#include <iostream>
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

struct GameState {
	char map[mapHeight][mapWidth+1];
	TObject mario;
	TObject *brick = nullptr;
	TObject *moving = nullptr;
	int brickLength;
	int movingLength;
	int score = 0;
	int maxLevel = 3;
};

int level = 1; 

void setCur(int x, int y);
void hide_cursor();
void clearMap(GameState& st);
void showMap(GameState& st);
void putObjectOnMap(GameState& st);
void putObjectOnMap(GameState& st, TObject obj);
void setObjectPos(TObject *obj, float xPos, float yPos);
void initObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
void playerDead(GameState& st);
void createLevel(GameState& st, int level);
void vertMoveObject (GameState& st, TObject *obj);
void deletedMoving(GameState& st, int i);
void marioCollision(GameState& st);
void HorizonMoveMapObject (GameState& st, TObject *obj);
void HorizonMoveMap(GameState& st, float dx);

TObject* GetNewBrick(GameState& st);
TObject* GetNewMoving(GameState& st);

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

void clearMap(GameState& st){
    for (int i = 0; i < mapWidth; i++) {
        st.map[0][i] = ' ';
    }
    st.map[0][mapWidth] = '\0';
    for (int j = 1; j < mapHeight; j++) {
        sprintf(st.map[j], st.map[0]);
    }
}

void showMap(GameState& st){
    st.map[mapHeight - 1][mapWidth - 1] = '\0';
    for (int j = 0; j < mapHeight; j++) {
        std::cout << st.map[j];
    }
}

void putObjectOnMap(GameState& st){
    char c[30];
    sprintf(c, "score: %d", st.score);
    int len = strlen(c);
    for (int i = 0; i < len; i++) {
        st.map[1][i + 5] = c[i];
    }
}

void putObjectOnMap(GameState& st, TObject obj){
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); i++) {
        for (int j = iy; j < (iy + iHeight); j++) {
            if (IsPosInMap(i, j)) {
                st.map[j][i] = obj.cType;
            }
		//map[iy][ix] = '@';
        }
    }
}


void setObjectPos(TObject *obj, float xPos, float yPos){
	obj->x = xPos;
	obj->y = yPos;
}

void initObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType){
	setObjectPos(obj, xPos, yPos);
    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertSpeed = 0;
    obj->cType = inType;
    obj->horizSpeed = 0.2;
}

void playerDead(GameState& st){
    system("color 4F");
    Sleep(500);
    createLevel(st, level);
}

void createLevel(GameState& st, int level){
	system("color 9F");
	
    st.brickLength = 0;
    st.brick = (TObject*)realloc(st.brick, 0);
    st.movingLength = 0;
    st.moving = (TObject*)realloc(st.moving, 0);
	
	initObject(&st.mario, 39, 10, 3, 3, '@');
	st.score = 0;
	
	if (level == 1){
		initObject(GetNewBrick(st), 20, 20, 40, 5, '#');
			initObject(GetNewBrick(st), 30, 10, 5, 3, '?');
			initObject(GetNewBrick(st), 50, 10, 5, 3, '?');
		initObject(GetNewBrick(st), 60, 15, 40, 10, '#');
			initObject(GetNewBrick(st), 60, 5, 10, 3, '-');
			initObject(GetNewBrick(st), 70, 5, 5, 3, '?');
			initObject(GetNewBrick(st), 75, 5, 5, 3, '-');
			initObject(GetNewBrick(st), 80, 5, 5, 3, '?');
			initObject(GetNewBrick(st), 85, 5, 10, 3, '-');
        initObject(GetNewBrick(st), 100, 20, 20, 5, '#');
        initObject(GetNewBrick(st), 120, 15, 10, 10, '#');
        initObject(GetNewBrick(st), 150, 20, 40, 5, '#');
        initObject(GetNewBrick(st), 210, 15, 10, 10, '+');
		
		initObject(GetNewMoving(st), 25, 10, 3, 2, 'o');
		initObject(GetNewMoving(st), 80, 10, 3, 2, 'o');
	}
		
	if (level == 2){
        initObject(GetNewBrick(st), 20, 20, 40, 5, '#');
        initObject(GetNewBrick(st), 60, 15, 10, 10, '#');
        initObject(GetNewBrick(st), 80, 20, 20, 5, '#');
        initObject(GetNewBrick(st), 120, 15, 10, 10, '#');
        initObject(GetNewBrick(st), 150, 20, 40, 5, '#');
        initObject(GetNewBrick(st), 210, 15, 10, 10, '+');

        initObject(GetNewMoving(st), 25, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 80, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 65, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 120, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 160, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 175, 10, 3, 2, 'o');
	}
		
		
	if (level == 3){
        initObject(GetNewBrick(st), 20, 20, 40, 5, '#');
        initObject(GetNewBrick(st), 80, 20, 15, 5, '#');
        initObject(GetNewBrick(st), 120, 15, 15, 10, '#');
        initObject(GetNewBrick(st), 160, 10, 15, 15, '#');

        initObject(GetNewMoving(st), 25, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 50, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 80, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 90, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 120, 10, 3, 2, 'o');
        initObject(GetNewMoving(st), 130, 10, 3, 2, 'o');
	}
	st.maxLevel = 3;
}

void vertMoveObject (GameState& st, TObject *obj){
	obj->isFly = true;
	obj->vertSpeed += 0.05;
	setObjectPos(obj, obj->x, obj->y + obj->vertSpeed);
	
	for (int i = 0; i < st.brickLength; i++){
		if (IsCollision( *obj, st.brick[i])){
			if (obj->vertSpeed > 0){
				obj->isFly = false;
			}
			if ((st.brick[i].cType == '?') && (obj->vertSpeed < 0) && (obj == &st.mario)){
				st.brick[i].cType = '-';
				initObject(GetNewMoving(st), st.brick[i].x, st.brick[i].y - 3, 3, 2, '$');
				st.moving[st.movingLength - 1].vertSpeed = -0.7;
			}
			obj->y -= obj->vertSpeed;;
			obj->vertSpeed = 0;;

			if (st.brick[i].cType == '+'){
				level++;
				if (level > st.maxLevel) level = 1;
					system("color 2F");
					Sleep(500);
					createLevel(st, level);
			}
			break;
		}
	}
}

void deletedMoving(GameState& st, int i){
    st.movingLength--;
    st.moving[i] = st.moving[st.movingLength];
    st.moving = (TObject*)realloc(st.moving, sizeof(TObject) * st.movingLength);
}


void marioCollision(GameState& st){
    for (int i = 0; i < st.movingLength; i++) {
        if (IsCollision(st.mario, st.moving[i])) {
            if (st.moving[i].cType == 'o') {
                if ((st.mario.isFly == TRUE) && (st.mario.vertSpeed > 0) &&
                    (st.mario.y + st.mario.height < st.moving[i].y + st.moving[i].height * 0.5)) {
                    st.score += 50;
                    deletedMoving(st, i);
                    i--;
                    continue;
                }
                else playerDead(st);
            }
            if (st.moving[i].cType == '$') {
                st.score += 100;
                deletedMoving(st, i);
                i--;
                continue;
            }
        }
    }
}

void HorizonMoveMapObject (GameState& st, TObject *obj){
	obj->x += obj->horizSpeed;	
	for (int i = 0; i < st.brickLength; i++){
		if ( IsCollision(obj[0], st.brick[i])){
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;
			return;
		}	
	}	
	if (obj[0].cType == 'o'){
		TObject tmp = *obj;
		vertMoveObject(st, &tmp);
		if (tmp.isFly == TRUE){
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;		
		}
	}
}

void HorizonMoveMap(GameState& st, float dx){
	st.mario.x -= dx;
	for (int i = 0; i < st.brickLength; i++){
		if (IsCollision(st.mario, st.brick[i])){
			st.mario.x += dx;
			return;
		}
	}
	st.mario.x += dx;
	
	for (int i = 0; i < st.brickLength; i++){
		st.brick[i].x += dx;
	}
	for (int i = 0; i < st.movingLength; i++){
		st.moving[i].x += dx;
	}
}

bool IsPosInMap(int x, int y){
	return ( (x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight) );
}

bool IsCollision(TObject o1, TObject o2){
	return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
		   ((o1.y + o1.height) >o2.y) && (o1.y < (o2.y + o2.height));
}	

TObject* GetNewMoving(GameState& st) {
    st.movingLength++;
    st.moving = (TObject*)realloc(st.moving, sizeof(TObject) * st.movingLength);
    return st.moving + st.movingLength - 1;
}

TObject* GetNewBrick(GameState& st) {
    st.brickLength++;
    st.brick = (TObject*)realloc(st.brick, sizeof(TObject) * st.brickLength);
    return st.brick + st.brickLength - 1;
}

int main(){
    GameState state;
	hide_cursor();
	createLevel(state, level);
	//system("color 9F");
	
	
	do{
		clearMap(state);
		
		if ((state.mario.isFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) {
			state.mario.vertSpeed = -1.0;
		}
		if (GetKeyState('A') < 0){
				HorizonMoveMap(state, 1);
		}
		if (GetKeyState('D') < 0){
				HorizonMoveMap(state,-1);
		}
		
		if (state.mario.y > mapHeight) {
			playerDead(state);
		}
		
		vertMoveObject(state, &state.mario);
		marioCollision(state);
		
		for ( int i = 0; i < state.brickLength; i++){
			putObjectOnMap(state, state.brick[i]);
		}
		
		for ( int i = 0; i < state.movingLength; i++){
			vertMoveObject (state, state.moving + i);
			HorizonMoveMapObject (state, state.moving + i);
			if (state.moving[i].y > mapHeight){
				deletedMoving(state, i);
				i--;
				continue;
			}
			putObjectOnMap (state, state.moving[i]);
		}
		putObjectOnMap(state, state.mario);
		putObjectOnMap(state);
		
		setCur(0,0);
		showMap(state);
		
		Sleep(10);
	}
	while (GetKeyState(VK_ESCAPE) >= 0);
	
    return 0;
}