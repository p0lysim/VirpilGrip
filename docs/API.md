# API reference

```cpp
#include <VirpilGrip.h>

VirpilGrip grip;          // one instance per board (the frame buffer is shared with the interrupts)
```

## Types

```cpp
enum class Direction { IDLE, UP, DOWN, LEFT, RIGHT };
enum class Intensity { NONE, SOFT, HARD };
```

## Life cycle

| Method | Description |
|---|---|
| `void Initialize()` | Builds the idle frame, configures the hardware SPI as a slave and enables the SPI and SS pin-change interrupts. Call it once in `setup()`. |
| `void Clear()` | Puts every control back to rest: buttons released, hats centred, thumbstick `IDLE`, brake 0 %, trigger released, safety lever up. |
| `void Stop()` | Disables the SPI peripheral (`SPI.end()`). |

## Controls

| Method | Parameters | Notes |
|---|---|---|
| `void Set_Button(int index, bool pressed)` | `index` 1 – 3 | Other indices are ignored. |
| `void Set_Hat4direction(int index, bool click, Direction dir)` | `index` 1 – 3, `dir` = `IDLE`, `UP`, `DOWN`, `LEFT`, `RIGHT` | `IDLE` + `click = false` → released. `click = true` + `IDLE` → centre push. |
| `void Set_Hat2direction(int index, Direction dir)` | `dir` = `IDLE`, `UP`, `DOWN` | There is a single 2-way hat: `index` is ignored. |
| `void Set_FireButton(bool leverUp, Intensity intensity)` | `NONE`, `SOFT` (1st stage), `HARD` (2nd stage) | `leverUp` is the trigger safety lever. |
| `void Set_BreakValue(float percent)` | 0 – 100 | Analog brake lever (12 bits). Above ~25 % the end-of-travel switch is reported as pressed too. |
| `void Set_JoystickValue(Direction xDir, Direction yDir, float xPercent, float yPercent, bool click)` | `xDir` = `LEFT`/`RIGHT`, `yDir` = `UP`/`DOWN`, percentages 0 – 100 from the centre | Analog thumbstick + push. Values are not clamped: stay within 0 – 100. |
| `void Set_WheelIncrease()` / `void Set_WheelDecrease()` | — | Scroll wheel – **work in progress**, the direction is not reported yet. |

All `Set_*()` calls only update a RAM buffer: they are fast and can be called on every `loop()`.
The base reads the buffer by itself through the SPI interrupt.

## Debug helpers

| Method | Description |
|---|---|
| `void Print()` | Prints the 112 bits on `Serial` in wire order (14 groups of 8). Call `Serial.begin()` first. |
| `void printBits(byte value)` | Prints one byte, MSB first. |

## Converting an analog stick

A thumbstick read with `analogRead()` returns 0 – 1023 with its centre around 512. This is how the
[`complete_grip`](../examples/complete_grip/complete_grip.ino) example converts it:

```cpp
int raw = analogRead(A1);                         // X axis
Direction xDir = Direction::LEFT;
float xPercent = (512.0 - raw) * 100.0 / 512.0;
if (raw > 512) {
  xDir = Direction::RIGHT;
  xPercent = (raw - 512.0) * 100.0 / 512.0;
}
```

Same thing for Y with `Direction::DOWN` / `Direction::UP`. Then:

```cpp
grip.Set_JoystickValue(xDir, yDir, xPercent, yPercent, digitalRead(3) == LOW);
```

## Reading switches

Wire every switch between an Arduino pin and GND and use the internal pull-up:

```cpp
pinMode(2, INPUT_PULLUP);
...
grip.Set_Button(1, digitalRead(2) == LOW);
```

## Integration constraints

- AVR boards only (ATmega328P, ATmega32U4).
- The hardware SPI pins are reserved (D10 – D13 on a Nano/Uno).
- The library defines `ISR(SPI_STC_vect)` and `ISR(PCINT0_vect)`. It therefore cannot be combined with libraries
  that define the same vectors (e.g. `SoftwareSerial`, pin-change interrupt libraries) or with the SPI used as a
  master (SD cards, displays…).
- Do not keep interrupts disabled for long: the base expects an answer for every byte.
- `Serial` (hardware UART) works normally.
