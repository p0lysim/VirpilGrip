int clockPin = 13;  //ou SCK jaune
int latchPin = 10;  //ou CS vert
int dataPin = 12;   //ou MISO blanc

//Stock en double toutes les données pour pouvoir lire rapidement les valeurs pour 1 bit
byte bytes_readded[14];  //14 octets
bool bits_readded[112];  //14*8 bits

int btn_1_bit_index = 11;  //11
int btn_2_bit_index = 1;   //1
int btn_3_bit_index = 25;  //b25

int hat4_1[] = { 0, 15, 13, 14, 12 };   //TRIGGER b0+b8, UP b15 DOWN b13 RIGHT b14 LEFT b12
int hat4_2[] = { 10, 9, 23, 16, 22 };   //TRIGGER b10,   UP b9  DOWN b21 RIGHT b16 LEFT b22
int hat4_3[] = { 17, 24, 30, 31, 29 };  //TRIGGER b15    UP b24 DOWN b30 RIGHT b31 LEFT b29

int hat2_1[] = { 28, 27, 26 };  //TRIGGER b28, UP b27 DOWN b26

int wheel_index[] = { 18, 19 };  //b16 b17
int wheel_value = 0;
bool wheel_status = 0;

int joy_x_first_bit = 96;  //1 bit de l'octet utilisé la valeur en X
int joy_x_second_bit = 89;
int joy_x_right[] = { 101, 100 };  //101 && !100

int joy_y_first_bit = 88;
int joy_y_second_bit = 81;
int joy_y_right[] = { 103, 102 };  //!103 && 102

int joy_clic = 2;  //b2

int break_trigger = 32;
int break_bit_0 = 66;
int break_bit_1 = 67;
int break_bit_2 = 68;
int break_bit_3 = 69;
int break_bit_4 = 70;
int break_bit_5 = 71;
int break_bit_6 = 49;
int break_bit_7 = 50;
int break_bit_8 = 51;
int break_bit_9 = 52;
int break_bit_10 = 53;
int break_bit_11 = 54;


int fire_soft = 4;   //LIGHT b4 , HARD b3+b4
int fire_hard = 3;   //LIGHT b4 , HARD b3+b4
int fire_lever = 6;  //b6 (0up 1down)


void setup() {

  Serial.begin(115200);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
}

void loop() {

  //Latch pin to 1 =  collect parallel data
  digitalWrite(latchPin, 1);
  delayMicroseconds(10);

  //Latch pin to 0 = transmit data serially
  digitalWrite(latchPin, 0);

  //while the shift register is in serial mode collect each shift register into a byte
  for (int i = 0; i < 14; i++) {
    shiftIn(dataPin, clockPin, bytes_readded, bits_readded, i);
  }
  //shiftIn(dataPin, clockPin, bytes_readded, bits_readded, 0);

  if (wheel_status != bits_readded[wheel_index[0]] && wheel_status != bits_readded[wheel_index[1]]) {
    wheel_status = bits_readded[wheel_index[0]];
    wheel_value++;
    wheel_value = wheel_value % 360;
  }

  print_all_bits();
  //print_btn_status();
  //print_hat_status();
  //print_wheel();
  print_joy();
  //print_break();
  //print_fire();
  Serial.println("");
}

void print_all_bits() {

  for (int i = 0; i < 14; i++) {

    printBits(bytes_readded[i]);
    Serial.print(" ");
  }
}

void printBits(byte val) {

  for (int i = 7; i >= 0; i--) {
    bool b = bitRead(val, i);
    Serial.print(b);
  }
}

void print_btn_status() {
  Serial.print("[BTN] ");
  Serial.print("[");
  Serial.print(bits_readded[btn_1_bit_index]);
  Serial.print("]");
  Serial.print(" [");
  Serial.print(bits_readded[btn_2_bit_index]);
  Serial.print("]");
  Serial.print(" [");
  Serial.print(bits_readded[btn_3_bit_index]);
  Serial.print("]");
}

