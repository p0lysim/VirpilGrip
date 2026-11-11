# Extras – development tools and demo grip

Nothing in this folder is compiled by the Arduino IDE when the library is installed: these are the tools
that were used to build it, and the 3D files of the demo grip.

| Folder | What it is |
|---|---|
| [`GripReader/`](GripReader/GripReader.ino) | Reads a **genuine Virpil grip** from an Arduino (the Arduino plays the base). This is how the frame and the bit map were reverse engineered. |
| [`SpiSlaveTest/`](SpiSlaveTest/SpiSlaveTest.ino) | The very first proof of concept: a minimal SPI slave that answers fixed bytes. |
| [`platformio/`](platformio) | PlatformIO / VS Code project used to develop and test the library. |
| [`demo_grip/`](demo_grip) | STL and STEP files of the [demo grip](../docs/DEMO_GRIP.md), a 3D-printed grip to test the library (3MF projects coming soon). |

## GripReader

Wire the grip to the Arduino (the Arduino powers the grip):

| Grip wire | Arduino |
|---|---|
| Yellow (SCK) | D13 – clock output |
| Green (SS) | D10 – latch / select output |
| White (MISO) | D12 – data input |
| Red (+5 V) | 5V |
| Black (GND) | GND |

Open the serial monitor at **115200 baud**. By default it prints the 14 raw bytes and the decoded thumbstick;
uncomment the other `print_*()` calls in `loop()` to decode buttons, hats, wheel, brake lever and trigger.

## PlatformIO project

The project links the library sources of this repository (`lib_deps = symlink://../..`), so anything you change
while testing here is changed in the library itself.

```bash
cd extras/platformio
pio run                  # build for the default board (Arduino Nano)
pio run -e micro         # build for an Arduino Micro
pio run -t upload        # flash
pio device monitor       # 115200 baud
```

`src/main.cpp` is a test bench: it sweeps the thumbstick Y axis and prints the frame with `grip.Print()`.
Commented blocks test the other controls.
