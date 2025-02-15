
#include <VirpilGrip.h>


VirpilGrip _grip;
bool _btn_status;

void setup()
{

  Serial.begin(9600);
  _grip.Initialize();

}


void loop()
{

  _btn_status = !_btn_status;
  _grip.Set_Button(1, _btn_status);
  Serial.println(_btn_status);
  delay(1000);

}