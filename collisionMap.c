#include <stdio.h>
#include <stdlib.h>
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
	for(shipIdx = 0; shipIdx < shipCount; shipIdx++){
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
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		point3d p;
		p3dEqual(p, shipList[shipIdx].myPosition);
		point3d zero = {0, 0, 0};
		p3dEqual(min, zero);
		p3dEqual(max, zero);
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
		while(oldPtr != NULL){
			int colPtr = getCollision();
			collisionList[colPtr][0] = ref;
			collisionList[colPtr][1] = shipIdList[oldPtr].ref;
		}
	}
	if(
	if(
	if(
	if(
	if(
	if(
	if(
	if(
}
void showGrid(){
}
