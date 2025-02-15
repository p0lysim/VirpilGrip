
#include <VirpilGrip.h>

VirpilGrip _grip;
Direction hat_dir = Direction::IDLE;

void setup()
{
  Serial.begin(9600);
  _grip.Initialize();
}

void loop()
{

  if(hat_dir == Direction::UP){
    hat_dir = Direction::RIGHT;

  }else if(hat_dir == Direction::RIGHT){
    hat_dir = Direction::DOWN;

  }else if(hat_dir == Direction::DOWN){
    hat_dir = Direction::LEFT;

  }else{
    hat_dir = Direction::UP;

  }
  _grip.Set_Hat4direction(1, false, hat_dir);
  delay(1000);

}