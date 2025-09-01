#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define mapWidth 80
#define mapHeight 25

typedef struct SObject{
	float x,y;
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;

void ClearMap(){
    for(int i = 0; i<mapWidth; i++){
        map[0][i] = '.';
    }
    map[0][mapWidth]='\0';
    for (int j=1;j<mapHeight; j++){
        sprintf(map[j],map[0]);
    }
}

void ShowMap(){
    map[mapHeight-1][mapWidth-1]='\0';
    for (int j=0; j<mapHeight; j++){
        printf("%s",map[j]);
    }
}

void SetObjectPos(TObject *obj, float xPos, float yPos){
	(*obj).x = xPos;
	(*obj).y = yPos;
}

void PutObjectOnMap(TObject obj){
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	map[iy][ix] = '@';

int main(){
    ClearMap();
    ShowMap();
    return 0;
}