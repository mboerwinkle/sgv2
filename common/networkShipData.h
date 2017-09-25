#ifndef NETWORKSHIPDATA_H
#define NETWORKSHIPDATA_H
typedef struct networkShipData{
	point3d myPosition;
	quaternion myRotation;
	char type;
	char color;
	char name[8];
}networkShipData;
#endif
