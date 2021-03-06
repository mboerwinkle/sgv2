#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "gfx.h"
#include "../common/networkShipData.h"

char *data = NULL;
int maxDataLen = 0;
int datalen = 0;
char lastMsgIdx;
char lastPacketIdx;
int bad = 1;
void messageParse();
void netParse(char* msg, int len){
	char thisMsgIdx = msg[0];
	char thisPacketIdx = msg[1];
//	printf("netparse %d %d\n", thisMsgIdx, thisPacketIdx);
	if(thisMsgIdx != lastMsgIdx){//first packet
		lastMsgIdx = thisMsgIdx;
		if(thisPacketIdx != 0){//we missed the first packet :(
			bad = 1;
			return;
		}
		if(!bad){//send the completed data to be processed
//			puts("sending message for processing");
			messageParse();
		}
		datalen = 0;
		bad = 0;
	}else{//continuation
		if(lastPacketIdx+1 != thisPacketIdx || bad == 1){//check for discontinuity
			bad = 1;
			return;
		}
	}
	lastPacketIdx = thisPacketIdx;
	if(datalen+len-2 > maxDataLen){
		maxDataLen = datalen+len-2;
		printf("Our biggest message ever! Reallocating! %d\n", maxDataLen);
		data = (char*)realloc(data, maxDataLen);
	}
	memcpy(data+datalen, msg+2, len-2);
	datalen+=len-2;
}
void messageParse(){
//	fwrite(data, 1, datalen, stdout);
//	printf("\ndatalen %d\n", datalen);
	char prefix[4] = {data[0], data[1], data[2], 0};
	if(!strcmp(prefix, "TXT")){
		printf("%s\n", data+3);
	}else if(!strcmp(prefix, "SCN")){
		int dataUsed = 3;
		P3DEQUAL(myPos, (int*)(data+dataUsed)); dataUsed+=sizeof(point3d);
		QUATEQUAL(targRot, (double*)(data+dataUsed)); dataUsed+=sizeof(quaternion);
		int shipsUsed = *((int*)(data+dataUsed)); dataUsed+=sizeof(int);
		gfxClear();
		quaternion shipRotation;
		for(int shipCount = 0; shipCount < shipsUsed; shipCount++){
			networkShipData thisShip = *(networkShipData*)(data+dataUsed);
			dataUsed+=sizeof(networkShipData);
			for(int dim = 0; dim < 4; dim++){
				shipRotation[dim] = (double)(thisShip.myRotation[dim])/127;
			}
			drawShip(thisShip.type, thisShip.myPosition, shipRotation, thisShip.color);
		}
		int gfxCount = *((int*)(data+dataUsed)); dataUsed+=sizeof(int);
		for(int temp = 0; temp < gfxCount; temp++){
			drawBullet((networkBullet*)(data+dataUsed));
			dataUsed+=sizeof(networkBullet);
		}
		gfxFlip();
	}
}
