#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"

void cross(vector a, vector b, vector save){
	vector ret;
	ret[0] = a[1]*b[2]-a[2]*b[1];
	ret[1] = a[2]*b[0]-a[0]*b[2];
	ret[2] = a[0]*b[1]-a[1]*b[0];
	vecEqual(save, ret);
}
void crossf(vectorf a, vectorf b, vectorf save){
	vectorf ret;
	ret[0] = a[1]*b[2]-a[2]*b[1];
	ret[1] = a[2]*b[0]-a[0]*b[2];
	ret[2] = a[0]*b[1]-a[1]*b[0];
	vecfEqual(save, ret);
}

double dot(vector a, vector b){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
double dotf(vectorf a, vectorf b){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void vecNormalize(vector a){
	double val = vecLen(a);
	a[0]/=val;
	a[1]/=val;
	a[2]/=val;
}
void vecfNormalize(vectorf a){
	double val = vecfLen(a);
	a[0]/=val;
	a[1]/=val;
	a[2]/=val;
}

double p3dDistance(point3d a, point3d b){
	point3d d = {a[0]-b[0], a[1]-b[1], a[2]-b[2]};
	return sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
}

//macrotize
void vecEqual(vector a, vector b){
	memcpy(a, b, sizeof(vector));
}
void vecfEqual(vectorf a, vectorf b){
	memcpy(a, b, sizeof(vectorf));
}
void p3dEqual(point3d a, point3d b){
	memcpy(a, b, sizeof(point3d));
}
void quatEqual(quaternion a, quaternion b){
	memcpy(a, b, sizeof(quaternion));
}

//FIXME macrotize
double vecLen(vector a){
	return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}
double vecfLen(vectorf a){
	return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
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
