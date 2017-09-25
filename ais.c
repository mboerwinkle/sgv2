#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ship.h"

void humanAi(ship* target, aiData* data){
	user* me = data->human.myuser;
	p3dEqual(me->myPosition, target->myPosition);
	quatEqual(me->myRotation, target->myRotation);
	vector pointer = {0.707, 0, -0.707};
	rotVector(pointer, me->myRotation);
	me->myPosition[0] -= target->myModel->radius*pointer[0];//fixme clean
	me->myPosition[1] -= target->myModel->radius*pointer[1];
	me->myPosition[2] -= target->myModel->radius*pointer[2];
	controlMap ctl = me->myControls;
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
/*	for(int x = 0; x < MAXMODULES; x++){
		if(myMod[x] == NULL) continue;
		if(ctl.fire & (1<<x)){
			myMod[x]->tick(1);
		}else{
			myMod[x]->tick(0);
		}
	}*/
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
}
void fighterAi(ship* target, aiData* data){
}
void destroyerAi(ship* target, aiData* data){
}
