#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "ship.h"
#include "common/geo/geo.h"
extern void handleCollisions(ship* ref, ship** col, int colCount);
int intersect_triangle(vectorf l1, vectorf l2, vectorf vert0, vectorf vert1, vectorf vert2);
#endif
