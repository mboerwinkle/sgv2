#ifndef SHIP_H
#define SHIP_H
#include "common/geo/geo.h"
#include "ability.h"
#include "user.h"
#include "common/stlLoad/loadStl.h"
#include "common/networkShipData.h"
typedef struct ship ship;
typedef struct aiData aiData;
typedef struct ability ability;

extern ship* spawnQueue;
extern int spawnQueueSize;
extern int spawnQueueCapacity;

extern ship* shipList;
extern int shipCountMax;
extern int shipCount;

extern void tickShipAi();
extern void tickShips();
extern void killShips();

extern void clearSpawnQueue();
extern void addSpawnQueue(point3d pos, quaternion rot, int type, void (*ai)(ship*, aiData*), aiData myData, char color, ability* myAbilities, int abilityCount);
extern void spawnShip(ship* queueEntry);

extern ship copyShip(ship* copyTarget, point3d pos, quaternion rot);
extern ship newShip(int hp, int maxHp, ability* myAbilities, int abilityCount, point3d myPosition, quaternion myRotation, double speed, double maxSpeed, double accel, double decel, double rollspeed, double pitchspeed, double yawspeed);

extern void idleAi(ship*, aiData*);
extern void humanAi(ship*, aiData*);
extern void fighterAi(ship*, aiData*);
extern void destroyerAi(ship*, aiData*);

typedef struct humanAiData{
	user* myuser;
}humanAiData;
typedef struct fighterAiData{
	int mode;
	point3d target;
}fighterAiData;
typedef struct destroyerAiData{
	int mode;
	point3d target;
	point3d destination;
}destroyerAiData;

typedef struct aiData{
	union{
		humanAiData human;
		fighterAiData fighter;
		destroyerAiData destroyer;
	};
}aiData;
typedef struct ship{
	int hp;
	int maxHp;
	ability* myAbilities;
	int abilityCount;
	point3d myPosition;
	vector positionOffset;//used to keep you flying in a straight line at slow speeds
	quaternion myRotation;
	double speed;
	double maxSpeed;
	double accel;
	double decel;
	double rollSpeed;
	double pitchSpeed;
	double yawSpeed;
	char color;
	char type;
	modelReference myModel;
	char name[8];

	void (*ai)(ship*, aiData*);
	aiData myAiData;
}ship;
extern ship shipTemplates[];
#endif
