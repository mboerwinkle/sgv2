#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H
extern void sendMessage(struct sockaddr_in *addr, char* data, int datalen);
#endif
