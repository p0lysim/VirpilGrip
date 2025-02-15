#include <VirpilGrip.h>

VirpilGrip _grip;

float joy_x_percent; //[0 - 100.0]
float joy_y_percent; //[0 - 100.0]

float inc = 1.0;

Direction dir_x = Direction::RIGHT;
Direction dir_y = Direction::UP;

void setup()
{
  Serial.begin(9600);
  _grip.Initialize();
}

void loop()
{
  joy_x_percent += inc;
  joy_y_percent += inc;

  if(joy_x_percent >= 100 ||joy_y_percent >= 100 ){
    inc = -1;
  }

  if(joy_x_percent <= 0 ||joy_y_percent <= 0 ){
    joy_x_percent = 0;
    joy_y_percent = 0;
    inc = 1;

    if(dir_x ==  Direction::RIGHT){
      dir_x = Direction::LEFT;
    }else{
      dir_x = Direction::RIGHT;
    }

    if(dir_y ==  Direction::UP){
      dir_y = Direction::DOWN;
    }else{
      dir_y = Direction::UP;
    }

  }

  _grip.Set_JoystickValue(dir_x, dir_y, joy_x_percent, joy_y_percent, false);
  delay(10);

}
