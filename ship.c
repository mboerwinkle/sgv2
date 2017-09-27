#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ship.h"

ship* shipList = NULL;
int shipCount = 0;
int shipCountMax = 0;

ship* spawnQueue = NULL;//FIXME have a data type for this structure
int spawnQueueSize = 0;
int spawnQueueCapacity = 0;

void killShips(){
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		if(shipList[shipIdx].hp <= 0){
			free(shipList[shipIdx].myAbilities);
			shipList[shipIdx] = shipList[shipCount-1];
			shipCount--;
		}
	}
}

void tickShips(){
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		ship* currShip = &(shipList[shipIdx]);
		currShip->ai(currShip, &(currShip->myAiData));
		double axis[3] = {1, 0, 0};
		rotVector(axis, currShip->myRotation);
		currShip->myPosition[0]+=(currShip->speed)*(axis[0]);
		currShip->myPosition[1]+=(currShip->speed)*(axis[1]);
		currShip->myPosition[2]+=(currShip->speed)*(axis[2]);
	}
}
void addSpawnQueue(point3d pos, quaternion rot, int type, void (*ai)(ship*, aiData*), aiData myData){
	if(spawnQueueSize == spawnQueueCapacity){
		spawnQueueCapacity+=5;
		spawnQueue = realloc(spawnQueue, spawnQueueCapacity*sizeof(ship));
	}
	spawnQueue[spawnQueueSize] = copyShip(&(shipTemplates[type]), pos, rot);
	spawnQueue[spawnQueueSize].type = type;//FIXME this shouldnt be here. complete newship
	spawnQueue[spawnQueueSize].myModel = &(models[type]);
	spawnQueue[spawnQueueSize].ai = ai;
	spawnQueue[spawnQueueSize].myAiData = myData;
	spawnQueueSize++;
}
void clearSpawnQueue(){
	for(int queueIdx = 0; queueIdx < spawnQueueSize; queueIdx++){
		spawnShip(&(spawnQueue[queueIdx]));
	}
	spawnQueueSize = 0;
}
void spawnShip(ship* queueEntry){
	if(shipCount == shipCountMax){
		shipCountMax+=10;
		shipList = realloc(shipList, shipCountMax*sizeof(ship));
	}
	shipList[shipCount] = *queueEntry;
	shipCount++;
	puts("spawned ship");
}

int getShipsWithin(ship*** output, point3d position, int distance){
	*output =  calloc(shipCount, sizeof(ship*));
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		(*output)[shipIdx] = &(shipList[shipIdx]);
	}
	return shipCount;//FIXME inefficient
}

ship copyShip(ship* copyTarget, point3d pos, quaternion rot){
	ship o = (*copyTarget);
	memcpy(o.myPosition, pos, sizeof(point3d));
	memcpy(o.myRotation, rot, sizeof(quaternion));
	return o;
};

ship newShip(int hp, int maxHp, ability** myAbilities, int abilityCount, point3d myPosition, quaternion myRotation, double speed, double maxSpeed, double accel, double decel, double rollspeed, double pitchspeed, double yawspeed){
	ship o;
	o.hp = hp;
	o.maxHp = maxHp;
	o.myAbilities = myAbilities;
	o.abilityCount = abilityCount;
	o.speed = speed;
	o.maxSpeed = maxSpeed;
	o.accel = accel;
	o.decel = decel;
	o.rollSpeed = rollspeed;
	o.pitchSpeed = pitchspeed;
	o.yawSpeed = yawspeed;
	memcpy(o.myPosition, myPosition, sizeof(point3d));
	memcpy(o.myRotation, myRotation, sizeof(quaternion));
	return o;//FIXME should newship and copyship accept a pointer to write to instead?

};
