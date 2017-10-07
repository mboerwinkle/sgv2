#include <stdio.h>
#include <stdlib.h>
#include "ship.h"
#include "collisionMap.h"
#include "collisions.h"
#include "bullet.h"
bullet* bulletList = NULL;
int bulletCountMax;
int bulletCount;

int bulletIntersectsShip(bullet* b, ship* s){
	vectorf origin;
	vectorf dir;
	for(int dim = 0; dim < 3; dim++){//FIXME vecequals
		dir[dim] = b->myVector[dim]*b->speed;
		origin[dim] = (b->myPos[dim]-s->myPosition[dim]);
	}
	quaternion revRot = {s->myRotation[0], -s->myRotation[1], -s->myRotation[2], -s->myRotation[3]};//FIXME revRot
	rotfVector(dir, revRot, dir);//this is fucked. FIXME
	rotfVector(origin, revRot, origin);
	for(int dim = 0; dim < 3; dim++){
		dir[dim]+=origin[dim];//This shouldn't exist. I change a vector to an end point here. but intersect_triangle turns it back. This is a big issue since bullets are effPrio#1
	}
	model* cModel = s->myModel;
	for(int triIdx = 0; triIdx < cModel->triangleCount; triIdx++){
		struct tri* cTri = &(cModel->triangles[triIdx]);
		if(intersect_triangle(origin, dir, cTri->p1, cTri->p2, cTri->p3)){
			puts("intersected");
			return 1;
		}
	}
	return 0;
}

void tickBullets(){
	for(int bIdx = 0; bIdx < bulletCount; bIdx++){
		bullet* b = &(bulletList[bIdx]);

		int intersected = 0;
		ship** draw = NULL;
		int quantity = getShipsWithin(&draw, b->myPos, b->speed);//FIXME see if you get better performance by getting ships within from the middle of the bullet and half the length of the vecotr
		for(int sidx = 0; sidx < quantity; sidx++){
			if(bulletIntersectsShip(b, draw[sidx])){
				draw[sidx]->hp-=b->damage;
				intersected = 1;
			}
		}
		free(draw);
		for(int dim = 0; dim < 3; dim++){
			b->myPos[dim]+=b->myVector[dim]*b->speed;
		}
		bulletList[bIdx].lifetime -= 1;
		if(bulletList[bIdx].lifetime <= 0 || intersected){
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
		bulletList[bulletCount].damage = 10;
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