void print_hat_status() {

  Serial.print("[HAT] ");

  Serial.print("[");
  if (bits_readded[hat4_1[0]]) {
    Serial.print(".");
  } else {
    if (!bits_readded[hat4_1[1]]) { Serial.print("UP "); }
    if (!bits_readded[hat4_1[2]]) { Serial.print("DOWN "); }
    if (!bits_readded[hat4_1[3]]) { Serial.print("RIGHT "); }
    if (!bits_readded[hat4_1[4]]) { Serial.print("LEFT "); }
  }
  Serial.print("] ");

  Serial.print("[");
  if (bits_readded[hat4_2[0]]) {
    Serial.print(".");
  } else {
    if (!bits_readded[hat4_2[1]]) { Serial.print("UP "); }
    if (!bits_readded[hat4_2[2]]) { Serial.print("DOWN "); }
    if (!bits_readded[hat4_2[3]]) { Serial.print("RIGHT "); }
    if (!bits_readded[hat4_2[4]]) { Serial.print("LEFT "); }
  }
  Serial.print("] ");

  Serial.print("[");
  if (bits_readded[hat4_3[0]]) {
    Serial.print(".");
  } else {
    if (!bits_readded[hat4_3[1]]) { Serial.print("UP "); }
    if (!bits_readded[hat4_3[2]]) { Serial.print("DOWN "); }
    if (!bits_readded[hat4_3[3]]) { Serial.print("RIGHT "); }
    if (!bits_readded[hat4_3[4]]) { Serial.print("LEFT "); }
  }
  Serial.print("] ");

  Serial.print("[");
  if (bits_readded[hat2_1[0]]) {
    Serial.print(".");
  } else {
    if (!bits_readded[hat2_1[1]]) { Serial.print("UP "); }
    if (!bits_readded[hat2_1[2]]) { Serial.print("DOWN "); }
  }
  Serial.print("]");
}

void print_wheel() {
  Serial.print("[WHEEL] [");
  Serial.print(wheel_value);
  Serial.print("]");
}

void print_joy() {

  byte bX = 0;
  byte bY = 0;

  setBit(&bY, 7, bits_readded[joy_y_first_bit]);
  setBit(&bY, 6, bits_readded[joy_y_second_bit]);
  setBit(&bY, 5, bits_readded[joy_y_second_bit + 1]);
  setBit(&bY, 4, bits_readded[joy_y_second_bit + 2]);
  setBit(&bY, 3, bits_readded[joy_y_second_bit + 3]);
  setBit(&bY, 2, bits_readded[joy_y_second_bit + 4]);
  setBit(&bY, 1, bits_readded[joy_y_second_bit + 5]);
  setBit(&bY, 0, bits_readded[joy_y_second_bit + 6]);


  setBit(&bX, 7, bits_readded[joy_x_first_bit]);
  setBit(&bX, 6, bits_readded[joy_x_second_bit]);
  setBit(&bX, 5, bits_readded[joy_x_second_bit + 1]);
  setBit(&bX, 4, bits_readded[joy_x_second_bit + 2]);
  setBit(&bX, 3, bits_readded[joy_x_second_bit + 3]);
  setBit(&bX, 2, bits_readded[joy_x_second_bit + 4]);
  setBit(&bX, 1, bits_readded[joy_x_second_bit + 5]);
  setBit(&bX, 0, bits_readded[joy_x_second_bit + 6]);

  bool UP = !bits_readded[joy_y_right[0]] && bits_readded[joy_y_right[1]];
  bool RIGHT = bits_readded[joy_x_right[0]] && !bits_readded[joy_x_right[1]];

  bool CLICK = !bits_readded[joy_clic];

  Serial.print("[JOY] ");
  Serial.print("[ ");
  Serial.print(bX);
  Serial.print(" , ");
  Serial.print(bY);
  Serial.print(" ]");

  Serial.print(" [ ");

  if (UP) {
    Serial.print("UP");
  } else {
    Serial.print("DOWN");
  }

  if (RIGHT) {
    Serial.print(" RIGHT ");
  } else {
    Serial.print(" LEFT ");
  }

  Serial.print(CLICK);


  Serial.print(" ]");
}

void print_break() {

  bool break_trigged = !bits_readded[break_trigger];

  byte hight_byte = 0;
  byte low_byte = 0;

  setBit(&hight_byte, 3, bits_readded[break_bit_0]);
  setBit(&hight_byte, 2, bits_readded[break_bit_1]);
  setBit(&hight_byte, 1, bits_readded[break_bit_2]);
  setBit(&hight_byte, 0, bits_readded[break_bit_3]);

  setBit(&low_byte, 7, bits_readded[break_bit_4]);
  setBit(&low_byte, 6, bits_readded[break_bit_5]);
  setBit(&low_byte, 5, bits_readded[break_bit_6]);
  setBit(&low_byte, 4, bits_readded[break_bit_7]);

  setBit(&low_byte, 3, bits_readded[break_bit_8]);
  setBit(&low_byte, 2, bits_readded[break_bit_9]);
  setBit(&low_byte, 1, bits_readded[break_bit_10]);
  setBit(&low_byte, 0, bits_readded[break_bit_11]);

  word value = word(hight_byte, low_byte);
  Serial.print("[BREAK] ");

  Serial.print("[ ");
  Serial.print(break_trigged);
  Serial.print(" ]");

  Serial.print("[ ");
  //printBits(hight_byte);
  //Serial.print(" ");
  //printBits(low_byte);
  Serial.print(value);
  Serial.print(" ] ");
}

