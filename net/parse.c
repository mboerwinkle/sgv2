#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ship.h"
#include "../user.h"
void netParse(int userIdx, char* msg){
	char prefix[4] = {msg[0], msg[1], msg[2], 0};
	if(!strcmp(prefix, "CTL")){
		memcpy(&(userList[userIdx].myControls), msg+3, sizeof(controlMap));
	}else if(!strcmp(prefix, "SPN")){
		puts("Spawning");
		point3d loc = {5000000, 5000000, 5000000};
		quaternion rot = {1, 0, 0, 0};
		spawnHumanShip(userIdx, loc, rot, 0);
	}
	
}
