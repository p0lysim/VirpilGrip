#include<SPI.h>

volatile int counter = 0;
volatile byte array_X[14] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01};

void setup()
{
  Serial.begin(115200);
  pinMode(MISO,OUTPUT);//Sets MISO as OUTPUT (Have to Send data to Master IN 
  SPCR |= _BV(SPE);//Turn on SPI in Slave Mode
  pinMode(SS, INPUT_PULLUP);
  SPI.attachInterrupt();//Interuupt ON is set for SPI commnucation
}

ISR (SPI_STC_vect)                        //Inerrrput routine function 
{


  SPDR = array_X[counter];
  counter ++;
    if(counter == 13){counter = 0;}


}


void loop()
{ 
}