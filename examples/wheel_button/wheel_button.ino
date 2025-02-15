#include <VirpilGrip.h>

VirpilGrip _grip;


void setup()
{
  Serial.begin(9600);
  _grip.Initialize();
}

void loop()
{

  _grip.Set_WheelIncrease(); //Work in progress
  delay(500);
}