#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "include/AT91SAM9263.h"

#include "config.h"
#include "led.h"
#include "dbgu.h"
#include "button.h"


#define CAPITAL_A_ASCII 65
#define LOWER_A_ASCII 97
#define CAPITAL_Z_ASCII 90
#define LOWER_Z_ASCII 122
#define CLOCK_DEVISOR (MCK /( 16* BAUD_RATE))

#define MCK 100000000
#define PIV (MCK / (16 * 1000)) - 1
#define DELAY 100
#define DELAY_STEP DELAY / 10
#define BAUD_RATE 19200


#define ENTER 13
#define BACKSPACE 8
#define SPACE 32

#define BUFFER 30

#define LEVEL_MAIN 0
#define LEVEL_LED 1
#define LEVEL_BUTTON 2
#define LEVEL_DBGU 3

#define B1 AT91C_PIO_PC5 //left  click
#define B2 AT91C_PIO_PC4// right click
#define LED1 1
#define LED2 2


void pitInit (void)
{
  volatile unsigned int read;
  AT91C_BASE_PITC->PITC_PIMR = PIV; //init PIV //disable timer //disable interupts
  read = AT91C_BASE_PITC->PITC_PIVR ; //to read and clear
}

void menuMessage(void)
{
  printMessage("Simple menu by Maria Gryglewska \n Write 'help' to obtain more information \n to return back use command 'up'");
}

void removeAndSpaceChar(void)
{
  dbguPrintAscii(BACKSPACE);
  dbguPrintAscii(SPACE);
  dbguPrintAscii(BACKSPACE);
}

int ledLevelCommand(char *command)
{

  if(strcasecmp(command, "help") == 0)
  {
    printMessage("SetLED\nClearLED\nBlinkLED\nLEDstatus\nChangeLED\n");
    printMessage("Use command for LED A or B\nExample: SetLed A");
  }
  else if(strcasecmp(command, "up") == 0)
  {
    return LEVEL_MAIN;
  }
  else
  {
    ledAFunctionality(command);
    ledBFunctionality(command);
  }
  return LEVEL_LED;
}

int buttonLevelCommand(char *command)
{
  if(strcasecmp(command, "help") == 0)
  {
    printMessage("ReadButton\nPullupEn\nPullupDis");
  }
  else if(strcasecmp(command, "up") == 0)
  {
    return LEVEL_MAIN;
  }
  else
  {
      buttonsFunctionality(command);
  }
  return LEVEL_BUTTON;
}

int dbguLevelCommand(char *command)
{
  if(strcasecmp(command, "help") == 0)
  {
    printMessage("DeviceStatus");
  }
  else if(strcasecmp(command, "up") == 0)
  {
    return LEVEL_MAIN;
  }
  else
  {
    dbguFunctionality(command);
  }
  return LEVEL_DBGU;
}

int mainMenuLevelCommand(char *command)
{
  if(strcasecmp(command, "help") == 0)
  {
    printMessage("LED\nButton\nDBGU");
  }
  else if(strcasecmp(command, "led") == 0)
  {
    return LEVEL_LED;
  }
  else if(strcasecmp(command, "button") == 0)
  {
    return LEVEL_BUTTON;
  }
  else if(strcasecmp(command, "dbgu") == 0)
  {
    return LEVEL_DBGU;
  }
  return LEVEL_MAIN;
}

void levelSwitch(int *level)
{
  if(*level == LEVEL_LED)
  {
    *level = ledLevel();
  }
  else if(*level == LEVEL_BUTTON)
  {
    *level = buttonLevel();
  }
  else if(*level == LEVEL_DBGU)
  {
    *level = dbguLevel();
  }
  else
  {
    *level = mainLevel();
  }
}

int mainLevel(void)
{
  printMessage("\n>");
  return levelFunction(LEVEL_MAIN, "\n>");
}

int ledLevel(void)
{
  printMessage("\nLED>");
  return levelFunction(LEVEL_LED, "\nLED>");
}

int buttonLevel(void)
{
  printMessage("\nbutton>");
  return levelFunction(LEVEL_BUTTON, "\nbutton>");
}

int dbguLevel(void)
{
  printMessage("\nDBGU>");
  return levelFunction(LEVEL_DBGU, "\nDBGU>");
}

int checkCommandLevel(int levelId, char *command)
{
  if(levelId == LEVEL_MAIN)
  {
      return mainMenuLevelCommand(command);
  }
  else if(levelId == LEVEL_LED)
  {
    return ledLevelCommand(command);
  }

  else if(levelId == LEVEL_BUTTON)
  {
    return buttonLevelCommand(command);
  }
  else if(levelId == LEVEL_DBGU)
  {
    return dbguLevelCommand(command);
  }
  else
  {
      return levelId;
  }
}

