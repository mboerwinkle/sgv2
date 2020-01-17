#ifndef GEO_H
#define GEO_H
#include <string.h>
#include <math.h>
typedef double vector[3];
typedef float vectorf[3];
typedef int point3d[3];
typedef double quaternion[4];

#define DISTANCE(a, b) sqrt((a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1])+(a[2]-b[2])*(a[2]-b[2]))//FIXME actually use
#define CROSS(v1, v2, dest)\
        dest[0]=v1[1]*v2[2]-v1[2]*v2[1];\
        dest[1]=v1[2]*v2[0]-v1[0]*v2[2];\
        dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1, v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define VECLEN(v1) sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2])
#define QUATLEN(q1) sqrt(q1[0]*q1[0]+q1[1]*q1[1]+q1[2]*q1[2]+q1[3]*q1[3])
//WARNING: do not use if save is a or save is b
#define QUATMULT(a, b, save)\
	save[0]=(b[0] * a[0] - b[1] * a[1] - b[2] * a[2] - b[3] * a[3]);\
	save[1]=(b[0] * a[1] + b[1] * a[0] + b[2] * a[3] - b[3] * a[2]);\
	save[2]=(b[0] * a[2] - b[1] * a[3] + b[2] * a[0] + b[3] * a[1]);\
	save[3]=(b[0] * a[3] + b[1] * a[2] - b[2] * a[1] + b[3] * a[0]);
//FIXME still have to do Sub, rotvec
#define VECEQUAL(a, b)\
	memcpy(a, b, sizeof(vector));
#define VECFEQUAL(a, b)\
	memcpy(a, b, sizeof(vectorf));
#define P3DEQUAL(a, b)\
	memcpy(a, b, sizeof(point3d));
#define QUATEQUAL(a, b)\
	memcpy(a, b, sizeof(quaternion));

extern void vecNormalize(vector a);
extern void vecfNormalize(vectorf a);

extern void quatMult(double* a, double* b, double* save);
extern void lerp(quaternion ret, quaternion one, quaternion two, double t);
extern void quatNormalize(quaternion r);
extern void rotVector(vector vec, quaternion rot, vector output);
extern void rotfVector(vectorf vec, quaternion rot, vectorf output);
extern void generateRotationMatrix(quaternion rot, double *M);
#endif
