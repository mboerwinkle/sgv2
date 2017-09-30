#ifndef BULLET_H
#define BULLET_H
#include "common/geo/geo.h"
#include "common/networkShipData.h"


typedef struct bullet{
	point3d myPos;
	vector myVector;
	int lifetime;
	int damage;
	char type;
}bullet;

extern bullet** bulletList;
extern int bulletCount;
void newBullet(char type, point3d pos, vector dir);
networkBullet getBulletNetworkData(bullet* target);


#endif
