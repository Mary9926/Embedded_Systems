#ifndef DBGU_H_INCLUDED
#define DBGU_H_INCLUDED

#include <string.h>
#include "dbgu.c"

void disableInterrupts(void);
void turnOffReciver(void);
void turnOffTransmitter(void);
void disabeReciver(void);
void disableTransmitter(void);
void configurePeripheralPorts(void);
void configureThroughput(void);
void configureOperationMode(void);
void turnOnReciver(void);
void turnOnTransmitter(void);
void dbguInit(void);
void parityInfo(void);
void deviceStatus(void);


void dbguPrintAscii(const char buffer);
char dbguReadAscii(char buffer);
void printMessage(char*StringToPrint);
#endif
