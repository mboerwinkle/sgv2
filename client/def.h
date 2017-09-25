#define CONTROLLERAXISMAX 32768.0
#define DEADZONE 8000
#define MSGSIZE 3000

#include "../common/geo/geo.h"
extern quaternion myRot;
extern quaternion targRot;
extern point3d myPos;
extern int has_joystick;
extern int sockfd;
extern struct sockaddr_in recvAddr, sendAddr;
