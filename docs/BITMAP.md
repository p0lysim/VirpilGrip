# Bit map of the 112-bit frame

Bit `n` = n-th bit on MISO after SS goes LOW · byte = `n / 8` · **`0` = active** unless stated otherwise.
Indices come from the `_IDX_*` members of [`VirpilGrip.h`](../src/VirpilGrip.h).

## By control

| Control | API call | Bits | Idle | Active |
|---|---|---|---|---|
| Button 1 | `Set_Button(1, pressed)` | b11 | 1 | 0 |
| Button 2 | `Set_Button(2, pressed)` | b1 | 1 | 0 |
| Button 3 | `Set_Button(3, pressed)` | b25 | 1 | 0 |
| 4-way hat #1 | `Set_Hat4direction(1, click, dir)` | centre b0, up b15, down b13, right b14, left b12 | all 1 | centre = 0 **and** direction bit = 0; push alone = centre only |
| 4-way hat #2 | `Set_Hat4direction(2, click, dir)` | centre b10, up b9, down b23, right b16, left b22 | all 1 | same |
| 4-way hat #3 | `Set_Hat4direction(3, click, dir)` | centre b17, up b24, down b30, right b31, left b29 | all 1 | same |
| 2-way hat | `Set_Hat2direction(1, dir)` | centre b28, up b27, down b26 | all 1 | centre = 0 and direction bit = 0 |
| Scroll wheel | `Set_WheelIncrease()` / `Set_WheelDecrease()` | b18, b19 (toggle together) | 1, 1 | toggles on each call – *work in progress* |
| Trigger, 1st stage | `Set_FireButton(_, Intensity::SOFT)` | b4 | 1 | 0 |
| Trigger, 2nd stage | `Set_FireButton(_, Intensity::HARD)` | b3 **and** b4 | 1 | 0 |
| Trigger safety lever | `Set_FireButton(leverUp, _)` | b6 | **0 = up** | **1 = down** (inverted logic) |
| Thumbstick push | `Set_JoystickValue(…, click)` | b2 | 1 | 0 |
| Thumbstick Y value | `Set_JoystickValue(…)` | b80 (MSB) … b87 (LSB) | 0 | 0 – 255 |
| Thumbstick X value | `Set_JoystickValue(…)` | b88 (MSB) … b95 (LSB) | 0 | 0 – 255 |
| Thumbstick X side | `Set_JoystickValue(…)` | b100, b101 | 0, 1 | LEFT: b100 = 1, b101 = 0 · RIGHT: b100 = 0, b101 = 1 |
| Thumbstick Y side | `Set_JoystickValue(…)` | b102, b103 | 0, 1 | UP: b102 = 1, b103 = 0 · DOWN: b102 = 0, b103 = 1 |
| Brake lever, 12-bit value | `Set_BreakValue(percent)` | value bits 11…6 → b66…b71, bits 5…0 → b49…b54 | 0 | 0 – 4095 |
| Brake lever, end-of-travel switch | `Set_BreakValue(percent)` | b32 | 1 | 0 when value > 1024 (≈ 25 %) |

## By byte

| Byte | Bits | pos 0 | pos 1 | pos 2 | pos 3 | pos 4 | pos 5 | pos 6 | pos 7 | Idle |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 0–7 | HAT1 centre | BTN2 | STICK push | TRIG stage 2 | TRIG stage 1 | const 1 | TRIG lever | const 1 | `BF` |
| 1 | 8–15 | const 1 | HAT2 up | HAT2 centre | BTN1 | HAT1 left | HAT1 down | HAT1 right | HAT1 up | `FF` |
| 2 | 16–23 | HAT2 right | HAT3 centre | WHEEL A | WHEEL B | const 1 | const 1 | HAT2 left | HAT2 down | `FF` |
| 3 | 24–31 | HAT3 up | BTN3 | 2-WAY down | 2-WAY up | 2-WAY centre | HAT3 left | HAT3 down | HAT3 right | `FF` |
| 4 | 32–39 | BRAKE switch | const 1 | const 1 | const 1 | const 0 | const 1 | const 1 | const 0 | `6F` |
| 5 | 40–47 | const 1 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | const 1 | `81` |
| 6 | 48–55 | const 0 | BRAKE b5 | BRAKE b4 | BRAKE b3 | BRAKE b2 | BRAKE b1 | BRAKE b0 | const 0 | `00` |
| 7 | 56–63 | const 0 | const 0 | const 0 | const 1 | const 0 | const 0 | const 1 | const 1 | `C8` |
| 8 | 64–71 | const 0 | const 0 | BRAKE b11 | BRAKE b10 | BRAKE b9 | BRAKE b8 | BRAKE b7 | BRAKE b6 | `00` |
| 9 | 72–79 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | `00` |
| 10 | 80–87 | STICK Y b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 | `00` |
| 11 | 88–95 | STICK X b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 | `00` |
| 12 | 96–103 | const 0 | const 0 | const 0 | const 0 | X left | X right | Y up | Y down | `A0` |
| 13 | 104–111 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | const 0 | `00` |

"Idle" is the byte value in memory (position 0 = least significant bit) right after `Initialize()`.

## Thumbstick encoding

`Set_JoystickValue(xDir, yDir, xPercent, yPercent, click)`, percentages from 0 to 100:

| Call | 8-bit value | Side bits |
|---|---|---|
| X `LEFT` 0 % → 100 % | 0 → 255 | b100 = 1, b101 = 0 |
| X `RIGHT` 0 % → 100 % | 255 → 0 | b100 = 0, b101 = 1 |
| Y `UP` 0 % → 100 % | 0 → 255 | b102 = 1, b103 = 0 |
| Y `DOWN` 0 % → 100 % | 255 → 0 | b102 = 0, b103 = 1 |
| X or Y `IDLE` | 0 | same side bits as `RIGHT` / `DOWN` |

So an axis is an 8-bit magnitude plus two side bits, the value being complemented on the RIGHT/DOWN side:
the travel is continuous from LEFT 100 % through the centre to RIGHT 100 %.
Note that, with this encoding, `IDLE` reads like a full RIGHT/DOWN deflection: prefer an explicit side with
0 % for a centred stick.

## Brake lever encoding

`value = percent × 4095 / 100` (12 bits), sent MSB first and split in two chunks:
bits 11…6 on b66…b71 and bits 5…0 on b49…b54. Bit b32 goes LOW above 1024 to emulate the end-of-travel switch.
