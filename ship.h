#ifndef SHIP_H
#define SHIP_H
#include "common/geo/geo.h"
#include "ability.h"
#include "user.h"
#include "common/stlLoad/loadStl.h"
#include "common/networkShipData.h"
typedef struct ship ship;
typedef struct aiData aiData;

extern ship** shipPtrList;//a shitty little data structure. should be replaced by nodes or sectors or something at some point
extern ship* shipList;
extern int shipCount;

extern void tickShips();
extern int getShipsWithin(ship*** output, point3d position, int distance);//FIXME
extern void spawnHumanShip(int userIdx, point3d pos, quaternion rot, int type);
extern void spawnComputerShip(point3d pos, quaternion rot, int type);
extern ship copyShip(ship* copyTarget, point3d pos, quaternion rot);
extern ship newShip(int hp, int maxHp, ability** myAbilities, int abilityCount, point3d myPosition, quaternion myRotation, double speed, double maxSpeed, double accel, double decel, double rollspeed, double pitchspeed, double yawspeed);

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
	ability** myAbilities;
	int abilityCount;
	point3d myPosition;
	quaternion myRotation;
	double speed;
	double maxSpeed;
	double accel;
	double decel;
	double rollSpeed;
	double pitchSpeed;
	double yawSpeed;
	int radius;
	char color;
	char type;
	model* myModel;
	char name[8];

	void (*ai)(ship*, aiData*);
	aiData myAiData;
}ship;
extern ship shipTemplates[];
#endif
