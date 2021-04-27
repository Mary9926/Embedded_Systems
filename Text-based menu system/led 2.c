#define DELAY 100

void pitDelayMs (unsigned int delay)
{
  volatile unsigned int read;
  int time = 0;
  while(time < delay)
  {
    time++;
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN ; //enable timer start timer
    volatile unsigned int flag = AT91C_BASE_PITC->PITC_PISR;
    while (AT91C_BASE_PITC->PITC_PISR == 0) {}
    flag = AT91C_BASE_PITC->PITC_PISR;
    read = AT91C_BASE_PITC->PITC_PIVR ; //clerars PITS in PIT_SR
  }

}

void configureLEDs(void)
{
  AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB8; //LED1
  AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB8; //LED1
  AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8; //LED1

  AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC29; //LED2
  AT91C_BASE_PIOC->PIO_OER = AT91C_PIO_PC29;  //LED2
  AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29; //LED2
}


void ledATurnOn(void)
{
  AT91C_BASE_PIOB->PIO_CODR= AT91C_PIO_PB8; //LED1
}

void ledBTurnOn(void)
{
    AT91C_BASE_PIOC->PIO_CODR = AT91C_PIO_PC29; //LED2
}

void ledATurnOff(void)
{
  AT91C_BASE_PIOB->PIO_SODR= AT91C_PIO_PB8; //LED1
}

void ledBTurnOff(void)
{
    AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29; //LED2
}


void blinkALed()
{
  for(unsigned int i = 0; i < 5; i++)
  {
    pitDelayMs (DELAY);
    ledATurnOn();
    pitDelayMs (DELAY);
    ledATurnOff();
  }
}
void blinkBLed()
{
  for(unsigned int i = 0; i < 5; i++)
  {
    pitDelayMs (DELAY);
    ledBTurnOn();
    pitDelayMs (DELAY);
    ledBTurnOff();
  }
}

unsigned int ledStatus(unsigned int led)
{
  if(led == 1)
  {
    return (AT91C_BASE_PIOB->PIO_ODSR &AT91C_PIO_PB8);
  }
  else
  {
      return ( AT91C_BASE_PIOC->PIO_ODSR &AT91C_PIO_PC29);

  }
}

void changeALed(unsigned int led)
{
    if(ledStatus(led) == 0)
    {
      ledATurnOff();
    }
    else
    {
      ledATurnOn();
    }
}

void changeBLed(unsigned int led)
{
    if(ledStatus(led) == 0)
    {
      ledBTurnOff();
    }
    else
    {
      ledBTurnOn();
    }
}
