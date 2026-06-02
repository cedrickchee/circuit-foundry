# Circuit Foundry

Circuit Foundry is a practical showcase for using Codex to build, compile, upload, and verify embedded systems projects with Arduino-class hardware.

The project started as a proof of concept: Codex should be useful at the bench, not only in a software editor. It should help turn a box of electronics parts, a microcontroller, and a rough goal into working firmware, repeatable commands, and documented hardware behavior.

This repository contains two deliverables:

- `skills/arduino-sketch-workflow`: a Codex skill for Arduino sketch work using Arduino CLI.
- `arduino_starter_kit`: Arduino UNO R3 examples, including board setup, sketches, C/C++ code, and proof images.


<table>
  <tr>
    <th>Arduino UNO R3 with LCD wiring</th>
    <th>LCD art demo running on Arduino UNO R3</th>
  </tr>
  <tr>
    <td>
      <p align="center" width="100%">
        <img width="60%"  alt="Arduino UNO R3 with LCD wiring" src="https://raw.githubusercontent.com/cedrickchee/circuit-foundry/refs/heads/main/docs/images/uno-r3-lcd-setup.jpg" />
      </p>
    </td>
    <td>
      <p align="center" width="100%">
        <img width="60%"  alt="LCD art demo running on Arduino UNO R3" src="https://raw.githubusercontent.com/cedrickchee/circuit-foundry/refs/heads/main/docs/images/lcd-art-demo.jpg" />
      </p>
    </td>
  </tr>
</table>

## What Is Included

| Path | Purpose | Hardware |
| --- | --- | --- |
| `arduino_starter_kit/HelloWorldSketch` | Minimal built-in LED blink sketch. | Arduino UNO R3 |
| `arduino_starter_kit/UnoR3Test` | Built-in LED blink plus serial verification output. | Arduino UNO R3 |
| `arduino_starter_kit/LcdDisplayTest` | 16x2 LCD uptime display with serial verification logs. | UNO R3, HD44780-compatible 16x2 LCD |
| `arduino_starter_kit/LcdExecutiveArtDemo` | Animated LCD demo with custom character glyphs and scene transitions. | UNO R3, HD44780-compatible 16x2 LCD |
| `skills/arduino-sketch-workflow` | Reusable Codex workflow for writing, compiling, uploading, and monitoring sketches. | Arduino CLI-compatible boards |

## Quick Start

Install Arduino CLI, then confirm the local toolchain:

```sh
arduino-cli version
arduino-cli core list
arduino-cli lib list
```

For Arduino UNO R3, install the AVR core if it is missing:

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
```

For the LCD examples, install `LiquidCrystal` if it is missing:

```sh
arduino-cli lib install LiquidCrystal
```

Compile a sketch with an explicit build path:

```sh
arduino-cli compile --fqbn arduino:avr:uno --build-path /tmp/arduino-uno-r3-test-build arduino_starter_kit/UnoR3Test
```

Upload the compiled output to a connected UNO R3:

```sh
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno --input-dir /tmp/arduino-uno-r3-test-build arduino_starter_kit/UnoR3Test
```

Open the serial monitor for sketches that print verification output:

```sh
arduino-cli monitor -p /dev/ttyACM0 -b arduino:avr:uno -c baudrate=9600
```

The serial port may be different on your machine. Run `arduino-cli board list` to discover the port when host USB access is available.

## Arduino Sketch Workflow Skill

The Codex skill in `skills/arduino-sketch-workflow` captures the repeatable workflow used for this project:

1. Inspect board, core, and library state with Arduino CLI.
2. Create sketches in Arduino-compatible folder layouts.
3. Prefer standard Arduino libraries when they fit the component.
4. Compile with explicit build paths under `/tmp`.
5. Upload using the compiled output.
6. Verify through serial monitor output or observable hardware behavior.

The helper script wraps the common commands:

```sh
skills/arduino-sketch-workflow/scripts/arduino_cli_workflow.sh core-list
skills/arduino-sketch-workflow/scripts/arduino_cli_workflow.sh lib-list
skills/arduino-sketch-workflow/scripts/arduino_cli_workflow.sh compile arduino_starter_kit/LcdDisplayTest
```

Use environment variables to target another board or port:

```sh
FQBN=arduino:avr:uno PORT=/dev/ttyACM0 skills/arduino-sketch-workflow/scripts/arduino_cli_workflow.sh compile-upload arduino_starter_kit/LcdDisplayTest
```

To install the skill into a local Codex skill directory, copy `skills/arduino-sketch-workflow` into your Codex skills location.

## Hardware Notes

The LCD examples use a 16x2 HD44780-compatible LCD in 4-bit parallel mode with the standard `LiquidCrystal` library.

Common Arduino Starter Kit wiring:

```text
RS -> 12
E  -> 11
D4 -> 5
D5 -> 4
D6 -> 3
D7 -> 2
```

The LCD circuit also needs power, ground, contrast adjustment, and backlight wiring appropriate for the module. See `arduino_starter_kit/README.md` for the focused example walkthrough.

## Troubleshooting

- `Cannot create build directory`: compile with `--build-path /tmp/<name>-build` or use the helper script.
- `LiquidCrystal.h: No such file or directory`: run `arduino-cli lib install LiquidCrystal`.
- `operation not permitted` during board discovery: host USB or serial access is blocked; rerun the command in an environment with device permissions.
- `No boards found`: check the USB cable, board power LED, and expected serial port such as `/dev/ttyACM0`.
- Upload success may print only `New upload port: /dev/ttyACM0 (serial)`.
- Serial monitor output may start with a partial line while the board resets; repeated clean lines after that are enough to confirm behavior.

## Verified Local Setup

The examples were checked with:

- Arduino CLI `1.5.0`
- Arduino AVR Boards core `arduino:avr 1.8.8`
- `LiquidCrystal 1.0.7`
- FQBN `arduino:avr:uno`

All sketches compile for Arduino UNO R3. In this sandboxed environment, `arduino-cli board list` reports `operation not permitted`, so hardware discovery and upload commands are documented but not claimed as verified inside the sandbox.

## License

Circuit Foundry is released under the MIT License. See `LICENSE`.
