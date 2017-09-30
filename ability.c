#include <stdio.h>
#include <stdlib.h>
#include "ability.h"
#include "bullet.h"

void applyAbility(ability* targ, int fire, ship* myShip){
	if(targ->cooldown > 0){
		targ->cooldown--;
		return;
	}
	if(fire){
		targ->act(myShip);
		targ->cooldown = targ->max;
	}
}

void ability_Machinegun(ship* target){
	vector dir = {1, 0, 0};
	rotVector(dir, target->myRotation);
	dir[0]*=500;
	dir[1]*=500;
	dir[2]*=500;
	newBullet(0, target->myPosition, dir);
}
