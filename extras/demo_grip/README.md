# Demo grip – 3D files

The 3D files of the [demo grip](../../docs/DEMO_GRIP.md), a simple 3D-printed grip made to test the library on a
real base. It runs the [`complete_grip`](../../examples/complete_grip/complete_grip.ino) example.

Exported from the Fusion 360 model, units: **mm**.

| File | Part |
|---|---|
| [`STL/shell_right.stl`](STL/shell_right.stl) | Right half shell |
| [`STL/shell_left.stl`](STL/shell_left.stl) | Left half shell |
| [`STL/ring_nut.stl`](STL/ring_nut.stl) | Knurled Ø 48 mm nut that screws onto the base |
| [`STL/tube.stl`](STL/tube.stl) | Ø 20 mm × 153 mm shaft |
| [`STL/linker_left.stl`](STL/linker_left.stl), [`STL/linker_right.stl`](STL/linker_right.stl) | Linkers holding the tube in the nut |
| [`STL/mount_reinforcement.stl`](STL/mount_reinforcement.stl) | Reinforcement fitted inside the top of the shells |
| [`STEP/demo_grip_assembly.step`](STEP/demo_grip_assembly.step) | Full assembly: the parts above plus the Arduino Nano, the thumbstick module and the push button, to see where everything goes |
| [`3MF/`](3MF) | Ready-to-print projects – coming soon |

- The STL files keep their position in the assembly: lay each part flat in your slicer before printing.
- Open an STL file on GitHub to get an interactive 3D preview.
- This folder is left out of the ZIP archives of the repository (**Code → Download ZIP**, release source code)
  to keep the library download small: download the files from here or clone the repository.
