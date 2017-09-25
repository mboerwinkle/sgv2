#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "collisionMap.h"

int collisionCount = 0;
int collisionsUsed;//reset each tick
collision* collisionList = NULL;
point3d min;//zeroed each tick
point3d max;//zeroed each tick
int gridSize;//reset each tick;
int nodeCount = 0;
int nodesUsed;//reset each tick
node* nodeList = NULL;
int shipIdCount = 0;
int shipIdsUsed;//reset each tick
shipId* shipIdList = NULL;

void createGrid(){
	initGrid();
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		addShip(&(shipList[shipIdx]), 0);
	}
	showGrid();
}
void initGrid(){
	//reset allocated memory
	collisionsUsed = 0;
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
		printf("min %d %d %d\nmax %d %d %d\n", min[0], min[1], min[2], max[0], max[1], max[2]);
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
	nodesUsed++;
	return nodesUsed-1;
}
int getCollision(){
	if(collisionCount == collisionsUsed){
		collisionCount+=10;
		collisionList = realloc(collisionList, sizeof(collision)*collisionCount);
	}
	collisionsUsed++;
	return collisionsUsed-1;
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
	if(nodeList[nodeIdx].size == GRANULARITY){
		int oldPtr = nodeList[nodeIdx].shipIdIdx;
		int newPtr = getShipId();
		nodeList[nodeIdx].shipIdIdx = newPtr;
		shipIdList[newPtr].ref = ref;
		shipIdList[newPtr].shipIdIdx = oldPtr;
		while(oldPtr != 0){
			int colPtr = getCollision();
			collisionList[colPtr][0] = ref;
			collisionList[colPtr][1] = shipIdList[oldPtr].ref;
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
void showGrid(){
}
