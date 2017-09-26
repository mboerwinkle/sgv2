#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "collisionMap.h"

point3d min;//zeroed each tick
point3d max;//zeroed each tick
int gridSize;//reset each tick;
int nodeCount = 0;
int nodesUsed;//reset each tick
node* nodeList = NULL;
int shipIdCount = 0;
int shipIdsUsed;//reset each tick
shipId* shipIdList = NULL;

ship** collision = NULL;
int collisionCount = 0;
int collisionUsed;//reset each ship

void createGrid(){
	initGrid();
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		addShip(&(shipList[shipIdx]), 0);
	}
	showGrid(&(nodeList[0]), 0);
	printf("---------------------------------\n");
}
void initGrid(){
	//reset allocated memory
	nodesUsed = 0;
	shipIdsUsed = 0;
	//finds min and max
	point3d zero = {0, 0, 0};
	point3d big = {INT_MAX, INT_MAX, INT_MAX};
	p3dEqual(min, big);
	p3dEqual(max, zero);
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		point3d p;
		p3dEqual(p, shipList[shipIdx].myPosition);
		for(int dim = 0; dim < 3; dim++){
			if(p[dim] < min[dim]) min[dim] = p[dim];
			if(p[dim] > max[dim]) max[dim] = p[dim];
		}
//		printf("min %d %d %d\nmax %d %d %d\n", min[0], min[1], min[2], max[0], max[1], max[2]);
		int mindif = max[2]-min[2];
		for(int dim = 0; dim < 2; dim++){
			if(max[dim]-min[dim] > mindif) mindif = max[dim]-min[dim];
		}
		gridSize = GRANULARITY;
		while(gridSize < mindif) gridSize*=2;//bitshift?
		getNode();//idx 0
		nodeList[0].size = gridSize;
		p3dEqual(nodeList[0].corner, min);
	}
}
int getNode(){
	if(nodeCount == nodesUsed){
		nodeCount+=10;
		nodeList = realloc(nodeList, sizeof(node)*nodeCount);
	}
	memset(&(nodeList[nodesUsed]), 0, sizeof(node));
	nodeList[nodesUsed].shipIdIdx = -1;
	nodesUsed++;
	return nodesUsed-1;
}
int getShipId(){
	if(shipIdCount == shipIdsUsed){
		shipIdCount+=10;
		shipIdList = realloc(shipIdList, sizeof(shipId)*shipIdCount);
	}
	shipIdsUsed++;
	return shipIdsUsed-1;
}
void addShip(ship* ref, int nodeIdx){
	collisionUsed = 0;
	if(nodeList[nodeIdx].size == GRANULARITY){
		int oldPtr = nodeList[nodeIdx].shipIdIdx;
		int newPtr = getShipId();
		nodeList[nodeIdx].shipIdIdx = newPtr;
		shipIdList[newPtr].ref = ref;
		shipIdList[newPtr].shipIdIdx = oldPtr;
		int dupe = 0;//has already collided this tick
		while(oldPtr != -1){
			if(collisionUsed == collisionCount){
				collisionCount+=5;
				collision = realloc(collision, sizeof(ship*)*collisionCount);
			}
			for(int colIdx = 0; colIdx < collisionUsed; colIdx++){
				if(collision[colIdx] == shipIdList[oldPtr].ref){
					dupe = 1;
					break;
				}
			}
			if(!dupe){
				collision[collisionUsed] = shipIdList[oldPtr].ref;
				collisionUsed++;
				puts("collided");
			}
			oldPtr = shipIdList[oldPtr].shipIdIdx;
		}
		return;
	}
	point3d p = {ref->myPosition[0]-nodeList[nodeIdx].corner[0], ref->myPosition[1]-nodeList[nodeIdx].corner[1], ref->myPosition[2]-nodeList[nodeIdx].corner[2]};
	double r = ref->myModel->radius;
	int size = nodeList[nodeIdx].size;
	int e[8] = {1, 1, 1, 1, 1, 1, 1, 1};//exists
	if(!(p[0]+r >= 0 && p[0]-r < size/2)){//FIXME masks on a char
		e[0] = 0;
		e[1] = 0;
		e[2] = 0;
		e[3] = 0;
	}
	if(!(p[0]+r >= size/2 && p[0]-r < size)){
		e[4] = 0;
		e[5] = 0;
		e[6] = 0;
		e[7] = 0;
	}
	if(!(p[1]+r >= 0 && p[1]-r < size/2)){
		e[0] = 0;
		e[1] = 0;
		e[4] = 0;
		e[5] = 0;
	}
	if(!(p[1]+r >= size/2 && p[1]-r < size)){
		e[2] = 0;
		e[3] = 0;
		e[6] = 0;
		e[7] = 0;
	}
	if(!(p[2]+r >= 0 && p[2]-r < size/2)){
		e[0] = 0;
		e[2] = 0;
		e[4] = 0;
		e[6] = 0;
	}
	if(!(p[2]+r >= size/2 && p[2]-r < size)){
		e[1] = 0;
		e[3] = 0;
		e[5] = 0;
		e[7] = 0;
	}
	for(int i = 0; i < 8; i++){
		if(e[i]){
			if(nodeList[nodeIdx].children[i] == 0){
				int cIdx = getNode();
				nodeList[nodeIdx].children[i] = cIdx;
				p3dEqual(nodeList[cIdx].corner, nodeList[nodeIdx].corner);
				nodeList[cIdx].size = size/2;
				int icopy = i;
				if(icopy >= 4){
					icopy-=4;
					nodeList[cIdx].corner[0]+=size/2;
				}
				if(icopy >= 2){
					icopy-=2;
					nodeList[cIdx].corner[1]+=size/2;
				}
				if(icopy >= 1){
					icopy-=1;//FIXME not strictly necessary...
					nodeList[cIdx].corner[2]+=size/2;
				}
			}
			addShip(ref, nodeList[nodeIdx].children[i]);
		}
	}
}
void showGrid(node* thisOne, int tabs){
	for(int tab = 0; tab < tabs; tab++) printf("  ");
	printf("S%d{\n", thisOne->size);
	for(int x = 0; x < 8; x++){
		if(thisOne->children[x] != 0){
			showGrid(&(nodeList[thisOne->children[x]]), tabs+1);
		}
	}
	int shipIdIdx = thisOne->shipIdIdx;
	int shipsInCell = 0;
	while(shipIdIdx != -1){
		shipsInCell++;
		shipIdIdx = shipIdList[shipIdIdx].shipIdIdx;//is this enough yet? :(
	}
	if(shipsInCell != 0){
		for(int tab = 0; tab < tabs; tab++) printf("  ");
		printf(" _ships_: %d\n", shipsInCell);
	}
	for(int tab = 0; tab < tabs; tab++) printf("  ");
	printf("}\n");
}
