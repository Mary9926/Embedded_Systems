#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "include/AT91SAM9263.h"

//-------------------------DBGU_IDR-----------------------------------//
#define DISABLE_RXRDY_INTERRUPT AT91C_US_RXRDY
#define DISABLE_TXRDY_INTERRUPT AT91C_US_TXRDY
#define DISABLE_END_OF_RECIVE_TRANSFER_INPUT AT91C_US_ENDRX
#define DISBLE_END_OF_TRNASMIT_INTERRUPT AT91C_US_ENDTX
#define DISABLE_OVERRUN_INTERRUPT AT91C_US_OVRE
#define DISABLE_FRAMING_ERROR_INTERUPT AT91C_US_FRAME
#define DISABLE_PARITY_ERROR_INTERRUPT AT91C_US_PARE
#define DISABLE_TXEMPTY_INTERRUPT AT91C_US_TXEMPTY
#define DISABLE_TXBUFE_INTERRUPT AT91C_US_TXBUFE
#define DISABLE_RXBUFF_INTERUPT AT91C_US_RXBUFF
#define DISABLE_COMM_TX_INTERRUPT AT91C_US_COMM_TX //30
#define DISABLE_COMM_RX_INTERRUPT AT91C_US_COMM_RX //31
#define DISABLE_RECIVER AT91C_US_RXDIS
#define DISABLE_TRANSIMTER AT91C_US_TXDIS

#define CAPITAL_A_ASCII 65
#define LOWER_A_ASCII 97
#define CAPITAL_Z_ASCII 90
#define LOWER_Z_ASCII 122
#define MCK 100000000
#define CD 115200

void disableInterrupts(void)
{
  AT91C_BASE_DBGU->DBGU_IDR = (DISABLE_RXRDY_INTERRUPT|DISABLE_TXRDY_INTERRUPT|DISABLE_END_OF_RECIVE_TRANSFER_INPUT|DISBLE_END_OF_TRNASMIT_INTERRUPT|
                          	DISABLE_OVERRUN_INTERRUPT|DISABLE_FRAMING_ERROR_INTERUPT|DISABLE_PARITY_ERROR_INTERRUPT|DISABLE_TXEMPTY_INTERRUPT|
                          	DISABLE_TXBUFE_INTERRUPT|DISABLE_RXBUFF_INTERUPT|DISABLE_COMM_TX_INTERRUPT|DISABLE_COMM_RX_INTERRUPT); //desactivate debug interrupt
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
  AT91C_BASE_DBGU->DBGU_CR |= DISABLE_RECIVER; //reset and turn off/disable receiver
}

void disableTransmitter(void)
{
	AT91C_BASE_DBGU->DBGU_CR |= DISABLE_TRANSIMTER; // reset and disable treansmitter
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

bool isCapital(char letter)
{
  return (letter >= CAPITAL_A_ASCII && letter <= CAPITAL_Z_ASCII);
}

bool isLower(char letter)
{
  return (letter >= LOWER_A_ASCII  && letter <= LOWER_Z_ASCII );
}

char letterConversion(char letter)
{
  if(isCapital(letter))
  {
	return tolower(letter);
  }
  else if(isLower(letter))
  {
	return toupper(letter);
  }
  else
  {
	return letter;
  }
}

void dbguPrintAscii(const char buffer)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - checj TXRDY flag
	//CSR channel status regster //TXRDY Interrupt
	AT91C_BASE_DBGU->DBGU_THR = letterConversion(buffer); //write a singel char to Transmitter Holding Register
}

char dbguReadAscii(char buffer)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)){};//data avaible
	buffer = AT91C_BASE_DBGU->DBGU_RHR;
	//return buffer;
}

void dbguPrintAsciiForAlphabet(const char buffer)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}; //wait until Tx buffer busy - checj TXRDY flag
	//CSR channel status regster //TXRDY Interrupt
	AT91C_BASE_DBGU->DBGU_THR = buffer; //write a singel char to Transmitter Holding Register
}

void printChars()
{
  char upper;
  char lower;
  for(upper ='A'; upper<= 'Z';++upper)
  {
	dbguPrintAsciiForAlphabet(upper);
  }
  for (lower ='a'; lower<= 'z';++lower)
  {
	dbguPrintAsciiForAlphabet(lower);
  }
}
void printMessage(char*StringToPrint)
{
  int iterrator = 0;
  while(StringToPrint[iterrator] != '\0')
  {
	while(!(AT91C_BASE_DBGU->DBGU_CSR & DISABLE_TXRDY_INTERRUPT)){}; //wait until Tx buffer busy - checj TXRDY flag
	//CSR channel status regster //TXRDY Interrupt
	AT91C_BASE_DBGU->DBGU_THR =StringToPrint[iterrator]; //write a singel char to Transmitter Holding Register
	iterrator++;
  }
}

int main()
{
  dbguInit();
  char newLine = '\n';
  printChars();
  dbguPrintAscii(newLine);
  char input2;

  char* text = "My String to print";
  dbguPrintAscii(newLine);
  dbguPrintAscii(newLine);
  printMessage(text);
  dbguPrintAscii(newLine);
  dbguPrintAscii(newLine);
  char input = 't';
  //input = letterConversion(input);
  dbguPrintAscii(input);
  dbguPrintAscii(newLine);
  char character;
  while(true)
  {
	character = dbguReadAscii(character);
	dbguPrintAscii(character);
  }

  return 0;
}
