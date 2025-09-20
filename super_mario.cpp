#include <cstring>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct TObject{
	float x,y;
	float width, height;
	float vertSpeed;
	float horizSpeed;
	bool isFly;
	char ctype;
};

const char BRICK = '#';
const char EMPTY_BOX = '-';
const char ENEMY = 'o';
const char FULL_BOX = '?';
const char MARIO = '@';
const char MONEY = '$';
const char WIN_BRICK = '+';

/*struct GameState {
	char map[mapHeight][mapWidth+1];
	TObject mario;
	TObject *bricks = nullptr;
	TObject *movings = nullptr;
	int bricksCount;
	int movingsCount;
	int score = 0;
	int maxLevel = 3;
};*/

int level = 1; 

void setCursorPosition(const int x, const int y);
void hide_cursor();
void clearMap(char **map, const int width, const int height);
void setShowMap(char **map, const int width, const int height);
void putObjOnMap(TObject* obj, char **map, const int width, const int height);
void putScoreOnMap(char **map, const int score);
void setObjPosition(TObject* obj, const float xpos, const float ypos);
void initObj(
	TObject* obj, 
	const float xpos, const float ypos, 
	const float owidth, const float oheight, 
	const char inType
);

void rerunLevel(
	TObject& mario, 
	TObject *bricks, int& bricksCount, 
	TObject *movings, int& movingsCount,
	const int currentLevel,
	int& score
);

void createCurrentLevel(
	const int currentLevel, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount, 
	TObject *&movings, int& movingsCount,
	int& score
);

void moveObjVertically (
	TObject* obj, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount,
	TObject *&movings, int& movingsCount,
	int& currentLevel,
	const int maxLevel,
	int& score
);

void deleteMovings(const int i, TObject *movings, int& movingsCount);

void checkMarioCollision(
	TObject& mario, 
	TObject *&bricks, int& bricksCount,
	TObject *&movings, int& movingsCount,
	const int currentLevel,
	int& score
);

void moveObjHorizontally (
	TObject* obj, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount, 
	TObject *&movings, int& movingsCount,
	int& currentLevel,
	const int maxLevel,
	int& score
);

void HorizonMoveMap(float dx);
void moveMapHorizontally(
	const float dx, 
	TObject& mario, 
	TObject *bricks, const int bricksCount, 
	TObject *movings, int& movingsCount
);

//TObject* GetNewbricks();
//TObject* GetNewmovings();

bool IsOnMap(const int x, const int y, const int width, const int height);
bool isCollision(TObject* obj1, TObject* obj2);



