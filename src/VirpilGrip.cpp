#include "VirpilGrip.h"
#include <SPI.h>

volatile byte *VirpilGrip::_registers = nullptr;
volatile int VirpilGrip::_byte_cursor = 0;
volatile byte VirpilGrip::_tmp_byte = 0;
int VirpilGrip::_register_byte_size = 14;

VirpilGrip::VirpilGrip()
{
}

VirpilGrip::~VirpilGrip()
{
    // Destructor
}

void VirpilGrip::Initialize()
{
    Register_init();

    Clear(); // Clear all the shiftregistrer values

    pinMode(MISO, OUTPUT); // Sets MISO as OUTPUT (Have to Send data to Master IN
    pinMode(SCK, INPUT);
    pinMode(SS, INPUT_PULLUP);

    SPCR |= _BV(SPE); // Turn on SPI in Slave Mode

#if defined(__AVR_ATmega328P__)

    PCICR |= (1 << PCIE0);   // Enable PCINT for PORTB
    PCMSK0 |= (1 << PCINT2); // Enable PCINT for SS pin (PB2/Pin 10)

#elif defined(__AVR_ATmega32U4__)

    PCICR |= (1 << PCIE0);   // Enable PCINT for PORTB
    PCMSK0 |= (1 << PCINT0); // Enable PCINT for SS pin (PB0 /Pin 8)

#else

    // same as arduino nano / uno
    PCICR |= (1 << PCIE0);   // Enable PCINT for PORTB
    PCMSK0 |= (1 << PCINT2); // Enable PCINT for SS pin (PB2/Pin 10)

#endif

    SPI.setBitOrder(LSBFIRST); // a utiliser avec le vrais shift register
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV8);

    SPI.attachInterrupt(); // Interuupt ON is set for SPI commnucation
}

void VirpilGrip::Stop()
{
    SPI.end();
}

ISR(PCINT0_vect)
{

#if defined(__AVR_ATmega328P__)

    if (PINB & (1 << PB2))
    {
        // SS is HIGH (inactive) Transaction complete --> reset register cursror
        VirpilGrip::Register_ResetCursor();
    }

#elif defined(__AVR_ATmega32U4__)

    if (PINB & (1 << PB0))
    {
        // SS is HIGH (inactive) Transaction complete --> reset register cursror
        VirpilGrip::Register_ResetCursor();
    }

#else
    if (PINB & (1 << PB2))
    {
        // SS is HIGH (inactive) Transaction complete --> reset register cursror
        VirpilGrip::Register_ResetCursor();
    }

#endif
}

ISR(SPI_STC_vect) // Inerrrput routine function
{
    SPDR = VirpilGrip::Register_ShiftByte();
}

void VirpilGrip::Clear()
{

    VirpilGrip::Register_Clear();

    Set_Button(1, false);
    Set_Button(2, false);
    Set_Button(3, false);

    Set_Hat4direction(1, false, Direction::IDLE);
    Set_Hat4direction(2, false, Direction::IDLE);
    Set_Hat4direction(3, false, Direction::IDLE);

    Set_Hat2direction(1, Direction::IDLE);

    _wheel_value = 1;
    _wheel_status = false;

    Set_JoystickValue(Direction::IDLE, Direction::IDLE, 0, 0, false);
    Set_BreakValue(0);

    Set_FireButton(true, Intensity::NONE);
}

void VirpilGrip::Set_Button(int index, bool value)
{

    if (index == 1)
    {
        VirpilGrip::Register_SetBit(_IDX_btn_1, !value);
    }
    else if (index == 2)
    {
        VirpilGrip::Register_SetBit(_IDX_btn_2, !value);
    }
    else if (index == 3)
    {
        VirpilGrip::Register_SetBit(_IDX_btn_3, !value);
    }
}

