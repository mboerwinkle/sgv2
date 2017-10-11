#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "ship.h"
#include "common/geo/geo.h"
extern void handleCollisions(ship* ref, ship** col, int colCount);
int intersect_triangle(vector l1, vector l2, vector vert0, vector vert1, vector vert2);
#endif
