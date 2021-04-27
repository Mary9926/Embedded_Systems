#include "dbgu.h"
#include <stdio.h>
#include <stdlib.h>
#define MCK 100000000
#define CLOCK_DEVISOR (MCK /( 16* BAUD_RATE))//325
#define CAPITAL_A_ASCII 65
#define LOWER_A_ASCII 97
#define CAPITAL_Z_ASCII 90
#define LOWER_Z_ASCII 122
#define PARITY AT91C_US_PAR_NONE

void disableInterrupts(void)
{
  AT91C_BASE_DBGU->DBGU_IDR = (AT91C_US_RXRDY|AT91C_US_TXRDY|AT91C_US_ENDRX|AT91C_US_ENDTX|
                            AT91C_US_OVRE|AT91C_US_FRAME|AT91C_US_PARE|AT91C_US_TXEMPTY|
                            AT91C_US_TXBUFE|AT91C_US_RXBUFF|AT91C_US_COMM_TX|AT91C_US_COMM_RX); //desactivate debug interrupt
}

void configurePeripheralPorts(void)
{
  AT91C_BASE_PIOC->PIO_ASR |= AT91C_PIO_PC30; // debug receiver data RxD//Pio Periherial A select Register//reciver
  AT91C_BASE_PIOC->PIO_PDR |= AT91C_PIO_PC30; //PIO Disable Register

  AT91C_BASE_PIOC->PIO_ASR |= AT91C_PIO_PC31; //debug receiver data TXD //transmitter
  AT91C_BASE_PIOC->PIO_PDR |= AT91C_PIO_PC31;
}

void turnOffReciver(void)
{
  AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RSTRX; //reset and turn off/disable receiver
}

void turnOffTransmitter(void)
{
    AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RSTTX; // reset and turn off/disable treansmitter
}

void disabeReciver(void)
{
  AT91C_BASE_DBGU->DBGU_CR |=  AT91C_US_RXDIS; //reset and turn off/disable receiver
}

void disableTransmitter(void)
{
    AT91C_BASE_DBGU->DBGU_CR |=  AT91C_US_TXDIS; // reset and disable treansmitter
}

void configureThroughput(void)
{
  AT91C_BASE_DBGU->DBGU_BRGR = CLOCK_DEVISOR; // MCK mster CLock/ (CD //clock Divisor *16) //baud Rate Generator Register //speed of transmiition
}

void configureOperationMode(void)
{
    AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL| PARITY ; // set nomral mode 14 15 bit 0 0 // set no parity
}

void turnOnReciver(void)
{
    AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RXEN;
}

void turnOnTransmitter(void)
{
  AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_TXEN;
}

void dbguInit(void)
{
  disableInterrupts();
  turnOffReciver();
  turnOffTransmitter();
  disabeReciver();
  disableTransmitter();
  configurePeripheralPorts();
  configureThroughput();
  configureOperationMode();
  turnOnReciver();
  turnOnTransmitter();
}

void dbguPrintAscii(const char buffer)
{
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - checj TXRDY flag
    //CSR channel status regster //TXRDY Interrupt
    AT91C_BASE_DBGU->DBGU_THR = buffer;//letterConversion(buffer); //write a singel char to Transmitter Holding Register
}

char dbguReadAscii(char buffer)
{
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)){};//data avaible
    buffer = AT91C_BASE_DBGU->DBGU_RHR;
    //return buffer;
}

void printMessage(char*StringToPrint)
{
  int iterrator = 0;
  while(StringToPrint[iterrator] != '\0')
  {
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - checj TXRDY flag
    //CSR channel status regster //TXRDY Interrupt
    AT91C_BASE_DBGU->DBGU_THR =StringToPrint[iterrator]; //write a singel char to Transmitter Holding Register
    iterrator++;
  }
}

void parityInfo(void)
{
  if(AT91C_BASE_DBGU->DBGU_MR & AT91C_US_PAR_EVEN)
  {
    printMessage("Even parity");
  }

  if(AT91C_BASE_DBGU->DBGU_MR & AT91C_US_PAR_ODD)
  {
    printMessage("Odd parity");
  }

  if(AT91C_BASE_DBGU->DBGU_MR & AT91C_US_PAR_SPACE)
  {
    printMessage("Space parity forced 0");
  }

  if(AT91C_BASE_DBGU->DBGU_MR & AT91C_US_PAR_MARK)
  {
    printMessage("Mark parity forced 1");
  }

  if(AT91C_BASE_DBGU->DBGU_MR & AT91C_US_PAR_NONE)
  {
    printMessage("No parity");
  }
}

void deviceStatus (void)
{
  int cd = AT91C_BASE_DBGU->DBGU_BRGR; //baud rate generator register
  int baudrate = (MCK / (cd*16));
  char message[32];
  parityInfo();
  printMessage("\nDatabits: 8");
  printMessage("\nBaudrate: ");

  itoa(baudrate,message,10);
  printMessage(message);
  dbguPrintAscii('\n');
}
