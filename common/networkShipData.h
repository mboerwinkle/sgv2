#ifndef NETWORKSHIPDATA_H
#define NETWORKSHIPDATA_H
typedef struct networkShipData{
	point3d myPosition;//12 bytes
	quaternion myRotation;//32 bytes
//	char myRotation[4];//4 bytes haha unit quaternions expressed in the size of an int? good idea!
	char type;//1 byte FIXME should this structure preexist for every ship?
	char color;//1 byte
	char name[8];
}networkShipData;//FIXME packed

//We save more than half of the size by expressing the quaternions as chars!

typedef struct networkBullet{
	point3d origin;//12 bytes
	vectorf dir;//12 bytes
//	char dir[3];//3 bytes
	char type;//1 byte
}networkBullet;//FIXME packed


//We save almost half of all bullets by storing the vector as a char!
#endif
