#ifndef GEO_H
#define GEO_H
#include <string.h>
typedef double vector[3];
typedef float floatVector[3];
typedef int point3d[3];
typedef double quaternion[4];


extern void vecEqual(vector a, vector b);
extern void p3dEqual(point3d a, point3d b);
extern void quatEqual(quaternion a, quaternion b);

extern double vecLen(vector a);
extern double floatVecLen(floatVector a);

extern void quatMult(double* a, double* b, double* save);
extern void lerp(quaternion ret, quaternion one, quaternion two, double t);
extern double quatLen(quaternion r);
extern void quatNormalize(quaternion r);
extern void rotVector(vector vec, quaternion rot);
extern void generateRotationMatrix(quaternion rot, double *M);
#endif