int main(){
	hide_cursor();
	
	const int mapWidth = 120;
	const int mapHeight = 30;
	char **map = new char* [mapHeight];
	for (int i = 0; i < mapHeight; i++) {
		map[i] = new char[mapWidth + 1];
	}
	
	TObject mario;
	
	TObject *bricks = nullptr;
	int bricksCount;
	
	TObject *movings = nullptr;
	int movingsCount;
	
	int currentLevel = 1;
	const int maxLevel = 3;
	int score;
	
	
	createCurrentLevel(
		currentLevel,
		mario, 
		bricks, bricksCount, 
		movings, movingsCount,
		score);

	const int jump_key = VK_SPACE;
	const int exit_key = VK_ESCAPE;
	const char left_direction_key = 'A';
	const char right_direction_key = 'D';
	
	do{
		clearMap(map, mapWidth, mapHeight);
		
		if ((mario.isFly == false) && (GetKeyState(jump_key) < 0)) {
			mario.vertSpeed = -1;
		}
		if (GetKeyState(left_direction_key) < 0){
				moveMapHorizontally(
					1,
					mario, 
					bricks, bricksCount, 
					movings, movingsCount);
		}
		if (GetKeyState(right_direction_key) < 0){
				moveMapHorizontally(
				-1, 
				mario, 
				bricks, bricksCount, 
				movings, movingsCount);
		}
		if (mario.y > mapHeight) {
			rerunLevel(
				mario, 
				bricks, bricksCount, 
				movings, movingsCount, 
				currentLevel,  
				score);
		}
		
		moveObjVertically(
			&mario, mario, 
			bricks, bricksCount, 
			movings, movingsCount, 
			currentLevel, maxLevel, 
			score);
			
		checkMarioCollision(
			mario, 
			bricks, bricksCount, 
			movings, movingsCount, 
			currentLevel, score);
		
		
		for (int i = 0; i < bricksCount; i++){
			putObjOnMap(&bricks[i], map, mapWidth, mapHeight);
		}
		
		for (int i = 0; i < movingsCount; i++){
			moveObjVertically (
				&movings[i], 
				mario, 
				bricks, bricksCount, 
				movings, movingsCount, 
				currentLevel, maxLevel, 
				score);
				
			moveObjHorizontally (
				&movings[i], 
				mario, 
				bricks, bricksCount, 
				movings, movingsCount, 
				currentLevel, maxLevel, 
				score
			);
			if (movings[i].y > mapHeight){
				deleteMovings(i, movings, movingsCount);
				i--;
				continue;
			}
			putObjOnMap (&movings[i], map, mapWidth, mapHeight);
		}
		putObjOnMap(&mario, map, mapWidth, mapHeight);
		putScoreOnMap(map, score);
		
		setCursorPosition(0,0);
		setShowMap(map, mapWidth, mapHeight);
		
		Sleep(10);
	}
	while (GetKeyState(exit_key) >= 0);
	
	for (int i = 0; i < mapHeight; i++) {
		delete [] map[i];
	}
	delete [] map;
	
	delete [] bricks;
	delete [] movings;
	
	return 0;
}


