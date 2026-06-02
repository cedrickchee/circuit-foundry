---
name: arduino-sketch-workflow
description: Write, compile, upload, and verify Arduino Sketch C/C++ code using Arduino CLI and Bash. Use when Codex needs to create or modify `.ino` sketches, detect a connected Arduino board such as an UNO R3, install required Arduino libraries, compile with the correct FQBN, upload over a serial port such as `/dev/ttyACM0`, or check sketch output with `arduino-cli monitor`.
---

# Arduino Sketch Workflow

## Overview

Use Arduino CLI as the source of truth for board detection, installed cores/libraries, compilation, upload, and serial monitoring. Prefer making a complete working sketch, compiling it, uploading it when hardware is connected, and reporting the exact commands and results.

## Workflow

1. Inspect the local Arduino setup:

```sh
arduino-cli board list
arduino-cli core list
arduino-cli lib list
```

For an Arduino UNO R3, use `arduino:avr:uno` unless `board list` clearly reports a different FQBN.

2. Create sketches as Arduino expects:

- Put each sketch in its own folder.
- Make the folder name match the primary `.ino` filename.
- Place it under the user-requested directory.
- Avoid overwriting an existing sketch unless the user asked for that.

Example layout:

```text
arduino_starter_kit/
└── LcdDisplayTest/
    └── LcdDisplayTest.ino
```

3. Use standard Arduino libraries when appropriate.

For a 16x2 HD44780-compatible LCD wired in parallel mode, prefer `#include <LiquidCrystal.h>` and the standard Arduino API. If compilation reports `LiquidCrystal.h: No such file or directory`, install it:

```sh
arduino-cli lib install LiquidCrystal
```

4. Compile with an explicit build path.

This avoids sandbox or read-only cache failures under `~/.cache/arduino`.

```sh
arduino-cli compile --fqbn arduino:avr:uno --build-path /tmp/arduino-<sketch>-build <sketch-folder>
```

5. Upload using the compiled build output.

```sh
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno --input-dir /tmp/arduino-<sketch>-build <sketch-folder>
```

If serial discovery or upload fails with permissions such as `operation not permitted`, rerun the same hardware-facing command with the required host/USB access approval.

6. Verify output when relevant.

For serial sketches:

```sh
arduino-cli monitor -p /dev/ttyACM0 -b arduino:avr:uno -c baudrate=9600
```

Stop the monitor with `Ctrl+C`. Initial partial or garbled text can happen when opening the monitor while the board is resetting or already transmitting.

## Helper Script

Use `scripts/arduino_cli_workflow.sh` for repeatable compile/upload/monitor commands. It defaults to `FQBN=arduino:avr:uno`, writes builds under `/tmp/arduino-sketch-builds`, and supports environment overrides:

```sh
FQBN=arduino:avr:uno PORT=/dev/ttyACM0 scripts/arduino_cli_workflow.sh compile-upload arduino_starter_kit/LcdDisplayTest
```

Read or patch the script if a task needs different board behavior.

## References

Read `references/uno-r3-examples.md` when the task resembles this thread: UNO R3 bring-up, blink plus serial output, 16x2 LCD with `LiquidCrystal`, or troubleshooting Arduino CLI cache/library/USB issues.
