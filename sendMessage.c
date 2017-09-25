#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sendMessage.h"
#include "net/listen.h"//FIXME shouldnt have things needed here there. make a sepearate header file used by both
char packet[MSGSIZE];
const int usableLength = MSGSIZE-2;
char* msgIdx = packet;
char* packetIdx = packet+1;
void sendMessage(struct sockaddr_in *addr, char* data, int datalen){
	(*msgIdx)++;
	(*packetIdx) = 0;
	int packetSize;
	while(datalen > 0){
		if(datalen > usableLength){
			packetSize = usableLength;
		}else{
			packetSize = datalen;
		}
		memcpy(packet+2, data, packetSize);
		sendto(sockfd, packet, packetSize+2, 0, (struct sockaddr*)(addr), sizeof(*addr));
		datalen-=packetSize;
		data+=packetSize;
		(*packetIdx)++;
	}
}