void VirpilGrip::Set_Hat4direction(int index, bool click_press, Direction direction)
{

    if (index == 1)
    {
        VirpilGrip::Register_SetBit(_IDX_hat4_1[0], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_1[1], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_1[2], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_1[3], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_1[4], 1);

        // Aucun mouvement
        if (!click_press && direction == Direction::IDLE)
        {
            return;
        }
        else if (direction != Direction::IDLE || click_press)
        {

            VirpilGrip::Register_SetBit(_IDX_hat4_1[0], 0);

            if (direction == Direction::UP)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_1[1], 0);
            }
            if (direction == Direction::DOWN)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_1[2], 0);
            }
            if (direction == Direction::RIGHT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_1[3], 0);
            }
            if (direction == Direction::LEFT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_1[4], 0);
            }
        }
    }
    else if (index == 2)
    {

        VirpilGrip::Register_SetBit(_IDX_hat4_2[0], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_2[1], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_2[2], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_2[3], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_2[4], 1);

        // Aucun mouvement
        if (!click_press && direction == Direction::IDLE)
        {
            return;
        }
        else if (direction != Direction::IDLE || click_press)
        {

            VirpilGrip::Register_SetBit(_IDX_hat4_2[0], 0);

            if (direction == Direction::UP)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_2[1], 0);
            }
            if (direction == Direction::DOWN)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_2[2], 0);
            }
            if (direction == Direction::RIGHT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_2[3], 0);
            }
            if (direction == Direction::LEFT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_2[4], 0);
            }
        }
    }
    else if (index == 3)
    {

        VirpilGrip::Register_SetBit(_IDX_hat4_3[0], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_3[1], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_3[2], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_3[3], 1);
        VirpilGrip::Register_SetBit(_IDX_hat4_3[4], 1);

        // Aucun mouvement
        if (!click_press && direction == Direction::IDLE)
        {
            return;
        }
        else if (direction != Direction::IDLE || click_press)
        {

            VirpilGrip::Register_SetBit(_IDX_hat4_3[0], 0);

            if (direction == Direction::UP)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_3[1], 0);
            }
            if (direction == Direction::DOWN)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_3[2], 0);
            }
            if (direction == Direction::RIGHT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_3[3], 0);
            }
            if (direction == Direction::LEFT)
            {
                VirpilGrip::Register_SetBit(_IDX_hat4_3[4], 0);
            }
        }
    }
}

void VirpilGrip::Set_Hat2direction(int index, Direction direction)
{

    VirpilGrip::Register_SetBit(_IDX_hat2_1[0], 1);
    VirpilGrip::Register_SetBit(_IDX_hat2_1[1], 1);
    VirpilGrip::Register_SetBit(_IDX_hat2_1[2], 1);

    // Aucun mouvement
    if (direction == Direction::IDLE)
    {
        return;
    }
    else if (direction != Direction::IDLE)
    {

        VirpilGrip::Register_SetBit(_IDX_hat2_1[0], 0);

        if (direction == Direction::UP)
        {
            VirpilGrip::Register_SetBit(_IDX_hat2_1[1], 0);
        }
        if (direction == Direction::DOWN)
        {
            VirpilGrip::Register_SetBit(_IDX_hat2_1[2], 0);
        }
    }
}

void VirpilGrip::Set_WheelIncrease()
{
    _wheel_value++;
    _wheel_status = !_wheel_status;
    _wheel_value %= 360;

    VirpilGrip::Register_SetBit(_IDX_wheel_index[0], _wheel_status);
    VirpilGrip::Register_SetBit(_IDX_wheel_index[1], _wheel_status);
}

void VirpilGrip::Set_WheelDecrease()
{
    _wheel_value--;
    _wheel_status = !_wheel_status;
    _wheel_value = (_wheel_value + 360) % 360;

    VirpilGrip::Register_SetBit(_IDX_wheel_index[0], _wheel_status);
    VirpilGrip::Register_SetBit(_IDX_wheel_index[1], _wheel_status);
}

void VirpilGrip::Set_JoystickValue(Direction x_direction, Direction y_direction, float x_percent, float y_percent, bool click_press)
{

    //[0 - 255]
    int x_value = (x_percent * 255.0) / 100.0;
    int y_value = (y_percent * 255.0) / 100.0;

    if (x_direction == Direction::RIGHT)
    {
        x_value = 255 - x_value;
    }

    if (y_direction == Direction::DOWN)
    {
        y_value = 255 - y_value;
    }

    // Solution ok en lecture
    // VirpilGrip::Register_SetBit(_IDX_joy_x_first_bit, GetBit(7, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit, GetBit(6, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 1, GetBit(5, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 2, GetBit(4, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 3, GetBit(3, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 4, GetBit(2, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 5, GetBit(1, x_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 6, GetBit(0, x_value));

    // VirpilGrip::Register_SetBit(_IDX_joy_y_first_bit, GetBit(7, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit, GetBit(6, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 1, GetBit(5, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 2, GetBit(4, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 3, GetBit(3, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 4, GetBit(2, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 5, GetBit(1, y_value));
    // VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 6, GetBit(0, y_value));

    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit - 1, GetBit(7, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit, GetBit(6, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 1, GetBit(5, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 2, GetBit(4, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 3, GetBit(3, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 4, GetBit(2, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 5, GetBit(1, x_value));
    VirpilGrip::Register_SetBit(_IDX_joy_x_second_bit + 6, GetBit(0, x_value));

    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit - 1, GetBit(7, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit, GetBit(6, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 1, GetBit(5, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 2, GetBit(4, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 3, GetBit(3, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 4, GetBit(2, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 5, GetBit(1, y_value));
    VirpilGrip::Register_SetBit(_IDX_joy_y_second_bit + 6, GetBit(0, y_value));

    if (y_direction == Direction::UP)
    {
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[0], 0);
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[1], 1);
    }
    else if (y_direction == Direction::DOWN)
    {
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[0], 1);
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[1], 0);
    }
    else
    {
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[0], 1);
        VirpilGrip::Register_SetBit(_IDX_joy_y_up[1], 0);
    }

    if (x_direction == Direction::LEFT)
    {
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[0], 0);
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[1], 1);
    }
    else if (x_direction == Direction::RIGHT)
    {
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[0], 1);
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[1], 0);
    }
    else
    {
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[0], 1);
        VirpilGrip::Register_SetBit(_IDX_joy_x_right[1], 0);
    }

    if (click_press)
    {
        VirpilGrip::Register_SetBit(_IDX_joy_clic, 0);
    }
    else
    {
        VirpilGrip::Register_SetBit(_IDX_joy_clic, 1);
    }
}

