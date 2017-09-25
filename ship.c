#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ship.h"

ship* shipList = NULL;
int shipCount = 0;

void tickShips(){
	for(int shipIdx = 0; shipIdx < shipCount; shipIdx++){
		ship* currShip = &(shipList[shipIdx]);
		currShip->ai(currShip, &(currShip->myAiData));
		double axis[3] = {1, 0, 0};
		rotVector(axis, currShip->myRotation);
		currShip->myPosition[0]+=currShip->speed*axis[0];
		currShip->myPosition[1]+=currShip->speed*axis[1];
		currShip->myPosition[2]+=currShip->speed*axis[2];
	}
}

void spawnHumanShip(int userIdx, point3d pos, quaternion rot, int type){
	puts("spawning human ship");
	shipList = realloc(shipList, (shipCount+1)*sizeof(ship));
	shipList[shipCount] = copyShip(&(shipTemplates[type]), pos, rot);
	shipList[shipCount].type = type;//FIXME this shouldnt be here. complete newship
	shipList[shipCount].myModel = &(models[type]);
	shipList[shipCount].ai = humanAi;
	shipList[shipCount].myAiData.human.myuser = &(userList[userIdx]);
	shipCount++;
}
void spawnComputerShip(point3d pos, quaternion rot, int type){
	shipList = realloc(shipList, (shipCount+1)*sizeof(ship));
	shipList[shipCount] = copyShip(&(shipTemplates[type]), pos, rot);
	shipList[shipCount].type = type;//FIXME this shouldnt be here. complete newship
	shipList[shipCount].myModel = &(models[type]);
	shipList[shipCount].ai = fighterAi;
	shipCount++;
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
