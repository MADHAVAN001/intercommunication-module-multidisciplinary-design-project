#ifndef WIRELESS_H
#define WIRELESS_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "main.h"

void setupWireless();
void *wireless_reconnect();
void *wireless_read();
void *wireless_write();
void closeWireless();

#endif