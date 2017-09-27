#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "ship.h"
#include "common/geo/geo.h"
extern void handleCollisions(ship* ref, ship** col, int colCount);
#endif
