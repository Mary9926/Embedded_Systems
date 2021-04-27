#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include <string.h>
#include "led.c"

void configureLEDs(void);
void pitDelayMs (unsigned int delay);
void ledATurnOn(void);
void ledATurnOff(void);
void ledBTurnOn(void);
void ledBTurnOff(void);
void blinkALed();
void blinkBLed();
unsigned int ledStatus(unsigned int led);
void changeALed(unsigned int led);
void changeBLed(unsigned int led);
#endif
