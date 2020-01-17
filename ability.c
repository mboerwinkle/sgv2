#include <stdio.h>
#include <stdlib.h>
#include "ability.h"
#include "bullet.h"
#include "collisionMap.h"

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
	rotVector(dir, target->myRotation, dir);
	point3d bulletStart;
	for(int dim = 0; dim < 3; dim++){
		bulletStart[dim] = target->myPosition[dim]+(int)(dir[dim]*(target->myModel.dat->radius+target->speed+5));
	}
	newBullet(0, bulletStart, dir, 500);
}

void ability_AutoMachinegun(ship* target){
	ship** draw = NULL;
	int quantity = getShipsWithin(&draw, target->myPosition, VIEW_DISTANCE);
	ship* best = NULL;
	double score = INFINITY;
	for(int sIdx = 0; sIdx < quantity; sIdx++){
		if(draw[sIdx]->color == target->color) continue;
		double newscore = DISTANCE(target->myPosition, draw[sIdx]->myPosition);
		if(newscore < score){
			score = newscore;
			best = draw[sIdx];
		}
	}
	free(draw);
	if(best == NULL) return;
	vector dir;
	for(int dim = 0; dim < 3; dim++){
		dir[dim] = best->myPosition[dim]-target->myPosition[dim];
	}
	vecNormalize(dir);
	point3d bulletStart;
	for(int dim = 0; dim < 3; dim++){
		bulletStart[dim] = target->myPosition[dim]+(int)(dir[dim]*(target->myModel.dat->radius+target->maxSpeed+5));
	}
	newBullet(0, bulletStart, dir, 500);
}
