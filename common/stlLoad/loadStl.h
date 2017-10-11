#ifndef LOADSTL_H
#define LOADSTL_H
#include <inttypes.h>
#include "../geo/geo.h"
struct loadTri{//Used just for loading from stl files.
	float vec[3];
	float p1[3];
	float p2[3];
	float p3[3];
	uint16_t attr;
}__attribute__((packed));

struct tri{
	int v[3];//Vertices
	vector normal;
};
typedef int edge[2];
typedef struct model{
	double radius;

	int pointCount;//All The Points
	vector* points;

	int triangleCount;//All The Triangles
	struct tri* triangles;

	int edgeCount;//All The Edges
	edge* edges;
}model;

typedef struct modelReference{
	model* dat;
	vector* rotatedPoints;
	int upToDate;
}modelReference;

void rotatePoints(modelReference* ref, quaternion rotation);

extern int modelCount;
extern model* models;
//loads models
extern void loadModels(char* dest);
extern void triNormal(struct loadTri* t);
#endif
