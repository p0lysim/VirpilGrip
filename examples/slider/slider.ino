#include <VirpilGrip.h>

VirpilGrip _grip;

float slider_value; //[0, 100.0]
float inc = 1.0;

void setup()
{
  Serial.begin(9600);
  _grip.Initialize();
}

void loop()
{

  slider_value += inc;

  if(slider_value >= 100 || slider_value <= 0){
    inc *= -1;
  }

  _grip.Set_BreakValue(slider_value);
  delay(10);
}