#ifndef LISTEN_H
#define LISTEN_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MSGSIZE 3000//size of udp data per packet. Except plus two. See sendMessage.cpp
extern int sockfd;
extern struct sockaddr_in recvAddr;

extern void setupNetworkListen();
#endif
