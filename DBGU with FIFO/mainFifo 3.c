#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "include/AT91SAM9263.h"

#define MCK 100000000
#define CD 115200
#define BUFFERSIZE 0xF
#define ASCII_ENTER 13

typedef struct FIFO
{
  char buffer[BUFFERSIZE+1];
  unsigned int head;
  unsigned int tail;
};

void fifoInit (struct FIFO *fifo)
{
  fifo->head = 0;
  fifo->tail = 0;
}

void fifoEmpty (struct FIFO *fifo)
{
  fifo->head = fifo->tail;
}

int fifoPut (struct FIFO *fifo, char data)
{
  if((fifo->tail - fifo->head) == 1 || (fifo->head - fifo->tail) == BUFFERSIZE)
  {// fifo  overflow
    return -1;
  }
  fifo->buffer[fifo->head] = data;
  fifo->head = (fifo->head + 1) & BUFFERSIZE; //put 1 byte successfully
  return 1;
}

int fifoGet (struct FIFO *fifo, char *data)
{
  if(fifo->head != fifo->tail)
  {
    *data = fifo->buffer[fifo->tail];
    fifo->tail = (fifo->tail +1) & BUFFERSIZE;
    return 1; // get 1 byte successfully
  }
  else
  {
    return -1; //no data in FIFO
  }
}


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
  AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RXDIS; //disable receiver
}

void disableTransmitter(void)
{
    AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_TXDIS; // disable treansmitter
}

void configureThroughput(void)
{
  AT91C_BASE_DBGU->DBGU_BRGR = MCK/(16*CD); // MCK mster CLock/ (CD //clock Divisor *16) //baud Rate Generator Register //speed of transmiition
}

void configureOperationMode(void)
{
    AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL| AT91C_US_PAR_NONE ; // set nomral mode 14 15 bit 0 0 // set no parity
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

void dbguPrintNewLine()
{
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - check TXRDY flag
    //CSR channel status regster //TXRDY Interrupt
    AT91C_BASE_DBGU->DBGU_THR = '\n'; //write a singel char to Transmitter Holding Register
}

void printMessage(const char*StringToPrint)
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

void dbguPrintBufferOverflowErrorMessage()
{
  dbguPrintNewLine();
  printMessage("There is no space left in the buffer");
}

void dbguPrintAsciiFifo(struct FIFO * fifo)
{
  dbguPrintNewLine();
  while(fifoGet(fifo,fifo->buffer)!=-1)
  {
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - checj TXRDY flag //CSR channel status regster //TXRDY Interrupt
    AT91C_BASE_DBGU->DBGU_THR = *fifo->buffer;
  }
  dbguPrintNewLine();
  fifoEmpty(fifo);
}

char dbguReadAsciiFifo(struct FIFO * fifo)
{
    while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)){}; //data avaible

    char currentCharacter = AT91C_BASE_DBGU->DBGU_RHR;
    if(currentCharacter == ASCII_ENTER)
    {
      dbguPrintAsciiFifo(fifo);
    }
    else if(fifoPut(fifo, AT91C_BASE_DBGU->DBGU_RHR)==-1)
    {
      dbguPrintBufferOverflowErrorMessage();
      dbguPrintAsciiFifo(fifo);
    }
}

int main()
{
  struct FIFO fifoStr;
  struct FIFO * fifo = &fifoStr;
  fifoInit(fifo);
  dbguInit();

  
  while(true)
  {
    dbguReadAsciiFifo(fifo);
  }

  return 0;
}