void VirpilGrip::Set_BreakValue(float percent)
{

    //[0 - 4095]
    int value = (percent * 4095.0) / 100.0;

    byte lowByte;
    byte highByte;
    IntToBytes(value, lowByte, highByte);

    VirpilGrip::Register_SetBit(_IDX_break_bit_0, GetBit(3, highByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_1, GetBit(2, highByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_2, GetBit(1, highByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_3, GetBit(0, highByte));

    VirpilGrip::Register_SetBit(_IDX_break_bit_4, GetBit(7, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_5, GetBit(6, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_6, GetBit(5, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_7, GetBit(4, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_8, GetBit(3, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_9, GetBit(2, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_10, GetBit(1, lowByte));
    VirpilGrip::Register_SetBit(_IDX_break_bit_11, GetBit(0, lowByte));

    if (value > 1024)
    {
        VirpilGrip::Register_SetBit(_IDX_break_trigger, 0);
    }
    else
    {
        VirpilGrip::Register_SetBit(_IDX_break_trigger, 1);
    }
}

void VirpilGrip::Set_FireButton(bool lever_up, Intensity press_intensity)
{
    VirpilGrip::Register_SetBit(_IDX_fire_lever, !lever_up);

    VirpilGrip::Register_SetBit(_IDX_fire_hard, 1);
    VirpilGrip::Register_SetBit(_IDX_fire_soft, 1);

    if (press_intensity == Intensity::NONE)
    {
        return;
    }
    else if (press_intensity == Intensity::SOFT)
    {
        VirpilGrip::Register_SetBit(_IDX_fire_soft, 0);
    }
    else if (press_intensity == Intensity::HARD)
    {
        VirpilGrip::Register_SetBit(_IDX_fire_soft, 0);
        VirpilGrip::Register_SetBit(_IDX_fire_hard, 0);
    }
}

char *VirpilGrip::DirectionToString(Direction direction)
{
    switch (direction)
    {
    case Direction::UP:
        return "UP";
    case Direction::DOWN:
        return "DOWN";
    case Direction::LEFT:
        return "LEFT";
    case Direction::RIGHT:
        return "RIGHT";
    case Direction::IDLE:
        return "IDLE";
    default:
        return "UNKNOWN";
    }
}

char *VirpilGrip::IntensityToString(Intensity intensity)
{
    switch (intensity)
    {
    case Intensity::NONE:
        return "NONE";
    case Intensity::SOFT:
        return "SOFT";
    case Intensity::HARD:
        return "HARD";
    default:
        return "UNKNOWN";
    }
}

void VirpilGrip::Print()
{
    // Print the register bits
    for (int i = 0; i < _register_byte_size; i++)
    {

        for (int j = 0; j < 8; j++)
        {

            Serial.print(Register_GetBit(i * 8 + j));
        }

        Serial.print(" ");
    }
    Serial.println();

    //    Serial.print("Button 1: ");
    //    Serial.println(_btn_1_press);
    //    Serial.print("Button 2: ");
    //    Serial.println(_btn_2_press);
    //    Serial.print("Button 3: ");
    //    Serial.println(_btn_3_press);
    //    Serial.print("Hat4_1 Press: ");
    //    Serial.println(_hat4_1_press);
    //    Serial.print("Hat4_1 Direction: ");
    //    Serial.println(DirectionToString(_hat4_1_dir));
    //    Serial.print("Hat4_2 Press: ");
    //    Serial.println(_hat4_2_press);
    //    Serial.print("Hat4_2 Direction: ");
    //    Serial.println(DirectionToString(_hat4_2_dir));
    //    Serial.print("Hat4_3 Press: ");
    //    Serial.println(_hat4_3_press);
    //    Serial.print("Hat4_3 Direction: ");
    //    Serial.println(DirectionToString(_hat4_3_dir));
    //    Serial.print("Hat2 Direction: ");
    //    Serial.println(DirectionToString(_hat2_dir));
    //    Serial.print("Wheel Value: ");
    //    Serial.println(_wheel_value);
    //    Serial.print("Wheel Status: ");
    //    Serial.println(_wheel_status);
    //    Serial.print("Joystick X Value: ");
    //    Serial.println(_joystick_x_value);
    //    Serial.print("Joystick Y Value: ");
    //    Serial.println(_joystick_y_value);
    //    Serial.print("Joystick Click: ");
    //    Serial.println(_joystick_click);
    //    Serial.print("Joystick X Direction: ");
    //    Serial.println(DirectionToString(_joystick_x_dir));
    //    Serial.print("Joystick Y Direction: ");
    //    Serial.println(DirectionToString(_joystick_y_dir));
    //    Serial.print("Break Value: ");
    //    Serial.println(_break_value);
    //    Serial.print("Fire Lever Down: ");
    //    Serial.println(_fire_lever_down);
    //    Serial.print("Fire Intensity: ");
    //    Serial.println(IntensityToString(_firse_intensity));
}

void VirpilGrip::printBits(byte val)
{

    for (int i = 7; i >= 0; i--)
    {
        bool b = bitRead(val, i);
        Serial.print(b);
    }
}

bool VirpilGrip::GetBit(int index, byte value)
{

    return (value >> index) & 1;
}

void VirpilGrip::IntToBytes(int value, uint8_t &lowByte, uint8_t &highByte)
{
    lowByte = value & 0xFF;         // Low byte
    highByte = (value >> 8) & 0xFF; // High byte
}

void VirpilGrip::Register_init()
{
    _registers = new byte[_register_byte_size];
    Clear();
}

void VirpilGrip::Register_Clear()
{
    _registers[0] = 0xFF;
    _registers[1] = 0xFF;
    _registers[2] = 0xFF;
    _registers[3] = 0xFF;
    _registers[4] = 0b01101111;
    _registers[5] = 0b10000001;
    _registers[6] = 0b01101110;
    _registers[7] = 0b11001000;
    _registers[8] = 0b11100000;
    _registers[9] = 0x00;
    _registers[10] = 0b01111000;
    _registers[11] = 0b01010000;
    _registers[12] = 0b01010000;
    _registers[13] = 0x00;

    _byte_cursor = 0;
}

void VirpilGrip::Register_SetBit(int bit_index, bool value)
{

    int tmp_byte_index = bit_index / 8;
    int tmp_bit_position = bit_index - (tmp_byte_index * 8);

    if (tmp_byte_index >= _register_byte_size)
    {
        return;
    }

    if (tmp_bit_position >= 8)
    {
        return;
    }

    if (value)
    {
        // Set the bit at bit_position to 1
        _registers[tmp_byte_index] |= (1 << tmp_bit_position);
    }
    else
    {
        // Set the bit at bit_position to 0
        _registers[tmp_byte_index] &= ~(1 << tmp_bit_position);
    }
}

bool VirpilGrip::Register_GetBit(int bit_index)
{

    int tmp_byte_index = bit_index / 8;
    int tmp_bit_position = bit_index - (tmp_byte_index * 8);

    if (tmp_byte_index >= _register_byte_size)
    {
        return;
    }

    if (tmp_bit_position >= 8)
    {
        return;
    }

    return (_registers[tmp_byte_index] >> tmp_bit_position) & 1;
}

byte VirpilGrip::Register_GetByte(int byte_index)
{
    byte_index++;
    if (byte_index >= _register_byte_size)
    {
        byte_index = 0;
    }

    return _registers[byte_index];
}

byte VirpilGrip::Register_ShiftByte()
{

    if (_byte_cursor + 1 >= _register_byte_size)
    {
        _tmp_byte = _registers[0];
    }
    else
    {
        _tmp_byte = _registers[_byte_cursor + 1];
    }

    _byte_cursor++;
    if (_byte_cursor >= _register_byte_size)
    {
        _byte_cursor = _byte_cursor;
    }

    return _tmp_byte;
}

void VirpilGrip::Register_MoveCursorNext()
{

    _byte_cursor++;
    if (_byte_cursor >= _register_byte_size)
    {
        _byte_cursor = 0;
    }
}

void VirpilGrip::Register_ResetCursor()
{

    _byte_cursor = 0;
}

int VirpilGrip::Register_GetCursorIndex()
{

    return _byte_cursor;
}