void setCursorPosition(const int x, const int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hide_cursor(){
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void clearMap(char **map, const int width, const int height){
    for (int i = 0; i < width; i++) {
        map[0][i] = ' ';
    }
    map[0][width] = '\0';
    for (int j = 1; j < height; j++) {
        sprintf(map[j], map[0]);
    }
}

void setShowMap(char **map, const int width, const int height){
	map[height - 1][width - 1] = '\0';
	for (int j = 0; j < height; j++) {
		std::cout << map[j];
    }
}

void putScoreOnMap(char **map, const int score){
    char c[30];
    sprintf(c, "score: %d", score);
    int len = std::strlen(c);
    for (int i = 0; i < len; i++) {
        map[1][i + 5] = c[i];
    }
}

void putObjOnMap(TObject* obj, char **map, const int width, const int height){
    int ix = (int) round(obj->x);
    int iy = (int) round(obj->y);
    int iwidth = (int) round(obj->width);
    int iheight = (int) round(obj->height);

    for (int i = ix; i < ix + iwidth; i++) {
        for (int j = iy; j < iy + iheight; j++) {
            if (IsOnMap(i, j, width, height)) {
                map[j][i] = obj->ctype;
            }
        }
    }
}


void setObjPosition(TObject *obj, const float xpos, float ypos){
	obj->x = xpos;
	obj->y = ypos;
}

void initObj(
	TObject* obj, 
	const float xpos, const float ypos, 
	const float owidth, const float oheight, 
	const char inType
){
	setObjPosition(obj, xpos, ypos);
    obj->width = owidth;
    obj->height = oheight;
    obj->vertSpeed = 0;
    obj->ctype = inType;
    obj->horizSpeed = 0.2;
}

void rerunLevel(
	TObject& mario, 
	TObject *bricks, int& bricksCount, 
	TObject *movings, int& movingsCount,
	const int currentLevel,
	int& score
){
    system("color 4F");
    Sleep(500);
    createCurrentLevel(
		currentLevel, 
		mario, 
		bricks, bricksCount, 
		movings, movingsCount,
		score);
}

void createCurrentLevel(
	const int currentLevel, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount,
	TObject *&movings, int& movingsCount,
	int& score
){
	system("color 9F");
	
	delete [] bricks;
	bricks = nullptr;
	delete [] movings;
	movings = nullptr;
	
	initObj(&mario, 39, 10, 3, 3, '@');
	score = 0;
	
	switch (currentLevel) {
		case 1:
			bricksCount = 13;
			bricks = new TObject[bricksCount];
			
			initObj(&bricks[0], 20, 20, 40, 5, BRICK);
			
			initObj(&bricks[1], 30, 10, 5, 3, FULL_BOX);
			initObj(&bricks[2], 50, 10, 5, 3, FULL_BOX);
			
			initObj(&bricks[3], 60, 15, 40, 10, BRICK);
			
			initObj(&bricks[4], 60, 5, 10, 3, EMPTY_BOX);
			initObj(&bricks[5], 70, 5, 5, 3, FULL_BOX);
			initObj(&bricks[6], 75, 5, 5, 3, EMPTY_BOX);
			initObj(&bricks[7], 80, 5, 5, 3, FULL_BOX);
			initObj(&bricks[8], 85, 5, 10, 3, EMPTY_BOX);
			
			initObj(&bricks[9], 100, 20, 20, 5, BRICK);
			initObj(&bricks[10], 120, 15, 10, 10, BRICK);
			initObj(&bricks[11], 150, 20, 40, 5, BRICK);
			initObj(&bricks[12], 210, 15, 10, 10, WIN_BRICK);
			
			movingsCount = 2;
			movings = new TObject[movingsCount];
			
			initObj(&movings[0], 25, 10, 3, 2, ENEMY);
			initObj(&movings[1], 80, 10, 3, 2, ENEMY);
			break;
		case 2:
			bricksCount = 6;
			bricks = new TObject[bricksCount];
			
			initObj(&bricks[0], 20, 20, 40, 5, BRICK);
			initObj(&bricks[1], 60, 15, 10, 10, BRICK);
			initObj(&bricks[2], 80, 20, 20, 5, BRICK);
			initObj(&bricks[3], 120, 15, 10, 10, BRICK);
			initObj(&bricks[4], 150, 20, 40, 5, BRICK);
			initObj(&bricks[5], 210, 15, 10, 10, WIN_BRICK);

			movingsCount = 6;
			movings = new TObject[movingsCount];
			
			initObj(&movings[0], 25, 10, 3, 2, ENEMY);
			initObj(&movings[1], 80, 10, 3, 2, ENEMY);
			initObj(&movings[2], 65, 10, 3, 2, ENEMY);
			initObj(&movings[3], 120, 10, 3, 2, ENEMY);
			initObj(&movings[4], 160, 10, 3, 2, ENEMY);
			initObj(&movings[5], 175, 10, 3, 2, ENEMY);
			break;
		case 3:
			bricksCount = 4;
			bricks = new TObject[bricksCount];
			
			initObj(&bricks[0], 20, 20, 40, 5, BRICK);
			initObj(&bricks[1], 80, 20, 15, 5, BRICK);
			initObj(&bricks[2], 120, 15, 15, 10, BRICK);
			initObj(&bricks[3], 160, 10, 15, 15, WIN_BRICK);
			
			movingsCount = 6;
			movings = new TObject[movingsCount];

			initObj(&movings[0], 25, 10, 3, 2, ENEMY);
			initObj(&movings[1], 50, 10, 3, 2, ENEMY);
			initObj(&movings[2], 80, 10, 3, 2, ENEMY);
			initObj(&movings[3], 90, 10, 3, 2, ENEMY);
			initObj(&movings[4], 120, 10, 3, 2, ENEMY);
			initObj(&movings[5], 135, 10, 3, 2, ENEMY);
			break;
		default: 
			break;
	}
}

void moveObjVertically (
	TObject* obj, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount,
	TObject *&movings, int& movingsCount,
	int& currentLevel,
	const int maxLevel,
	int& score
){
	obj->isFly = true;
	obj->vertSpeed += 0.05;
	setObjPosition(obj, obj->x, obj->y + obj->vertSpeed);
	
	for (int i = 0; i < bricksCount; i++){
		if (isCollision(obj, &bricks[i])){
			if (obj->vertSpeed > 0){
				obj->isFly = false;
			}
			if (
				bricks[i].ctype == FULL_BOX && 
				obj->vertSpeed < 0 && 
				obj == &mario
			){
				bricks[i].ctype = EMPTY_BOX;
				
				movingsCount++;
				TObject* temp = new TObject[movingsCount];
				for (int i = 0; i < movingsCount - 1; i++) {
					temp[i] = movings[i];
				}
				delete [] movings;
				movings = temp;
				
				initObj(
					&movings[movingsCount - 1], 
					bricks[i].x, bricks[i].y - 3, 
					3, 2, 
					'$');
				movings[movingsCount - 1].vertSpeed = -0.7;
			}
			obj->y -= obj->vertSpeed;;
			obj->vertSpeed = 0;;

			if (bricks[i].ctype == WIN_BRICK){
				currentLevel++;
				if (currentLevel > maxLevel){
					currentLevel = 1;
				}
					system("color 2F");
					Sleep(500);
					createCurrentLevel(
					currentLevel, 
					mario, 
					bricks, bricksCount, 
					movings, movingsCount,
					score);
			}
			break;
		}
	}
}

void deleteMovings(const int i, TObject *movings, int& movingsCount){
    movingsCount--;
    movings[i] = movings[movingsCount];
}


void checkMarioCollision(
	TObject& mario, 
	TObject *&bricks, int& bricksCount, 
	TObject *&movings, int& movingsCount,
	const int currentLevel,
	int& score
){
    for (int i = 0; i < movingsCount; i++) {
        if (isCollision(&mario, &movings[i])) {
            if (movings[i].ctype == ENEMY) {
                if (					mario.isFly == true && 
					mario.vertSpeed > 0 &&
					mario.y + mario.height < movings[i].y + movings[i].height * 0.5
				){
					score += 50;
                    deleteMovings(i, movings, movingsCount);
                    i--;
                    continue;
                }
                else rerunLevel(
					mario, 
					bricks, bricksCount, 
					movings, movingsCount, 
					currentLevel,  
					score);
            }
            if (movings[i].ctype == MONEY) {
                score += 100;
                deleteMovings(i, movings, movingsCount);
                i--;
                continue;
            }
        }
    }
}

void moveObjHorizontally (
	TObject* obj, 
	TObject& mario, 
	TObject *&bricks, int& bricksCount,
	TObject *&movings, int& movingsCount,
	int& currentLevel,
	const int maxLevel,
	int& score
){
	obj->x += obj->horizSpeed;
	
	for (int i = 0; i < bricksCount; i++){
		if ( isCollision(obj, &bricks[i])){
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;
			return;
		}	
	}	
	if (obj->ctype == ENEMY){
		TObject tmp = *obj;
		moveObjVertically(
			&tmp, 
			mario, 
			bricks, bricksCount, 
			movings, movingsCount, 
			currentLevel, maxLevel, 
			score);
		if (tmp.isFly == true){
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;		
		}
	}
}

void moveMapHorizontally(
	const float dx, 
	TObject& mario, 
	TObject *bricks, const int bricksCount,
	TObject *movings, int& movingsCount
){
	mario.x -= dx;
	for (int i = 0; i < bricksCount; i++){
		if (isCollision(&mario, &bricks[i])){
			mario.x += dx;
			return;
		}
	}
	mario.x += dx;
	
	for (int i = 0; i < bricksCount; i++){
		bricks[i].x += dx;
	}
	for (int i = 0; i < movingsCount; i++){
		movings[i].x += dx;
	}
}

bool IsOnMap(const int x, const int y, const int width, const int height){
	return x >= 0 && x < width && y >= 0 && y < height;
}

bool isCollision(TObject* obj1, TObject* obj2){
	return (
		obj1->x + obj1->width > obj2->x && 
		obj1->x < obj2->x + obj2->width &&
		obj1->y + obj1->height > obj2->y && 
		obj1->y < obj2->y + obj2->height
	);
}	

/*TObject* GetNewmovings() {
    movingsCount++;
    movings = (TObject*)realloc(movings, sizeof(TObject) * movingsCount);
    return movings + movingsCount - 1;
}

TObject* GetNewbricks() {
    bricksCount++;
    bricks = (TObject*)realloc(bricks, sizeof(TObject) * bricksCount);
    return bricks + bricksCount - 1;
}*/