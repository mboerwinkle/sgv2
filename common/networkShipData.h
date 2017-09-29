#ifndef NETWORKSHIPDATA_H
#define NETWORKSHIPDATA_H
typedef struct networkShipData{
	point3d myPosition;
	quaternion myRotation;//FIXME float
	char type;//FIXME should this structure preexist for every ship?
	char color;
	char name[8];
}networkShipData;//FIXME packed
typedef struct networkBullet{
	point3d origin;
	char offset[3];
	char type;
}networkBullet;//FIXME packed
#endif
