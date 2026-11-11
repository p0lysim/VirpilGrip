# Changelog

## 1.1.0 – unreleased

Repository reorganisation – **the library code is unchanged**.

- Arduino library 1.5 layout: sources moved to `src/`, `library.properties` and `keywords.txt` added
  (installable from the GitHub ZIP, ready for the Arduino Library Manager).
- New documentation: `docs/` (wiring, protocol, bit map, API, demo grip) and a complete README.
- New illustrations: overview picture of the demo grip on a Virpil / Thrustmaster base, "how it works"
  diagram (SVG, light and dark themes) and a wiring diagram showing where the Arduino and the connector sit
  in the grip, the connector pinout and the Arduino Nano wiring.
- 3D files of the demo grip in `extras/demo_grip/`: STL of every printed part and STEP of the full assembly
  (3MF projects to come).
- Development tools published in `extras/`: `GripReader` (reads a genuine grip), `SpiSlaveTest`
  and the PlatformIO development project.
- GitHub Actions workflow compiling every example for Uno, Nano, Micro and Leonardo.

## 1.0.0 – 2025-02-15

- First public version: `VirpilGrip` library, six examples, wiring pictures.
