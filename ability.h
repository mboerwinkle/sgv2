#ifndef ABILITY_H
#define ABILITY_H
#include "ship.h"

typedef struct ship ship;

typedef struct ability{
	void (*act)(ship* target);
	int max;
	int cooldown;
}ability;

extern void applyAbility(ability* targ, int fire, ship* myShip);

extern void ability_Machinegun(ship* target);
extern void ability_AutoMachinegun(ship* target);
/*
extern void ability_Laser(ship* target);
extern void ability_Shotgun(ship* target);
extern void ability_Rocketlauncher(ship* target);
extern void ability_Teleport(ship* target);
extern void ability_Heatseeking(ship* target);
*/
#endif
