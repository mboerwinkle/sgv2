#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "collisionMap.h"
#include "bullet.h"
#include "ship.h"
#include "common/geo/geo.h"
#include "common/stlLoad/loadStl.h"
#include "delay/delay.h"
#include "net/listen.h"

unsigned int tickCount = 0;
void handleHumanSpawnRequests();
ship shipTemplates[3];//FIXME put in new function dedicated to generating types (from file?)
int main(){
	srandom(time(NULL));
	loadModels("common/models/");
	printf("SIZES:\nShip %ld\nBullet %ld\nNetwork Ship %ld\nNetwork Bullet %ld\n", sizeof(ship), sizeof(bullet), sizeof(networkShipData), sizeof(networkBullet));
	point3d start = {0, 0, 0};
	quaternion rot = {1, 0, 0, 0};
	shipTemplates[0] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.1, 0.1, 0.1);
	shipTemplates[1] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.05, 0.05, 0.05);
	shipTemplates[2] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.1, 0.1, 0.1);
	setupNetworkListen();
	{
		point3d cs = {5000000, 5000000, 5000000};
		quaternion cr = {1, 0, 0, 0};
		aiData dat;
		dat.fighter.mode = -1;
		addSpawnQueue(cs, cr, 0, idleAi, dat, 1, NULL, 0);
	/*	for(int it = 0; it < 30; it++){
			ability* myAbilities = calloc(1, sizeof(ability));
			myAbilities[0].act = ability_Machinegun;
			myAbilities[0].max = 2;
			myAbilities[0].cooldown = 2;
			addSpawnQueue(cs, cr, 1, fighterAi, dat, 1, myAbilities, 1);
			cs[0]+=400;
		}
		cs[1] +=500;
		cs[2] += 500;
		cs[0] -= 400*30;
		for(int it = 0; it < 30; it++){
			ability* myAbilities = calloc(1, sizeof(ability));
			myAbilities[0].act = ability_Machinegun;
			myAbilities[0].max = 2;
			myAbilities[0].cooldown = 2;
			addSpawnQueue(cs, cr, 1, fighterAi, dat, 0, myAbilities, 1);
			cs[0]+=400;
		}*/
	} 
	while(1){
		delay(40);
		handleHumanSpawnRequests();
		clearSpawnQueue();
		createGrid();

		tickShips();
		killShips();
		sendAllUserData();
		tickBullets();
		tickCount++;
		
	}
}

void handleHumanSpawnRequests(){
	for(int userIdx = 0; userIdx < userCount; userIdx++){
		if(userList[userIdx].myControls.spawn != -1){//they want the spawn
			point3d cs = {5000000, 5000000, 5000000};
			quaternion cr = {1, 0, 0, 0};
			aiData dat;
			dat.human.myuser = &(userList[userIdx]);
			ability* myAbilities = calloc(1, sizeof(ability));
			myAbilities[0].act = ability_Machinegun;
			myAbilities[0].max = 2;
			myAbilities[0].cooldown = 2;
			addSpawnQueue(cs, cr, 1, humanAi, dat, 0, myAbilities, 1);
			userList[userIdx].myControls.spawn = -1;
			
		}
	}
}
