#include <stdio.h>
#include <stdlib.h>

#include "user.h"
#include "collisionMap.h"
#include "ship.h"
#include "sendMessage.h"
#include "bullet.h"

#define MAXUSERS 20

user userList[MAXUSERS];
int userCount = 0;

networkShipData getNetworkShipData(ship* target);

void createNewUser(unsigned long ip){
	if(userCount == MAXUSERS) return;
	userList[userCount].addr.sin_family=AF_INET;
	userList[userCount].addr.sin_port=htons(5999);
	userList[userCount].addr.sin_addr.s_addr=ip;
	userList[userCount].myControls.spawn = -1;
	userCount++;
	printf("added new user %ld\n", ip);
}
void sendAllUserData(){
	for(int userIdx = 0; userIdx < userCount; userIdx++){
		sendView(&(userList[userIdx]));
	}
}
void sendView(user* destination){
	static char data[100000] = {'S', 'C', 'N', 0};//fixme magic number overflow
	int dloc = 3;//next free place to write to
	ship** draw = NULL;
	int quantity = getShipsWithin(&draw, destination->myPosition, VIEW_DISTANCE);
	P3DEQUAL((int*)&(data[dloc]), destination->myPosition);
	dloc+=sizeof(point3d);
	QUATEQUAL((double*)&(data[dloc]), destination->myRotation);
	dloc+=sizeof(quaternion);
	*(int*)&(data[dloc]) = quantity;
	dloc+=sizeof(int);
	for(int shipIdx = 0; shipIdx < quantity; shipIdx++){
		*(networkShipData*)&(data[dloc]) = getNetworkShipData(draw[shipIdx]);
		dloc+=sizeof(networkShipData);
	}
	free(draw);
	int* netBulletCount = (int*)&(data[dloc]);
	*netBulletCount = 0;
	dloc+=sizeof(int);
	for(int bIdx = 0; bIdx < bulletCount; bIdx++){
		if(DISTANCE(bulletList[bIdx].myPos, destination->myPosition) < VIEW_DISTANCE){//FIXME should have some sort of optimization (octree)
			*(networkBullet*)&(data[dloc]) = getBulletNetworkData(&(bulletList[bIdx]));
			(*netBulletCount) = (*netBulletCount)+1;
			dloc+=sizeof(networkBullet);
		}
	}
	strcpy(&(data[dloc]), "END\0");
	dloc+=4;
	sendMessage(&(destination->addr), data, dloc);
}
//FIXME take in a networkShipData ptr to write to
networkShipData getNetworkShipData(ship* target){
	networkShipData ret;
	P3DEQUAL(ret.myPosition, target->myPosition);
	//quatEqual(ret.myRotation, target->myRotation);
	for(int dim = 0; dim < 4; dim++){
		ret.myRotation[dim] = target->myRotation[dim]*127;
	}
	ret.type = target->type;
	ret.color = target->color;
	return ret;
}
