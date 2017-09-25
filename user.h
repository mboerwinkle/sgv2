#ifndef USER_H
#define USER_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common/geo/geo.h"
#include "common/controlMap.h"


typedef struct user{
	struct sockaddr_in addr;
	point3d myPosition;
	quaternion myRotation;
	controlMap myControls;
}user;
extern user userList[];
extern int userCount;

extern void createNewUser(unsigned long ip);
extern void sendAllUserData();
extern void sendView(user* destination);
#endif
