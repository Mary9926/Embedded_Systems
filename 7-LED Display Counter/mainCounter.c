#include <stdint.h>
#include <stdbool.h>

#define PIOB_PER (volatile unsigned int * const) 0xFFFFF400 ///setting IO port in IO mode//PIO ENABLE REGISTER
#define PIOB_OER (volatile unsigned int * const) 0xFFFFF410 //setting the direction of IO port
#define PIOB_CODR (volatile unsigned int * const) 0xFFFFF434 //clean output data register
#define PIOB_SODR (volatile unsigned int * const) 0xFFFFF430 // forcing the required state on port output // Controller Set Output Data Register//write only
#define PIOB_ODSR (volatile unsigned int*) 0xFFFFF438 // output data status register

#define A (1<<25)  //processor pin PB25
#define B (1<<24) //processor pin  PB24
#define C (1<<22) //processor pin PB22
#define D (1<<21) //processor pin PB21
#define E (1<<20) //processor pin PB20
#define F (1<<27) //processor pin  PB27
#define G (1<<26) //processor pin PB26

#define LEFT_DISPLAY (1<<28) //turn on left led
#define RIGHT_DISPLAY (1<<30) // turn on right led
#define BRIGHTNESS (1<<29) // third transistor

#define DELAY 1000
#define POSITION_LEFT 0
#define POSITION_RIGHT 1


uint32_t ledDecode (uint8_t digit)
{
int number[10] = {};
number[0] = A|B|C|D|E|F;
number[1] = B|C;
number[2] = A|B|D|E|G;
number[3] = A|B|C|G|G|D;
number[4] = F|B|G|C;
number[5] = A|F|G|C|D;
number[6] = A|F|G|E|D|C;
number[7] = A|B|C;
number[8] = A|B|C|D|E|F|G;
number[9] = A|B|F|G|C|D;
return number[digit];
}

void delayMs (volatile unsigned int delay)
{
  volatile int time;
  for(time = 0; time < 80000*delay; time++);
}

void ledInit(void)
{
*PIOB_PER = (POSITION_LEFT|POSITION_RIGHT|A|B|C|D|E|F|G);
*PIOB_OER = (POSITION_LEFT|POSITION_RIGHT|A|B|C|D|E|F|G);
*PIOB_CODR =(A|B|C|D|E|F|G);
*PIOB_SODR = (BRIGHTNESS|LEFT_DISPLAY|RIGHT_DISPLAY);
}

void ledDisplayDigit(uint8_t digit,  uint8_t position)
{
*PIOB_CODR =(A|B|C|D|E|F|G);
*PIOB_SODR =ledDecode(digit);

if(position == POSITION_LEFT)
{
*PIOB_CODR = LEFT_DISPLAY;
*PIOB_SODR = RIGHT_DISPLAY;
}
else if(position == POSITION_RIGHT)
{
*PIOB_CODR = RIGHT_DISPLAY;
*PIOB_SODR = LEFT_DISPLAY;
}
}

int main(void)
{
int iterrator;
iterrator = 0;
  ledInit();
  while(true)
  {
ledDisplayDigit(iterrator , POSITION_LEFT);
iterrator++;
delayMs(DELAY);
if(iterrator == 10)
{
iterrator= 0;
}
  }

  return 0;
}