void print_fire() {

  bool lever_down = bits_readded[fire_lever];
  bool is_fire_soft = !bits_readded[fire_soft];
  bool is_fire_hard = is_fire_soft && !bits_readded[fire_hard];

  Serial.print("[FIRE] ");
  if (lever_down) {
    Serial.print("[DOWN] ");
  } else {
    Serial.print("[UP] ");
  }

  if (is_fire_hard) {
    Serial.print("[HARD] ");
  } else if (is_fire_soft) {
    Serial.print("[SOFT] ");
  } else {
    Serial.print("[NOT FIRE] ");
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------
* returns a byte with each bit in the byte corresponding to a pin on the shift register
------------------------------------------------------------------------------------------------------------------------------------*/
void shiftIn(int DataPin, int ClockPin, byte stored_bytes[], bool stored_bits[], int byte_index) {

  int tmp_bit = 0;
  byte data_in_readed = 0;

  pinMode(ClockPin, OUTPUT);
  pinMode(DataPin, INPUT);

  /*------------------------------------------------------------------------------------------------------------------------------------
  * we will be holding the clock pin high 8 times (0,..,7) at theend of each time through the for loop
  * at the beginning of each loop when we set the clock low, it will be doing the necessary low to high drop to cause the shift
  * register's DataPin to change state based on the value
  * of the next bit in its serial information flow.
  * The register transmits the information about the pins from pin 7 to pin 0 so that is why our function counts down
  ------------------------------------------------------------------------------------------------------------------------------------*/
  for (int i = 7; i >= 0; i--) {

    digitalWrite(ClockPin, 0);
    delayMicroseconds(2);
    tmp_bit = digitalRead(DataPin);

    stored_bits[byte_index * 8 + (7 - i)] = tmp_bit;  //Stock le bit lu dans un tableau stocké en variable de class pour etre lu ensuite rapidement

    //read 1 on the data pin
    if (tmp_bit) {
      data_in_readed = data_in_readed | (1 << i);  //set the bit to 0 no matter what
    }

    digitalWrite(ClockPin, 1);
  }

  stored_bytes[byte_index] = data_in_readed;
}

/*------------------------------------------------------------------------------------------------------------------------------------
* returns a byte with each bit in the byte corresponding to a pin on the shift register
------------------------------------------------------------------------------------------------------------------------------------*/
void shiftOu(int DataPin, int ClockPin) {

  int temp = 0;
  byte myDataIn = 0;

  pinMode(ClockPin, OUTPUT);
  pinMode(DataPin, INPUT);

  /*------------------------------------------------------------------------------------------------------------------------------------
  * we will be holding the clock pin high 8 times (0,..,7) at theend of each time through the for loop
  * at the beginning of each loop when we set the clock low, it will be doing the necessary low to high drop to cause the shift
  * register's DataPin to change state based on the value
  * of the next bit in its serial information flow.
  * The register transmits the information about the pins from pin 7 to pin 0 so that is why our function counts down
  ------------------------------------------------------------------------------------------------------------------------------------*/
  for (int i = 7; i >= 0; i--) {

    digitalWrite(ClockPin, 1);

    delayMicroseconds(2);
    digitalWrite(DataPin, 1);

    digitalWrite(ClockPin, 0);
  }
}

/*------------------------------------------------------------------------------------------------------------------------------------
* get bit in byte
------------------------------------------------------------------------------------------------------------------------------------*/
boolean getBit(byte value, byte index) {

  boolean bitState;

  bitState = value & (1 << index);

  return bitState;
}

/*------------------------------------------------------------------------------------------------------------------------------------
* set bit in byte
------------------------------------------------------------------------------------------------------------------------------------*/
void setBit(byte* myVarIn, byte whatBit, boolean s) {

  boolean bitState;

  if (s) {

    *myVarIn = *myVarIn | (1 << whatBit);

  }

  else {

    *myVarIn = *myVarIn & ~(1 << whatBit);
  }
}