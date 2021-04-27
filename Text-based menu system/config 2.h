#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string.h>

#define CLOCK_DEVISOR (MCK /( 16* BAUD_RATE))

#define MCK 100000000
#define PIV (MCK / (16 * 1000)) - 1
#define DELAY 100
#define DELAY_STEP DELAY / 10
#define BAUD_RATE 19200
#define PARITY AT91C_US_PAR_NONE

void pitInit (void);
void menuMessage(void);
void removeAndSpaceChar(void);
int ledLevelCommand(char *command);
int dbguLevelCommand(char *command);
int buttonLevelCommand(char *command);
int mainMenuLevelCommand(char *command);
void levelSwitch(int *level);
int mainLevel(void);
int ledLevel(void);
int buttonLevel(void);
int dbguLevel(void);
int checkCommandLevel(int levelId, char *command);
int levelFunction(int levelId, char* command);

void showLedStatus(unsigned int status);
void ledAFunctionality(char * command);
void ledBFunctionality(char * command);
void buttonsFunctionality(char * command);
void dbguFunctionality(char * command);

#endif
