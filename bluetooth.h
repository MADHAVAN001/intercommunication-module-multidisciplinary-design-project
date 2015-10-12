#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "main.h"

void setupBluetooth();
void *bluetooth_reconnect();
void *bluetooth_read();
void *bluetooth_write();
void closeBluetooth();

#endif