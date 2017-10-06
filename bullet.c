#include <stdio.h>
#include <stdlib.h>
#include "ship.h"
#include "collisionMap.h"
#include "bullet.h"
bullet* bulletList = NULL;
int bulletCountMax;
int bulletCount;

int bulletIntersectsShip(bullet* b, ship* s){
	return 0;
}

void tickBullets(){
	for(int bIdx = 0; bIdx < bulletCount; bIdx++){
		bullet* b = &(bulletList[bIdx]);
		for(int dim = 0; dim < 3; dim++){
			b->myPos[dim]+=b->myVector[dim]*b->speed;
		}
		
		ship** draw = NULL;
		int quantity = getShipsWithin(&draw, b->myPos, VECLEN(b->myVector));
		for(int sidx = 0; sidx < quantity; sidx++){
			if(bulletIntersectsShip(b, draw[sidx])){
				draw[sidx]->hp-=b->damage;
			}
		}
		free(draw);

		bulletList[bIdx].lifetime -= 1;
		if(bulletList[bIdx].lifetime <= 0){
			bulletList[bIdx] = bulletList[bulletCount-1];
			bulletCount--;
			bIdx--;
		}
	}
}

void newBullet(char type, point3d pos, vector dir, int speed){
	if(bulletCountMax == bulletCount){
		bulletCountMax+=20;
		bulletList = realloc(bulletList, sizeof(bullet)*bulletCountMax);
	}
	p3dEqual(bulletList[bulletCount].myPos, pos);
	vecEqual(bulletList[bulletCount].myVector, dir);
	bulletList[bulletCount].type = type;
	bulletList[bulletCount].speed = speed;
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
		ret.dir[dim] = target->myVector[dim]*127;
	}
	ret.type = target->type;
	return ret;
}
