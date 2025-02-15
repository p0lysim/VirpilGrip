#ifndef VIRPILGRIP_H
#define VIRPILGRIP_H

#include <Arduino.h>

/*--------------------
* Pinout
* JAUNE : SCK
* VERT : SS
* BLANC : MISO
--------------------*/

enum class Direction
{
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class Intensity
{
    NONE,
    SOFT,
    HARD

};

class VirpilGrip
{

private:



    /*--------------------
    * Simple buttons
    --------------------*/
    int _IDX_btn_1 = 11; // b11
    int _IDX_btn_2 = 1;  // b1
    int _IDX_btn_3 = 25; // b25

    /*--------------------
    * hat 4 directions
    --------------------*/
    int _IDX_hat4_1[5] = {0, 15, 13, 14, 12};  // TRIGGER b0+b8, UP b15 DOWN b13 RIGHT b14 LEFT b12
    int _IDX_hat4_2[5] = {10, 9, 23, 16, 22};  // TRIGGER b10,   UP b9  DOWN b21 RIGHT b16 LEFT b22
    int _IDX_hat4_3[5] = {17, 24, 30, 31, 29}; // TRIGGER b15    UP b24 DOWN b30 RIGHT b31 LEFT b29

    /*--------------------
    * hat 2 directions
    --------------------*/
    int _IDX_hat2_1[3] = {28, 27, 26}; // TRIGGER b28, UP b27 DOWN b26

    /*--------------------
    * Wheel
    --------------------*/
    int _IDX_wheel_index[2] = {18, 19}; // b16 b17
    int _wheel_value;
    bool _wheel_status;

    /*--------------------
    * Joystick
    --------------------*/
    int _IDX_joy_x_first_bit = 96; // 1e bit de l'octet utilisé la valeur en X
    int _IDX_joy_x_second_bit = 89;
    int _IDX_joy_x_right[2] = {101, 100}; // 101 && !100

    int _IDX_joy_y_first_bit = 88;
    int _IDX_joy_y_second_bit = 81;
    int _IDX_joy_y_up[2] = {103, 102}; //! 103 && 102

    int _IDX_joy_clic = 2; // b2

    /*--------------------
    * Break
    --------------------*/
    int _IDX_break_trigger = 32;
    int _IDX_break_bit_0 = 66;
    int _IDX_break_bit_1 = 67;
    int _IDX_break_bit_2 = 68;
    int _IDX_break_bit_3 = 69;
    int _IDX_break_bit_4 = 70;
    int _IDX_break_bit_5 = 71;
    int _IDX_break_bit_6 = 49;
    int _IDX_break_bit_7 = 50;
    int _IDX_break_bit_8 = 51;
    int _IDX_break_bit_9 = 52;
    int _IDX_break_bit_10 = 53;
    int _IDX_break_bit_11 = 54;

    /*--------------------
    * Fire button
    --------------------*/
    int _IDX_fire_soft = 4;  // LIGHT b4 , HARD b3+b4
    int _IDX_fire_hard = 3;  // LIGHT b4 , HARD b3+b4
    int _IDX_fire_lever = 6; // b6 (0up 1down)

    /*--------------------
    * private function
    --------------------*/
    char *DirectionToString(Direction direction);
    char *IntensityToString(Intensity intensity);
    bool GetBit(int index, byte value);
    void IntToBytes(int value, uint8_t &lowByte, uint8_t &highByte);


    /*--------------------
    * Shift register
    --------------------*/
    static int _register_byte_size;// 14 bytes
    static volatile byte * _registers;
    static volatile int _byte_cursor;
    static volatile byte _tmp_byte;

    void Register_init();
    void Register_Clear();
    void Register_SetBit(int bit_index, bool value);
    bool Register_GetBit(int bit_index);
    byte Register_GetByte(int byte_index);
    void Register_MoveCursorNext();
    int Register_GetCursorIndex();

public:
    VirpilGrip();
    ~VirpilGrip();
    void Initialize();
    void Stop();
    void Clear();
    void Set_Button(int index, bool value);
    void Set_Hat4direction(int index, bool click_press, Direction direction);
    void Set_Hat2direction(int index, Direction direction);
    void Set_WheelIncrease();
    void Set_WheelDecrease();
    void Set_JoystickValue(Direction x_direction, Direction y_direction, float x_value, float y_value, bool click_press);
    void Set_BreakValue(float value);
    void Set_FireButton(bool lever_up, Intensity press_intensity);

    static void Register_ResetCursor();
    static byte Register_ShiftByte();

    void Print();
    void printBits(byte val);

};

#endif