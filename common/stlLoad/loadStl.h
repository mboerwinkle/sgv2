#ifndef LOADSTL_H
#define LOADSTL_H
#include <inttypes.h>
struct tri{
	float vec[3];
	float p1[3];
	float p2[3];
	float p3[3];
	uint16_t attr;
}__attribute__((packed));
typedef struct model{
	uint32_t triangleCount;
	double radius;
	struct tri* triangles;
}model;
extern int modelCount;
extern model* models;
//loads models
extern void loadModels(char* dest);
extern void triNormal(struct tri* t);
#endif
