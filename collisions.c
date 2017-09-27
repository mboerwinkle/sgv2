#include <stdio.h>
#include <stdlib.h>
#include "collisions.h"
void handleCollisions(ship* ref, ship** col, int colCount){
	point3d pos;
	p3dEqual(pos, ref->myPosition);
	double rad = ref->myModel->radius;
	for(int idx = 0; idx < colCount; idx++){
		point3d pos2;
		p3dEqual(pos2, col[idx]->myPosition);
		double rad2 = col[idx]->myModel->radius;
		if(rad+rad2 > p3dDistance(pos, pos2)){
		//	ref->hp = 0;
		//	col[idx]->hp = 0;
		}
	}
}
