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
	vector dir = {5000, 0, 0};
	int rint = random();
	dir[1] = ((short*)(&rint))[0]%500;
	dir[2] = ((short*)(&rint))[1]%500;
	vecNormalize(dir);
	rotVector(dir, target->myRotation);
	point3d bulletStart;
	for(int dim = 0; dim < 3; dim++){
		bulletStart[dim] = target->myPosition[dim]+(dir[dim]*(target->myModel->radius+5));
	}
	newBullet(0, bulletStart, dir, 500);
}
