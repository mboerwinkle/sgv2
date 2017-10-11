#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"
void vecNormalize(vector a){
	double val = VECLEN(a);
	a[0]/=val;
	a[1]/=val;
	a[2]/=val;
}
void vecfNormalize(vectorf a){
	double val = VECLEN(a);
	a[0]/=val;
	a[1]/=val;
	a[2]/=val;
}

//append a to b and save to save
void quatMult(double* a, double* b, double* save){
	quaternion newTarg;//fixme dont have save. instead return an array
	newTarg[0]=(b[0] * a[0] - b[1] * a[1] - b[2] * a[2] - b[3] * a[3]);
	newTarg[1]=(b[0] * a[1] + b[1] * a[0] + b[2] * a[3] - b[3] * a[2]);
	newTarg[2]=(b[0] * a[2] - b[1] * a[3] + b[2] * a[0] + b[3] * a[1]);
	newTarg[3]=(b[0] * a[3] + b[1] * a[2] - b[2] * a[1] + b[3] * a[0]);
	QUATEQUAL(save, newTarg);
}
void lerp(quaternion ret, quaternion one, quaternion two, double t){
	quaternion temp;
	for(int x = 0; x < 4; x++){
		temp[x] = two[x]*t+one[x]*(1-t);
	}
	QUATEQUAL(ret, temp);
}
void quatNormalize(quaternion r){
	double val = QUATLEN(r);
	r[0]/=val;
	r[1]/=val;
	r[2]/=val;
	r[3]/=val;
}
void rotVector(vector vec, quaternion rot, vector output){
	double mag = VECLEN(vec);
	quaternion pureVec = {0, vec[0]/mag, vec[1]/mag, vec[2]/mag};
	quaternion revRot = {rot[0], -rot[1], -rot[2], -rot[3]};
	quaternion temp;
	QUATMULT(rot, pureVec, temp);
	QUATMULT(temp, revRot, pureVec);
	output[0] = pureVec[1]*mag;
	output[1] = pureVec[2]*mag;
	output[2] = pureVec[3]*mag;
}
void rotfVector(vectorf vec, quaternion rot, vectorf output){
	double mag = VECLEN(vec);
	quaternion pureVec = {0, vec[0]/mag, vec[1]/mag, vec[2]/mag};
	quaternion revRot = {rot[0], -rot[1], -rot[2], -rot[3]};
	quaternion temp;
	QUATMULT(rot, pureVec, temp);
	QUATMULT(temp, revRot, pureVec);
	output[0] = pureVec[1]*mag;
	output[1] = pureVec[2]*mag;
	output[2] = pureVec[3]*mag;
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
