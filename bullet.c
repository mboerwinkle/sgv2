#include <stdio.h>
#include <stdlib.h>
#include "bullet.h"
bullet* bulletList = NULL;
int bulletCountMax;
int bulletCount;

void tickBullets(){
	for(int bIdx = 0; bIdx < bulletCount; bIdx++){
		bullet* b = &(bulletList[bIdx]);
		for(int dim = 0; dim < 3; dim++){
			b->myPos[dim]+=b->myVector[dim];
		}
		//collide
	}
}

void newBullet(char type, point3d pos, vector dir){
	puts("making bullet");
	if(bulletCountMax == bulletCount){
		bulletCountMax+=20;
		bulletList = realloc(bulletList, sizeof(bullet)*bulletCountMax);
	}
	p3dEqual(bulletList[bulletCount].myPos, pos);
	vecEqual(bulletList[bulletCount].myVector, dir);
	bulletList[bulletCount].type = type;
	if(type == 0){
		bulletList[bulletCount].lifetime = 40;
		bulletList[bulletCount].damage = 1;
	}else{
		puts("unknown bullet type");
	}
	bulletCount++;
}

networkBullet getBulletNetworkData(bullet* target){
	networkBullet ret;
	p3dEqual(ret.origin, target->myPos);
	for(int dim = 0; dim < 3; dim++){
		ret.dir[dim] = target->myVector[dim];
	}
	ret.type = target->type;
	return ret;
}
