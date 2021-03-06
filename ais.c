#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "collisionMap.h"
#include "ship.h"

void idleAi(ship* target, aiData* data){
}

void humanAi(ship* target, aiData* data){
	//if(target->speed != 0) printf("%d %d %d\n", target->myPosition[0], target->myPosition[1], target->myPosition[2]);
	user* me = data->human.myuser;
	controlMap ctl = me->myControls;
	P3DEQUAL(me->myPosition, target->myPosition);
	QUATEQUAL(me->myRotation, target->myRotation);
	vector pointer = {-0.866, 0, 0.5};
	pointer[1]-=ctl.yaw;
	pointer[2]+=ctl.pitch;
	vecNormalize(pointer);//FIXME need interpolated side to side camera movement first
	rotVector(pointer, me->myRotation, pointer);
	me->myPosition[0] += 3*target->myModel.dat->radius*pointer[0];//fixme clean
	me->myPosition[1] += 3*target->myModel.dat->radius*pointer[1];
	me->myPosition[2] += 3*target->myModel.dat->radius*pointer[2];
	//ctl.accel
	double accel = target->accel;
	double decel = target->decel;
	double maxSpeed = target->maxSpeed;
	if(target->speed+accel < maxSpeed*ctl.accel){
		target->speed+=accel;
	}else if(target->speed-decel > maxSpeed*ctl.accel){
		target->speed-=decel;
	}
	else{
		target->speed = maxSpeed*ctl.accel;
	}
	//math.stackexchange.com/questions/40164/how-do-you-rotate-a-vector-by-a-unit-quaternion#40169
	quaternion* rot = &(target->myRotation);
	if(ctl.yaw != 0){
		double angleChg = ctl.yaw*target->yawSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, 0, sin(0.5*angleChg)};
		quatMult(*rot, addRot, *rot);
	}
	if(ctl.roll != 0){
		double angleChg = ctl.roll*target->rollSpeed;
		quaternion addRot = {cos(0.5*angleChg), sin(0.5*angleChg), 0, 0};
		quatMult(*rot, addRot, *rot);
	}
	if(ctl.pitch != 0){
		double angleChg = ctl.pitch*target->pitchSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, sin(0.5*angleChg), 0};
		quatMult(*rot, addRot, *rot);
	}
	for(int abiIdx = 0; abiIdx < target->abilityCount; abiIdx++){
		int status = 0;
		if(ctl.fire != -1 && ctl.fire%target->abilityCount == abiIdx) status = 1;//FIXME INelegant
		applyAbility(&(target->myAbilities[abiIdx]), status, target);
	}
	quatNormalize(*rot);
	target->myModel.upToDate = 0;
}
void turn(ship* target, double y, double z, double* pitch, double* roll, double* yaw){//This takes a vector giving a direction of desired motion and controls the ship. It figures out if yaw or pitch is better suited, then rolls to make it even better. Technically it is better to roll to a corner then use yaw and pitch. But we're not doing that.
	if(y == 0 && z == 0){
		*pitch = 1;//break out of some nasty behavior
		return;
	}
	int bias = 1;//1 for pitch turn, -1 for yaw turn. Kind of arbitrary, but it helps keep the roll code from having too much duplication
	if(fabs(y) > fabs(z)){//Yaw turn
		bias = -1;
		if(y > 0){
			*yaw = -1;
		}else{
			*yaw = 1;
		}
	}else{//Pitch turn
		if(z > 0){
			*pitch = 1;
		}else{
			*pitch = -1;
		}
	}
	//roll code
	if(y == 0 || z == 0) return;//no need for a roll
	if(y*z*bias < 0){//Which way you need to roll alternates by quadrant and by which type of turn you are executing.
		*roll = -1;
	}else{
		*roll = 1;
	}
}
void fighterAi(ship* target, aiData* data){
	double throttle = 1, pitch = 0, roll = 0, yaw = 0;
	double accel = target->accel;
	double decel = target->decel;
	double maxSpeed = target->maxSpeed;
	if(target->speed+accel < maxSpeed*throttle){
		target->speed+=accel;
	}else if(target->speed-decel > maxSpeed*throttle){
		target->speed-=decel;
	}
	else{
		target->speed = maxSpeed*throttle;
	}
	ship** draw = NULL;
	int quantity = getShipsWithin(&draw, target->myPosition, VIEW_DISTANCE);
	ship* best;
	double score = INFINITY;
	int reason = -1;//-1 we've got nothing. 0 closest friend 1 closest enemy 2 proximity
	for(int sIdx = 0; sIdx < quantity; sIdx++){
		if(draw[sIdx] == target) continue;
		double radius = draw[sIdx]->myModel.dat->radius;
		double distance = DISTANCE(target->myPosition, draw[sIdx]->myPosition);//calculate distance

		if(reason >= 3) continue;
		//test reason 2
		if(distance < (target->myModel.dat->radius*3+radius)){//only multiply your radius so you play nice with big slow ships.
			if(reason != 2){
				score = INFINITY;
				reason = 2;
			}
			if(distance-radius < score){
				best = draw[sIdx];
				score = distance-radius;
			}
		}
		if(reason >= 2) continue;
		//test reason 1
		if(draw[sIdx]->color != target->color){
			if(reason != 1){
				score = INFINITY;
				reason = 1;
			}
			if(distance-radius < score){
				best = draw[sIdx];
				score = distance-radius;
			}
		}
		
		if(reason >= 1) continue;
		//test reason 0
		reason = 0;
		if(distance-radius < score){
			best = draw[sIdx];
			score = distance-radius;
		}
	}
	free(draw);
	vector relLoc;
	if(reason >= 0){
		for(int dim = 0; dim < 3; dim++){//SUB
			relLoc[dim] = best->myPosition[dim]-target->myPosition[dim];
		}
		quaternion revRot = {target->myRotation[0], -target->myRotation[1], -target->myRotation[2], -target->myRotation[3]};
		rotVector(relLoc, revRot, relLoc);
	}
	if(reason == 2){//proximity
		turn(target, -relLoc[1], -relLoc[2], &pitch, &roll, &yaw);//turn away from collisions
	}else if(reason == 1){//enemy
		turn(target, relLoc[1], relLoc[2], &pitch, &roll, &yaw);
	//	if(fabs(relLoc[1])+fabs(relLoc[2]) < relLoc[0]){//FIXME not precise
			applyAbility(&(target->myAbilities[0]), 1, target);
	//	}
	}else if(reason == 0){
		turn(target, relLoc[1]+500, relLoc[2], &pitch, &roll, &yaw);
	}
	quaternion* rot = &(target->myRotation);
	if(yaw != 0){//FIXME make standard apply rotations function
		double angleChg = yaw*target->yawSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, 0, sin(0.5*angleChg)};
		quatMult(*rot, addRot, *rot);
	}
	if(roll != 0){
		double angleChg = roll*target->rollSpeed;
		quaternion addRot = {cos(0.5*angleChg), sin(0.5*angleChg), 0, 0};
		quatMult(*rot, addRot, *rot);
	}
	if(pitch != 0){
		double angleChg = pitch*target->pitchSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, sin(0.5*angleChg), 0};
		quatMult(*rot, addRot, *rot);
	}
	quatNormalize(*rot);
	target->myModel.upToDate = 0;
}
void destroyerAi(ship* target, aiData* data){//FIXME. this is just an "always fire" hack of the fighter ai
	double throttle = 1, pitch = 0, roll = 0, yaw = 0;
	double accel = target->accel;
	double decel = target->decel;
	double maxSpeed = target->maxSpeed;
	if(target->speed+accel < maxSpeed*throttle){
		target->speed+=accel;
	}else if(target->speed-decel > maxSpeed*throttle){
		target->speed-=decel;
	}
	else{
		target->speed = maxSpeed*throttle;
	}
	ship** draw = NULL;
	int quantity = getShipsWithin(&draw, target->myPosition, VIEW_DISTANCE);
	ship* best;
	double score = INFINITY;
	int reason = -1;//-1 we've got nothing. 0 closest friend 1 closest enemy 2 proximity
	for(int sIdx = 0; sIdx < quantity; sIdx++){
		if(draw[sIdx] == target) continue;
		double radius = draw[sIdx]->myModel.dat->radius;
		double distance = DISTANCE(target->myPosition, draw[sIdx]->myPosition);//calculate distance

		if(reason >= 3) continue;
		//test reason 2
		if(distance < (target->myModel.dat->radius+radius)*4){
			if(reason != 2){
				score = INFINITY;
				reason = 2;
			}
			if(distance-radius < score){
				best = draw[sIdx];
				score = distance-radius;
			}
		}
		if(reason >= 2) continue;
		//test reason 1
		if(draw[sIdx]->color != target->color){
			if(reason != 1){
				score = INFINITY;
				reason = 1;
			}
			if(distance-radius < score){
				best = draw[sIdx];
				score = distance-radius;
			}
		}
		
		if(reason >= 1) continue;
		//test reason 0
		reason = 0;
		if(distance-radius < score){
			best = draw[sIdx];
			score = distance-radius;
		}
	}
	free(draw);
	vector relLoc;
	if(reason >= 0){
		for(int dim = 0; dim < 3; dim++){//SUB
			relLoc[dim] = best->myPosition[dim]-target->myPosition[dim];
		}
		quaternion revRot = {target->myRotation[0], -target->myRotation[1], -target->myRotation[2], -target->myRotation[3]};
		rotVector(relLoc, revRot, relLoc);
	}
	applyAbility(&(target->myAbilities[0]), 1, target);
	if(reason == 2){//proximity
		turn(target, -relLoc[1], -relLoc[2], &pitch, &roll, &yaw);//turn away from collisions
	}else if(reason == 1){//enemy
		turn(target, relLoc[1], relLoc[2], &pitch, &roll, &yaw);
	}else if(reason == 0){
		turn(target, relLoc[1]+500, relLoc[2], &pitch, &roll, &yaw);
	}
	quaternion* rot = &(target->myRotation);
	if(yaw != 0){//FIXME make standard apply rotations function
		double angleChg = yaw*target->yawSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, 0, sin(0.5*angleChg)};
		quatMult(*rot, addRot, *rot);
	}
	if(roll != 0){
		double angleChg = roll*target->rollSpeed;
		quaternion addRot = {cos(0.5*angleChg), sin(0.5*angleChg), 0, 0};
		quatMult(*rot, addRot, *rot);
	}
	if(pitch != 0){
		double angleChg = pitch*target->pitchSpeed;
		quaternion addRot = {cos(0.5*angleChg), 0, sin(0.5*angleChg), 0};
		quatMult(*rot, addRot, *rot);
	}
		quatNormalize(*rot);
	target->myModel.upToDate = 0;
}
