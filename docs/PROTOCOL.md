# Protocol – how a Virpil base talks to its grip

> This is the result of reverse engineering: Virpil does not publish this protocol. The frame layout was
> captured from a genuine grip with the [`GripReader`](../extras/GripReader) sketch, then used by this
> library to emulate a grip on a real base. Values that have not been measured are flagged as such.

## Roles

| | Virpil base | Grip (genuine or emulated) |
|---|---|---|
| SPI role | **master** | **slave** |
| Drives | SCK, SS | MISO |
| MOSI | not connected | not connected |

The base polls the grip continuously. Each poll is one **transaction**:
SS goes LOW, the base clocks out the bits, the grip presents one bit on MISO per clock, then SS goes HIGH.

## Frame format

- **14 bytes = 112 bits** per transaction.
- **Bit numbering** (used everywhere in this project): **bit `n` is the n-th bit seen on MISO after SS goes
  LOW**, `n = 0 … 111`. It lives in byte `n / 8`, at position `n % 8`.
  The library sends bytes **LSB first**, so position 0 of a byte is the first bit on the wire.
- **Digital inputs are active LOW**: `1` = released, `0` = pressed (like classic shift-register grips with
  pull-up resistors).
- **Analog values** (12-bit brake lever, 2 × 8-bit thumbstick) are sent **most significant bit first**.
- A number of bits are never changed by the library. Their idle values were copied from a genuine grip and
  are kept as is (they may identify the grip model – do not change them).

The complete bit map is in [BITMAP.md](BITMAP.md).

Idle frame produced by `Initialize()`:

```
bytes (hex) : BF FF FF FF 6F 81 00 C8 00 00 00 00 A0 00
on the wire : 11111101 11111111 11111111 11111111 11110110 10000001 00000000
              00010011 00000000 00000000 00000000 00000000 00000101 00000000
```

## Reading a genuine grip (`extras/GripReader`)

The Arduino replaces the base and bit-bangs the bus:

```
D10 (green, SS/latch)  ‾‾|10 µs|______________________________________ … ______|‾‾
D13 (yellow, SCK)      ‾‾‾‾‾‾‾‾‾‾\_read_/‾‾‾‾\_read_/‾‾‾‾ … (112 clocks)
D12 (white, MISO)                bit 0        bit 1
```

1. Pulse SS/latch HIGH for 10 µs (the grip latches its inputs), then pull it LOW.
2. For each of the 112 bits: clock LOW, wait 2 µs, **sample MISO**, clock HIGH.
3. Bit number `n` is stored in `bits_readded[n]` and decoded on the serial monitor (115200 baud).

## Emulating a grip (the library)

### Set-up – `Initialize()`

1. Build the idle frame in a 14-byte buffer.
2. MISO as output, SCK as input, SS as input with pull-up.
3. Enable the hardware SPI **in slave mode** (`SPE = 1`, `MSTR = 0`), LSB first, SPI mode 0.
4. Enable the SPI *transfer complete* interrupt and a **pin-change interrupt on SS**.

### Answering the base – interrupts

An SPI slave must have the next byte **already loaded in `SPDR`** before the master starts clocking it.
The library therefore always pre-loads the *next* byte:

```
             SS↓                                                     SS↑            SS↓ (next poll)
MISO      :     [ buf[0] ][ buf[1] ][ buf[2] ] … [ buf[12] ][ buf[13] ]              [ buf[0] ] …
SPI ISR   :              ↑ load     ↑ load        ↑          ↑ load buf[0]
                          buf[1]     buf[2]        buf[13]    (end of frame)
SS ISR    :                                                        cursor = 0
```

- `ISR(SPI_STC_vect)` → `SPDR = buf[cursor + 1]` (or `buf[0]` after the last byte), `cursor++`.
- `ISR(PCINT0_vect)` → when SS goes **HIGH**, `cursor = 0`: the emulator re-synchronises on every frame.
- Your sketch only changes bits in the buffer through the `Set_*()` functions. There is no "send" call:
  the next poll from the base automatically carries the new state.

### Constraints

- The base must clock exactly 14 bytes per frame (it does).
- The master must leave a few microseconds between bytes for the interrupt to reload `SPDR`. Virpil bases do,
  but the exact SPI clock and polling rate have not been measured yet.
- The very first byte after power-up is whatever `SPDR` contains at reset (one possibly wrong frame).
- The library owns the `SPI_STC_vect` and `PCINT0_vect` interrupt vectors: see the
  [integration constraints](API.md#integration-constraints).
