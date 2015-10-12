#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "main.h"

void *control_main();
void onReady();
int isReady();
void setTarget(char target);
char *substring(char *string, int position, int length);
int splitString(char *input, char *delim, char *results[], int limit);
void printCommand();
void printResponse();

#endif