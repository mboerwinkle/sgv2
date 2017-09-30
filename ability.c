#include <stdio.h>
#include <stdlib.h>
#include "ability.h"

void applyAbility(ability* targ, int fire, ship* myShip){
	if(targ->cooldown > 0){
		targ->cooldown--;
		return;
	}
	if(fire){
		targ->act(myShip);
		targ->cooldown = targ->max;
	}
}

void ability_Machinegun(ship* target){
	
}
