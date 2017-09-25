#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"

//macrotize
void vecEqual(vector a, vector b){
	memcpy(a, b, sizeof(vector));
}
void p3dEqual(point3d a, point3d b){
	memcpy(a, b, sizeof(point3d));
}
void quatEqual(quaternion a, quaternion b){
	memcpy(a, b, sizeof(quaternion));
}


//append a to b and save to save
void quatMult(double* a, double* b, double* save){
	quaternion newTarg;//fixme dont have save. instead return an array
	newTarg[0]=(b[0] * a[0] - b[1] * a[1] - b[2] * a[2] - b[3] * a[3]);
	newTarg[1]=(b[0] * a[1] + b[1] * a[0] + b[2] * a[3] - b[3] * a[2]);
	newTarg[2]=(b[0] * a[2] - b[1] * a[3] + b[2] * a[0] + b[3] * a[1]);
	newTarg[3]=(b[0] * a[3] + b[1] * a[2] - b[2] * a[1] + b[3] * a[0]);
	quatNormalize(newTarg);//FIXME efficency does not need to fix every tick
	quatEqual(save, newTarg);
}
void lerp(quaternion ret, quaternion one, quaternion two, double t){
	quaternion temp;
	for(int x = 0; x < 4; x++){
		temp[x] = two[x]*t+one[x]*(1-t);
	}
	quatEqual(ret, temp);
}
double quatLen(quaternion r){//FIXME macrotize
	return sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]+r[3]*r[3]);
}
void quatNormalize(quaternion r){
	double val = quatLen(r);
	r[0]/=val;
	r[1]/=val;
	r[2]/=val;
	r[3]/=val;
}
void rotVector(vector vec, quaternion rot){
	quaternion pureVec = {0, vec[0], vec[1], vec[2]};
	quaternion revRot = {rot[0], -rot[1], -rot[2], -rot[3]};
	quatMult(rot, pureVec, pureVec);
	quatMult(pureVec, revRot, pureVec);
	vec[0] = pureVec[1];
	vec[1] = pureVec[2];
	vec[2] = pureVec[3];
}
/*
M[0]  M[4]  M[8]  M[12]
M[1]  M[5]  M[9]  M[13]
M[2]  M[6]  M[10] M[14]
M[3]  M[7]  M[11] M[15]
*/
void generateRotationMatrix(quaternion rot, double *M){
	M[0] = 1-2*rot[2]*rot[2]-2*rot[3]*rot[3];
	M[1] = 2*rot[1]*rot[2]+2*rot[0]*rot[3];
	M[2] = 2*rot[1]*rot[3]-2*rot[0]*rot[2];
	M[3] = 0;
	M[4] = 2*rot[1]*rot[2]-2*rot[0]*rot[3];
	M[5] = 1-2*rot[1]*rot[1]-2*rot[3]*rot[3];
	M[6] = 2*rot[2]*rot[3]+2*rot[0]*rot[1];
	M[7] = 0;
	M[8] = 2*rot[1]*rot[3]+2*rot[0]*rot[2];
	M[9] = 2*rot[2]*rot[3]-2*rot[0]*rot[1];
	M[10] = 1-2*rot[1]*rot[1]-2*rot[2]*rot[2];
	M[11] = 0;
	M[12] = 0;
	M[13] = 0;
	M[14] = 0;
	M[15] = 1;
}
