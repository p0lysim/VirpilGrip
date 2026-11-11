# Demo grip

A 3D-printed grip designed in Fusion 360 to demonstrate the library on a real Virpil base.
It runs the [`complete_grip`](../examples/complete_grip/complete_grip.ino) example.

| Outside | Inside (left shell hidden) | Side |
|---|---|---|
| <img src="images/demo_grip_iso.png" width="260"/> | <img src="images/demo_grip_inside.png" width="260"/> | <img src="images/demo_grip_side.png" width="260"/> |

## Parts

| Part | Description |
|---|---|
| Right / left shells | Two 3D-printed half shells (≈ 38 × 158 × 92 mm each) |
| Tube | Ø 20 mm × 153 mm shaft between the shells and the mount |
| Knurled nut | Ø 48 mm nut that screws onto the Virpil base, like a genuine grip |
| Linkers + reinforcement | Printed parts clamping the tube to the nut |
| Arduino Nano | Housed in the shell, USB port reachable for flashing |
| Analog thumbstick module | 2-axis + push (KY-023 / PS2 type, 34 × 26 mm PCB), top of the grip |
| Push button | Ø 16 mm, thumb side |

Overall size ≈ 63 × 277 × 92 mm.

## Electronics

| Part | Arduino Nano pin |
|---|---|
| Base connector | see [WIRING.md](WIRING.md) (D10, D12, D13, 5V, GND) |
| Thumbstick X | A1 |
| Thumbstick Y | A0 |
| Thumbstick push | D3 (to GND, `INPUT_PULLUP`) |
| Push button | D2 (to GND, `INPUT_PULLUP`) |

The thumbstick and the button are reported to the base as the grip's thumbstick and **button 1**.

## CAD files

The printable files are not published yet.
