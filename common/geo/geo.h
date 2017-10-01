#ifndef GEO_H
#define GEO_H
#include <string.h>
typedef double vector[3];
typedef float vectorf[3];
typedef int point3d[3];
typedef double quaternion[4];

extern double p3dDistance(point3d a, point3d b);

extern void cross(vector a, vector b, vector save);
extern void crossf(vectorf a, vectorf b, vectorf save);
extern double dot(vector a, vector b);
extern double dotf(vectorf a, vectorf b);
extern void vecNormalize(vector a);
extern void vecfNormalize(vectorf a);

extern void vecEqual(vector a, vector b);
extern void vecfEqual(vectorf a, vectorf b);
extern void p3dEqual(point3d a, point3d b);
extern void quatEqual(quaternion a, quaternion b);

extern double vecLen(vector a);
extern double vecfLen(vectorf a);

extern void quatMult(double* a, double* b, double* save);
extern void lerp(quaternion ret, quaternion one, quaternion two, double t);
extern double quatLen(quaternion r);
extern void quatNormalize(quaternion r);
extern void rotVector(vector vec, quaternion rot);
extern void rotfVector(vectorf vec, quaternion rot, vectorf output);
extern void generateRotationMatrix(quaternion rot, double *M);
#endif
