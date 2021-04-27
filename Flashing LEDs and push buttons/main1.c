#include <stdint.h>
#include <stdbool.h>
 
#define PIOB_CODR (volatile unsigned int *)0xFFFFF434//clean output data register//turn on
#define PIOB_PER (volatile unsigned int *)0xFFFFF400 //setting IO port in IO mode //Controller PIO Enable Register//write only
#define PIOB_OER (volatile unsigned int *)0xFFFFF410 //setting the direction of IO port //  Controller Output Enable Register// write only
#define PIOB_SODR (volatile unsigned int *)0xFFFFF430 //forcing the required state on port output // Controller Set Output Data Register//write only
 
#define PIOC_SODR (volatile unsigned int * const) 0xFFFFF630 //forcing the required state on port output
#define PIOC_PDSR (volatile unsigned int * const) 0xFFFFF63C //read input state/./PIO Controller Pin Data Status Register//read only
#define PIOC_PER (volatile unsigned int * const) 0xFFFFF600 //setting IO port in IO mode
#define PIOC_OER (volatile unsigned int * const) 0xFFFFF610 //setting the direction of IO port
#define PIOC_CODR (volatile unsigned int * const) 0xFFFFF634 //Controller Clear Output Data Register
#define PIOC_ODR (volatile unsigned int * const) 0xFFFFF614 //PIO Controller Output Disable Register
#define PIOC_PUER (volatile unsigned int * const) 0xFFFFF664 //PIO Pull Up Enable Register
 
#define PMC_PCER (volatile unsigned int * const) 0xFFFFFC10 //send clock state to peripherals devices
 
#define DELAY 1000
#define DELAY_STEP  DELAY / 10
 
#define LED1 1 << 8
#define LED2 1 << 29
 
#define B1 1 << 5 //left  click
#define B2 1 << 4 // right click
#define PID4 1 << 4
 
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
      delayMs(DELAY);
      *PIOB_CODR = LED1;     // Turn on
 
      delayMs(DELAY);
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
