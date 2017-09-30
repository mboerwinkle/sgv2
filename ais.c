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
void fighterAi(ship* target, aiData* data){
}
void destroyerAi(ship* target, aiData* data){
}
