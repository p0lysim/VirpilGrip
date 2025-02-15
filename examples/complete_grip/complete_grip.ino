
#include <VirpilGrip.h>


VirpilGrip _grip;

int pin_joy_x = A1;
int pin_joy_y = A0;
int pin_joy_clic = 3;
int pin_btn_right = 2;


void setup() {
  Serial.begin(9600);
  _grip.Initialize();

  pinMode(pin_joy_clic, INPUT_PULLUP);
  pinMode(pin_btn_right, INPUT_PULLUP);
  pinMode(pin_joy_x, INPUT);
  pinMode(pin_joy_y, INPUT);
}


void loop() {
  bool btn_right_clicked = digitalRead(pin_btn_right) == 0;
  bool btn_joy_clicked = digitalRead(pin_joy_clic) == 0;

  _grip.Set_Button(1, btn_right_clicked);

  int joy_x_value = analogRead(pin_joy_x);
  int joy_y_value = analogRead(pin_joy_y);

  Direction x_dir = Direction::LEFT;
  Direction y_dir = Direction::DOWN;

  float x_dir_percent = ((512.0 - joy_x_value) * 100.0) / 512.0;
  float y_dir_percent = ((512.0 - joy_y_value) * 100.0) / 512.0;

  if (joy_x_value > 512) {

    x_dir = Direction::RIGHT;
    x_dir_percent = ((joy_x_value - 512.0) * 100.0) / 512.0;
  }

  if (joy_y_value > 512) {
    y_dir = Direction::UP;
    y_dir_percent = ((joy_y_value - 512.0) * 100.0) / 512.0;
  }

  _grip.Set_JoystickValue(x_dir, y_dir, x_dir_percent, y_dir_percent, btn_joy_clicked);

}



