#include <stdio.h>
#include <stdlib.h>


#include "bullet.h"
#include "ship.h"
#include "common/geo/geo.h"
#include "common/stlLoad/loadStl.h"
#include "delay/delay.h"
#include "net/listen.h"

unsigned int tickCount = 0;
ship shipTemplates[3];//FIXME put in new function dedicated to generateing types (from file?)
int main(){
	point3d start = {0, 0, 0};
	quaternion rot = {1, 0, 0, 0};
	shipTemplates[0] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.1, 0.1, 0.1);
	shipTemplates[1] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.1, 0.1, 0.1);
	shipTemplates[2] = newShip(100, 100, NULL, 0, start, rot, 0, 10, 1, 1, 0.1, 0.1, 0.1);
	point3d cs = {5000000, 5000000, 5000000};
	quaternion cr = {1, 0, 0, 0};
	spawnComputerShip(cs, cr, 2);
	setupNetworkListen();
	loadModels("common/models/");
	while(1){
		delay(40);
		sendAllUserData();
		tickShips();
		tickCount++;
//		checkCollisions(shipList);
//		tickBullets(bulletList);
		
	}
}
