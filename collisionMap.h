#ifndef COLLISIONMAP_H
#define COLLISIONMAP_H
#include "common/geo/geo.h"
#include "ship.h"
#define GRANULARITY 1200//just a guess. should be tuned later
/*brief intro here.
Octree collision map supposed to provide O(n) ship collision lookup time (where n is the number of ships assuming identical density)

min is the bottom corner of the search space
max is the upper corner of the search space

I have evergrowing lists of nodes and shipIds. This is to prevent mallocs and frees.(because theyre heavy and the whole tree gets regenerated each tick).

so for "shipIdIdx" and "children[]", even though theyre ints, theyre really pointers. (the index in the list)

GRANULARITY is the size of the smallest cells.

the size of the grid is "gridSize". It is (a power of 2)*GRANULARITY.
It begins at min.
*/
extern int getShipsWithin(ship*** output, point3d position, int distance);

typedef struct shipId{
	ship* ref;
	int shipIdIdx;
}shipId;
typedef struct node{
	int children[8];
	point3d corner;
	int size;
	int shipIdIdx;
}node;

extern point3d min;
extern point3d max;
extern int gridSize;
extern int nodeCount;
extern int nodesUsed;
extern node* nodeList;
extern int shipIdCount;
extern int ShipIdsUsed;
extern shipId* shipIdList;

//does initGrid, then adds ships, then handles collision, then shows the collision grid
extern void createGrid();
extern void initGrid();
extern void addShip(ship* ref, int nodeIdx);
extern void showGrid(node* thisOne, int tabs);

//all the get functions are like custom mallocs
extern int getNode();
extern int getShipId();
#endif
