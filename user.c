#include <stdio.h>
#include <stdlib.h>

#include "user.h"
#include "ship.h"
#include "sendMessage.h"

#define MAXUSERS 20
#define VIEW_DISTANCE 100000
user userList[MAXUSERS];
int userCount = 0;

networkShipData getNetworkShipData(ship* target);

void createNewUser(unsigned long ip){
	if(userCount == MAXUSERS) return;
	userList[userCount].addr.sin_family=AF_INET;
	userList[userCount].addr.sin_port=htons(5999);
	userList[userCount].addr.sin_addr.s_addr=ip;
	userCount++;
	printf("added new user %ld\n", ip);
}
void sendAllUserData(){
	for(int userIdx = 0; userIdx < userCount; userIdx++){
		sendView(&(userList[userIdx]));
	}
}
void sendView(user* destination){
	static char data[100000] = "SCN";//fixme magic number overflow
	int dloc = 3;//next free place to write to
	ship** draw;
	int quantity = getShipsWithin(&draw, destination->myPosition, VIEW_DISTANCE);
	p3dEqual((int*)&(data[dloc]), destination->myPosition);
	dloc+=sizeof(point3d);
	quatEqual((double*)&(data[dloc]), destination->myRotation);
	dloc+=sizeof(quaternion);
	*(int*)&(data[dloc]) = quantity;
	dloc+=sizeof(int);
	for(int shipIdx = 0; shipIdx < quantity; shipIdx++){
		*(networkShipData*)&(data[dloc]) = getNetworkShipData(draw[shipIdx]);
		dloc+=sizeof(networkShipData);
	}
	strcpy(&(data[dloc]), "END\0");
	dloc+=4;
	free(draw);
	sendMessage(&(destination->addr), data, dloc);
}
//FIXME take in a networkShipData ptr to write to
networkShipData getNetworkShipData(ship* target){
	networkShipData ret;
	p3dEqual(ret.myPosition, target->myPosition);
	quatEqual(ret.myRotation, target->myRotation);
	ret.type = target->type;
	ret.color = target->color;
	strcpy(ret.name, target->name);
	return ret;
}
