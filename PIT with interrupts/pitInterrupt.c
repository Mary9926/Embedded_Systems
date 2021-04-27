#include <stdint.h>
#include <stdbool.h>
#include "include/AT91SAM9263.h"

#define MCK 100000000
#define PIV (MCK / (16 * 1000)) - 1
#define DELAY 1000
#define DELAY_STEP DELAY / 10

#define B1 AT91C_PIO_PC5 //left  click
#define B2 AT91C_PIO_PC4// right click
#define PID4 1 << 4

bool delayFlag = false;
unsigned int TimerCounter = 0;
unsigned int elseInterrupt = 0;

void pitInit (void)
{
  volatile unsigned int read;
  AT91C_BASE_PITC->PITC_PIMR = PIV; //init PIV //disable timer //disable interupts
  read = AT91C_BASE_PITC->PITC_PIVR ; //to read and clear
}

void timerInterruptHandler()
{
  volatile unsigned int read;
  if (AT91C_BASE_PITC->PITC_PIMR & AT91C_PITC_PITIEN) // if flag interrupt is set/interrupt enabled
  {
	if (AT91C_BASE_PITC->PITC_PISR ) // 1 ms // if flag pits is set / time requested/check if pit intterrupt was triggered
	{
  	read = AT91C_BASE_PITC->PITC_PIVR ;  //clerars PITS in PIT_SR//and count once again
  	TimerCounter++;
  	delayFlag = true;
	}
	else
	{
  	elseInterrupt++;
	}
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;
}

void pitInitInterrupts()
{
	AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_SYS); //disable PIT timer interrupts
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (unsigned int) timerInterruptHandler; //configuration of the pointer for timer intterupt handler -handler for processor periherial device
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE | AT91C_AIC_PRIOR_HIGHEST; //configure method of interrupt triggering
	AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS); //clear an interrupt flag
	AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS); //turn on an interrupt
	AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITIEN;  //turn on PIT timer interrupt

}


void pitDelayMs (unsigned int delay)
{
  volatile unsigned int read;
  int time = 0;
  read = AT91C_BASE_PITC->PITC_PIVR;//clear PITS
  AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN; //turn on timer
  while(time < delay)
  {
	time++;
	while (!delayFlag){}
	delayFlag = false;
  }
  AT91C_BASE_PITC->PITC_PIMR &= ~ (AT91C_PITC_PITEN); //turn off timer
}
void configureButtons(void)
{
	AT91C_BASE_PIOC->PIO_PER = B1| B2;
	AT91C_BASE_PIOC->PIO_ODR = B1 | B2 ;
	AT91C_BASE_PIOC->PIO_PPUER = B1| B2 ;
}

void configureClock(void)
{
	AT91C_BASE_PMC->PMC_PCER = PID4;
}

void configureLEDs(void)
{
  AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB8; //LED1
  AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB8; //LED1
  AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8; //LED1

  AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC29; //LED2
  AT91C_BASE_PIOC->PIO_OER = AT91C_PIO_PC29;  //LED2
  AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PC29; //LED2
}


int buttonState(int state)
{
  return ((AT91C_BASE_PIOC->PIO_PDSR & state) == 0);
}

int main(void)
{
  pitInit ();
  pitInitInterrupts();
  configureLEDs();
  configureButtons();
  configureClock();
  AT91C_BASE_PIOB->PIO_CODR= AT91C_PIO_PB8;   // Turn on
  AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8; // Turn off

  while(1)
  {
	int iterrator;
	for(iterrator = 0; iterrator < DELAY_STEP; iterrator++)
	{
  	pitDelayMs(DELAY);
  	AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB8; 	// Turn on //LED1

  	pitDelayMs(DELAY);
  	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8; // Turn off //LED1

  	if(buttonState(B1))
  	{
    	AT91C_BASE_PIOC->PIO_CODR = AT91C_PIO_PC29; //LED2
  	}

  	if(buttonState(B2))
  	{
    	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29; //LED2
  	}
	}
  }
  return 0;
}