int levelFunction(int levelId, char* command)
{
  char character;
  char newLine = '\n';
  char inputCommand[BUFFER];
  int iterator = 0;
  int level = levelId;
  memset(inputCommand, 0, BUFFER);//pointer input command and replace all 0 /clears string

  while(true)
  {
    character = dbguReadAscii(character); // read a single char

    if(character == ENTER)
    {
        dbguPrintAscii(newLine);
        level = checkCommandLevel(levelId, inputCommand);//check level and pass the command
        if(level != levelId) // if levels are not equal
        {
          break;
        }
        else
        {
          printMessage(command);// >
        }
        memset(inputCommand, 0, BUFFER); //;//pointer input command and replace all 0 /clears string
        iterator = 0; //tell us where currently we are in string// go to the begin of string
      }
      else if(character == BACKSPACE)
      {
        if(iterator > 0)
        {
          removeAndSpaceChar();
          iterator--;
          inputCommand[iterator] = 0; //reset only one char //set last char to nothing
        }
      }
     else if(iterator < BUFFER) //add character to string
     {
        dbguPrintAscii(character);//print char
        inputCommand[iterator++] = character; //add char to string
      }
    else
    {
      printMessage("\nbuffer full");
      break;
    }
  }
    return level;
}

void showLedStatus(unsigned int status)
{
  if(status == 0)
  {
    printMessage("Led active");
  }
  else
  {
    printMessage("Led no-active");
  }
}

void ledAFunctionality(char * command)
{

  if(strcasecmp(command, "SETLED A") == 0)
  {
    ledATurnOn();
  }
  else if(strcasecmp(command, "CLEARLED A") == 0)
  {
      ledATurnOff();
  }
  else if(strcasecmp(command, "BLINKLED A") == 0)
  {
      blinkALed();
  }
  else if(strcasecmp(command, "LEDSTATUS A") == 0)
  {
        showLedStatus(ledStatus(LED1));
  }
  else if(strcasecmp(command, "CHANGELED A") == 0)
  {
        changeALed(LED1);
  }
}


void ledBFunctionality(char * command)
{

  if(strcasecmp(command, "SETLED B") == 0)
  {
    ledBTurnOn();
  }
  else if(strcasecmp(command, "CLEARLED B") == 0)
  {
      ledBTurnOff();
  }

  else if(strcasecmp(command, "BLINKLED B") == 0)
  {
      blinkBLed();
  }
 else if(strcasecmp(command, "LEDSTATUS B") == 0)
  {
      showLedStatus(ledStatus(LED2));
  }
  else if(strcasecmp(command, "CHANGELED B") == 0)
  {
        changeBLed(LED2);
  }
}


void buttonsFunctionality (char * command)
{
  if(strcasecmp(command, "READBUTTON A") == 0)
  {
    if(buttonState(B1))
    {
      printMessage("\n\tButton A is active");
    }
    else
    {
      printMessage("\n\tButton A is inactive");
    }
    if((AT91C_BASE_PIOC->PIO_PPUSR & B1) == 0) //button disabled
    {
      printMessage("\n\t Pull up disabled for button a ");

    }
    else
    {
        printMessage("\n\t Pull up enabled for button a ");
    }
  }
  else if(strcasecmp(command, "READBUTTON B") == 0)
  {
    if(buttonState(B2))
    {
      printMessage("\n\tButton B is active");
    }
    else
    {
      printMessage("\n\tButton B is inactive");
    }
    if((AT91C_BASE_PIOC->PIO_PPUSR & B2) == 0) //button disabled
    {
      printMessage("\n\t Pull up disabled for button b ");

    }
    else
    {
        printMessage("\n\t Pull up enabled for button b ");
    }
  }

  else if(strcasecmp(command, "PULLUPEN A") == 0)
  {
      pullUpResistorEnable(B1);
      printMessage("\n\tPull up resistor for Button A enabled");
  }
  else if(strcasecmp(command, "PULLUPDIS A") == 0)
  {
        pullUpResistorDisable(B1);
        printMessage("\n\tPull up resistor for Button A disabled");
  }
  else if(strcasecmp(command, "PULLUPEN B") == 0)
  {
      pullUpResistorEnable(B2);
      printMessage("\n\tPull up resistor for Button B enabled");
  }
  else if(strcasecmp(command, "PULLUPDIS B") == 0)
  {
        pullUpResistorDisable(B2);
        printMessage("\n\tPull up resistor for Button B disabled");
  }

}

void dbguFunctionality(char * command)
{

  if(strcasecmp(command, "DEVICESTATUS") == 0)
  {
    deviceStatus();
  }
}


int main(void)
{
  pitInit();
  dbguInit();
  configureLEDs();
  configureButtons();
  menuMessage();

  int level = 0;
  while(true)
  {
    levelSwitch(&level);
  }

  return 0;
}
