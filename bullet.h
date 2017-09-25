#ifndef BULLET_H
#define BULLET_H
#include "common/geo/geo.h"

typedef struct bullet{
	point3d myPos;
	vector myVector;
	int lifetime;
	int damage;
}bullet;

extern bullet** bulletList;
extern int bulletCount;
void newBullet(char type, point3d pos, vector dir);


#endif
