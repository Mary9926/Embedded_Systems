#define B1 AT91C_PIO_PC5 //left  click
#define B2 AT91C_PIO_PC4// right click

void configureButtons(void)
{
    AT91C_BASE_PIOC->PIO_PER = B1| B2;
    AT91C_BASE_PIOC->PIO_ODR = B1 | B2 ;
    AT91C_BASE_PIOC->PIO_PPUER = B1| B2 ;
}

int buttonState(int state)
{
  return ((AT91C_BASE_PIOC->PIO_PDSR & state) == 0);
}

//PIO_PPUS

void pullUpResistorEnable(unsigned int button)
{
  if(button == B1)
  {
      AT91C_BASE_PIOC->PIO_PPUER = B1;
  }
  else if(button == B2)
  {
      AT91C_BASE_PIOC->PIO_PPUER = B2;
  }
}


void pullUpResistorDisable(unsigned int button)
{
  if(button == B1)
  {
    AT91C_BASE_PIOC->PIO_PPUDR = B1;// disable the pull up resistor on the I/O line
  }
  else if(button == B2)
  {
      AT91C_BASE_PIOC->PIO_PPUDR = B2;
  }
}
