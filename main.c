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
ship shipTemplates[5];//FIXME put in new function dedicated to generating types (from file?)
int main(){
	srandom(time(NULL));
	loadModels("common/models/");
	printf("SIZES:\nShip %ld\nBullet %ld\nNetwork Ship %ld\nNetwork Bullet %ld\n", sizeof(ship), sizeof(bullet), sizeof(networkShipData), sizeof(networkBullet));
	point3d start = {0, 0, 0};
	quaternion rot = {1, 0, 0, 0};
	shipTemplates[0] = newShip(120, 120, NULL, 0, start, rot, 0, 10, 1, 1, 0.07, 0.07, 0.07);//user
	shipTemplates[1] = newShip(100, 100, NULL, 0, start, rot, 0, 7, 1, 1, 0.05, 0.05, 0.05);//fighters
	shipTemplates[2] = newShip(100, 100, NULL, 0, start, rot, 0, 7, 1, 1, 0.05, 0.05, 0.05);
	shipTemplates[3] = newShip(2000, 2000, NULL, 0, start, rot, 0, 3, 0.05, 0.05, 0.005, 0.005, 0.005);//carriers
	shipTemplates[4] = newShip(2000, 2000, NULL, 0, start, rot, 0, 3, 0.05, 0.05, 0.005, 0.005, 0.005);
	setupNetworkListen();
	{
		point3d cs = {5000000, 5000000, 5000000};
		quaternion cr = {1, 0, 0, 0};
		aiData dat;
		dat.fighter.mode = -1;
		//addSpawnQueue(cs, cr, 1, idleAi, dat, 1, NULL, 0);
		/*ability* myAbilities = calloc(1, sizeof(ability));
		myAbilities[0].act = ability_AutoMachinegun;
		myAbilities[0].max = 2;
		myAbilities[0].cooldown = 2;
		cs[0]+=2000;
		cs[1]+=2000;
		addSpawnQueue(cs, cr, 4, destroyerAi, dat, 0, myAbilities, 1);
		cs[0]-=2000;
		cs[1]-=2000;
		for(int x = 0; x < 3; x++){
			cs[0]+=400;
			for(int y = 0; y < 3; y++){
				cs[1]+=400;
				for(int z = 0; z < 3; z++){
					cs[2]+=800;
					cs[2]+=400;
					ability* myAbilities = calloc(1, sizeof(ability));
					myAbilities[0].act = ability_Machinegun;
					myAbilities[0].max = 2;
					myAbilities[0].cooldown = 2;
					addSpawnQueue(cs, cr, 1, fighterAi, dat, 1, myAbilities, 1);
					cs[2]-=400;
					myAbilities = calloc(1, sizeof(ability));
					myAbilities[0].act = ability_Machinegun;
					myAbilities[0].max = 2;
					myAbilities[0].cooldown = 2;
					addSpawnQueue(cs, cr, 2, fighterAi, dat, 0, myAbilities, 1);

				}
				cs[2]-=800*3;
			}
			cs[1]-=3*400;
		}*/

	} 
	while(1){
		delay(40);
		handleHumanSpawnRequests();
		clearSpawnQueue();
		createGrid();
		tickShipAi();//Added this so that stuff would be more balanced.
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
			myAbilities[0].act = ability_AutoMachinegun;
			myAbilities[0].max = 2;
			myAbilities[0].cooldown = 2;
			addSpawnQueue(cs, cr, 2, humanAi, dat, 1, myAbilities, 1);
			userList[userIdx].myControls.spawn = -1;
			
		}
	}
}
