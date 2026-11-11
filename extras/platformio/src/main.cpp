#include <Arduino.h>
#include "VirpilGrip.h"

VirpilGrip grip;

bool btn_status;
int fire_intensity;
int direction;
int break_value;
int break_sign;

int joy_x = 0;
int joy_sign = 1;
int joy_y = 0;
Direction x_direction;
Direction y_direction;

void setup()
{

  Serial.begin(115200);
  grip.Initialize();
  Serial.println("Start");

  fire_intensity = 0;
  direction = 0;
  break_value = 0;
  break_sign = 1;
  x_direction = Direction::RIGHT;
  y_direction = Direction::UP;
}

void loop()
{

  //    btn_status = !btn_status;
  //    grip.Set_Button(1, btn_status);
  //    delay(500);
  //    grip.Set_Button(2, btn_status);
  //    delay(500);
  //    grip.Set_Button(3, btn_status);
  //    delay(500);

  //    if (fire_intensity == 0)
  //    {
  //      grip.Set_FireButton(false, Intensity::NONE);
  //    }
  //    else if (fire_intensity == 1)
  //    {
  //      grip.Set_FireButton(btn_status, Intensity::SOFT);
  //    }
  //    else if (fire_intensity == 2)
  //    {
  //      grip.Set_FireButton(btn_status, Intensity::HARD);
  //    }
  //
  //    fire_intensity = (fire_intensity + 1) % 3;
  //    delay(500);

  //  if (direction == 0)
  //  {
  //
  //    grip.Set_Hat4direction(0, false, Direction::UP);
  //    grip.Set_Hat4direction(1, false, Direction::UP);
  //    grip.Set_Hat4direction(2, false, Direction::UP);
  //    grip.Set_Hat4direction(3, false, Direction::UP);
  //
  //    grip.Set_Hat2direction(0, Direction::UP);
  //
  //  }
  //  else if (direction == 1)
  //  {
  //
  //    grip.Set_Hat4direction(0, false, Direction::DOWN);
  //    grip.Set_Hat4direction(1, false, Direction::DOWN);
  //    grip.Set_Hat4direction(2, false, Direction::DOWN);
  //    grip.Set_Hat4direction(3, false, Direction::DOWN);
  //
  //    grip.Set_Hat2direction(0, Direction::DOWN);
  //
  //  }
  //  else if (direction == 2)
  //  {
  //
  //    grip.Set_Hat4direction(0, false, Direction::RIGHT);
  //    grip.Set_Hat4direction(1, false, Direction::RIGHT);
  //    grip.Set_Hat4direction(2, false, Direction::RIGHT);
  //    grip.Set_Hat4direction(3, false, Direction::RIGHT);
  //
  //  }
  //  else if (direction == 3)
  //  {
  //
  //    grip.Set_Hat4direction(0, false, Direction::LEFT);
  //    grip.Set_Hat4direction(1, false, Direction::LEFT);
  //    grip.Set_Hat4direction(2, false, Direction::LEFT);
  //    grip.Set_Hat4direction(3, false, Direction::LEFT);
  //
  //  }
  //  else if (direction == 4)
  //  {
  //
  //   grip.Set_Hat4direction(0, false, Direction::IDLE);
  //   grip.Set_Hat4direction(1, false, Direction::IDLE);
  //   grip.Set_Hat4direction(2, false, Direction::IDLE);
  //   grip.Set_Hat4direction(3, false, Direction::IDLE);
  //
  //    grip.Set_Hat2direction(0, Direction::IDLE);
  //  }
  //
  //  direction = (direction + 1) % 5;
  //

  //  grip.Set_WheelIncrease();

  //  grip.Set_BreakValue(break_value);
  //
  //  break_value += break_sign;
  //  if(break_value >= 100){
  //    break_sign = -1;
  //  }
  //  if(break_value <=0){
  //    break_sign = 1;
  //  }

  //Serial.print(joy_x);
  //Serial.print(" -->  ");

  grip.Set_JoystickValue(Direction::IDLE, Direction::DOWN, 0, joy_x, false);

  joy_x += joy_sign;

  if (joy_x >= 100 || joy_x <= 0)
  {
    joy_sign *= -1;
  }


  // Serial.println(joy_x);
  grip.Print();
  delay(10);
}
