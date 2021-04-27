#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string.h>
#include "button.c"

void configureButtons(void);
void pullUpResistorEnable(unsigned int button);
void pullUpResistorDisable(unsigned int button);
int buttonState(int state);

#endif
