#include <stdint.h>
#include <stdbool.h>
#include "include/AT91SAM9263.h"

#define PIOB_CODR AT91C_PIOB_CODR //clean output data register//turn on
#define PIOB_PER AT91C_PIOB_PER //setting IO port in IO mode //Controller PIO Enable Register//write only
#define PIOB_OER AT91C_PIOB_OER //setting the direction of IO port //  Controller Output Enable Register// write only
#define PIOB_SODR AT91C_PIOB_SODR //forcing the required state on port output // Controller Set Output Data Register//write only

#define PIOC_SODR AT91C_PIOC_SODR //forcing the required state on port output
#define PIOC_PDSR AT91C_PIOC_PDSR //read input state/./PIO Controller Pin Data Status Register//read only
#define PIOC_PER AT91C_PIOC_PER //setting IO port in IO mode
#define PIOC_OER AT91C_PIOC_OER //setting the direction of IO port
#define PIOC_CODR AT91C_PIOC_CODR //Controller Clear Output Data Register
#define PIOC_ODR AT91C_PIOC_ODR //PIO Controller Output Disable Register
#define PIOC_PUER AT91C_PIOC_PPUER//PIO Pull Up Enable Register

#define PITC_PIVR AT91C_PITC_PIVR //Periodic Interval Timer Value Register /read-only
//Reading this register clears PITS in PIT_SR.
#define PITC_MR AT91C_PITC_PIMR //read-write / Periodic Interval Timer Mode Register
#define PITC_SR AT91C_PITC_PISR //real-time status register -
// check if counter was reset  (bit PITS)
//periodic Interval Timer Status Register//flag
#define PITC_PIIR AT91C_PITC_PIVR//Periodic Interval Timer Image Register
//Returns the current value of the periodic interval time

#define PMC_PCER  AT91C_PMC_PCER //send clock state to peripherals devices

#define DELAY 1000
#define DELAY_STEP DELAY / 10

#define LED1 AT91C_PIO_PB8
#define LED2 AT91C_PIO_PC29

#define B1 AT91C_PIO_PC5 //left  click
#define B2 AT91C_PIO_PC4// right click
#define PID4 1 << 4

#define DISABLE_TIMER ~AT91C_PITC_PITEN //PIT_MR
#define ENABLE_TIMER AT91C_PITC_PITEN //PIT_MR
#define DISABLE_INTERUPTS  ~AT91C_PITC_PITIEN//PIT_MR

#define PIV 6249

void pitInit (void)
{
  volatile unsigned int read;
  *PITC_MR &= DISABLE_TIMER; //disable timer
  *PITC_MR &= DISABLE_INTERUPTS; //disable interupts
  *PITC_MR = PIV; //init PIV
  read = *PITC_PIVR ; //to read and clear

}


void pitDelayMs (unsigned int delay)
{
  volatile unsigned int read;
  int time = 0;
  while(time < delay)
  {
    time++;

    *PITC_MR |= ENABLE_TIMER ; //enable timer start timer
    volatile unsigned int flag = *PITC_SR;
    while (*PITC_SR == 0) {}
    flag = *PITC_SR;
    read = *PITC_PIVR; //clerars PITS in PIT_SR
  }

}
void configureButtons(void)
{
    *PIOC_PER = B1| B2;
    *PIOC_ODR = B1 | B2 ;
    *PIOC_PUER = B1| B2 ;
}

void configureClock(void)
{
    *PMC_PCER = PID4;
}

void configureLEDs(void)
{
    *PIOB_PER = LED1;
    *PIOB_OER = LED1;
    *PIOC_PER = LED2;
    *PIOC_OER = LED2;
}


void delayMs (volatile unsigned int delay)
{
  volatile int time;
  for(time = 0; time < 80000*delay; time++);
}


int buttonState(int state)
{
  return ((*PIOC_PDSR & state) == 0);
}


int main(void)
{
    pitInit ();
    configureLEDs();
    configureButtons();
    configureClock();
   *PIOB_CODR = LED1;   // Turn on
   *PIOB_SODR = LED1; // Turn off

  while(1)
  {
    int iterrator;
    for(iterrator = 0; iterrator < DELAY_STEP; iterrator++)
    {
      pitDelayMs(DELAY);
      *PIOB_CODR = LED1;     // Turn on

      pitDelayMs(DELAY);
      *PIOB_SODR = LED1; // Turn off

      if(buttonState(B1))
        {
            *PIOC_CODR = LED2;
        }

      if(buttonState(B2))
        {
              *PIOC_SODR = LED2;
        }

  }

}

  return 0;

}
