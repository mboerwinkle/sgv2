#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ship.h"

void humanAi(ship* target, aiData* data){
	user* me = data->human.myuser;
	controlMap ctl = me->myControls;
	p3dEqual(me->myPosition, target->myPosition);
	quatEqual(me->myRotation, target->myRotation);
	vector pointer = {-0.866, 0, 0.5};
	pointer[1]-=ctl.yaw;
	pointer[2]+=ctl.pitch;
	vecNormalize(pointer);//FIXME need interpolated side to side camera movement first
	rotVector(pointer, me->myRotation);
	me->myPosition[0] += 1.5*target->myModel->radius*pointer[0];//fixme clean
	me->myPosition[1] += 1.5*target->myModel->radius*pointer[1];
	me->myPosition[2] += 1.5*target->myModel->radius*pointer[2];
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
}
void turn(ship* target, double y, double z, double* pitch, double* roll, double* yaw){//This takes a vector giving a direction of desired motion and controls the ship. It figures out if yaw or pitch is better suited, then rolls to make it even better. Technically it is better to roll to a corner then use yaw and pitch. But we're not doing that.
	if(y == 0 && z == 0) return;
	int bias = 1;//1 for pitch turn, -1 for yaw turn. Kind of arbitrary, but it helps keep the roll code from having too much duplication
	if(fabs(y) > fabs(z)){//Yaw turn
		bias = -1;
		if(y > 0){
			*yaw = -1;
		}else{
			*yaw = 1;
		}
		if(y*z == 0){
			return;
	}else{//Pitch turn
		if(z > 0){
			*pitch = -1;
		}else{
			*pitch = 1;
		}
	}
	//roll code
	if(y == 0 || z == 0) return;//no need for a roll
	if(y*z*bias < 0){//Which way you need to roll alternates by quadrant and by which type of turn you are executing.
		*roll = -1;
	}
		*roll = 1;
	}
}
void fighterAi(ship* target, aiData* data){
	double throttle = 0.1, pitch = 0, roll = 0, yaw = 0;
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
	for(int sIdx = 0; sIdx < quantity; sIdx++){
		if(p3dDistance(target->myPosition, draw[sIdx]->myPosition) < (target->myModel->radius+draw[sIdx]->myModel->radius*2)){
			puts("too close turning away");
		}
	}
	
}
void destroyerAi(ship* target, aiData* data){
}
