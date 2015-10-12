#ifndef ARDUINO_H
#define ARDUINO_H
#include "main.h"

void setupArduino();
void *arduino_reconnect();
void *arduino_read();
void *arduino_write();
void closeArduino();

#endif
