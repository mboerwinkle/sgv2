#ifndef GFX_H
#define GFX_H

#include "../common/geo/geo.h"
#include "../common/networkShipData.h"

extern void drawLine(point3d a, point3d b);
void drawShip(short type, point3d where, quaternion rot, char color, char* name);
typedef struct bullet{
        int type;
        point3d pos;
        vector dir;
}bullet;
extern void drawBullet(bullet* targ);
extern void gfxClear();

extern void gfxFlip();

extern void initGfx();

extern void quitGfx();

#endif
