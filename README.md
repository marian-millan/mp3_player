<div align="center">

# Portable MP3 Player

### A custom, distraction-free music player inspired by retro physical media

**KiCad 9 · Arduino · DFPlayer Mini · SSD1306 OLED**

</div>

## Overview

We love music, but wanted a way to listen without constantly reaching for a
phone and getting distracted. Inspired by the tactile feel of retro physical
media, we set out to build a dedicated hardware MP3 player.

This repository contains versioned hardware designs, assembled-board models,
and Arduino firmware. The player combines physical transport controls and a
rotary volume encoder with a compact OLED interface.

## Features

- Standalone MP3 playback through a DFRobot DFPlayer Mini
- 128 × 64 SSD1306 OLED interface
- Previous, play/pause, and next track buttons
- Rotary encoder volume control
- Custom KiCad schematic and two-layer PCB layout
- STEP model of the assembled board

## Repository Layout

| Path | Contents |
| --- | --- |
| [`versions/v1/firmware`](versions/v1/firmware) | Version 1 Arduino firmware |
| [`versions/v1/hardware/mp3_player_schematic_v1.kicad_sch`](versions/v1/hardware/mp3_player_schematic_v1.kicad_sch) | Version 1 KiCad schematic |
| [`versions/v1/hardware/mp3_player_pcb_v1.kicad_pcb`](versions/v1/hardware/mp3_player_pcb_v1.kicad_pcb) | Version 1 KiCad PCB layout |
| [`versions/v1/hardware/mp3_player_assembly_v1.step`](versions/v1/hardware/mp3_player_assembly_v1.step) | Version 1 assembled-board model |
| [`versions/v2`](versions/v2) | Version 2 placeholder—in development |

```text
versions/
├── v1/
│   ├── firmware/
│   │   └── mp3_player_v1/
│   │       └── mp3_player_v1.ino
│   └── hardware/
│       ├── mp3_player_assembly_v1.step
│       ├── mp3_player_pcb_v1.kicad_pcb
│       └── mp3_player_schematic_v1.kicad_sch
└── v2/
    └── README.md
```

### Versioning Convention

Each release keeps its firmware and hardware together under
`versions/v<release>`. Project artifacts follow the
`descriptive_name_v<release>.<extension>` naming pattern.

New iterations should be added as sibling directories rather than overwriting
earlier versions. Arduino sketches are kept in matching directories because
the Arduino IDE expects a sketch and its containing directory to share a name.

## Firmware

### Requirements

- Arduino IDE or Arduino CLI
- An Arduino-compatible board configured for the project hardware
- [DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)
- [U8g2](https://github.com/olikraus/u8g2)

`SoftwareSerial`, `SPI`, and `Wire` are supplied by the Arduino platform.

### Pin Map

| Function | Pin |
| --- | ---: |
| Previous track button | D2 |
| Play/pause button | D3 |
| Next track button | D4 |
| Rotary encoder A | D8 |
| Rotary encoder B | D9 |
| DFPlayer serial | D10 / D11 |
| OLED display | Hardware I²C |

The buttons use the microcontroller's internal pull-up resistors and are active
low. Playback actions occur when a button is released.

### Upload

1. Install the `DFRobotDFPlayerMini` and `U8g2` libraries with the Arduino
   Library Manager.
2. Open
   [`versions/v1/firmware/mp3_player_v1/mp3_player_v1.ino`](versions/v1/firmware/mp3_player_v1/mp3_player_v1.ino).
3. Select the correct board and serial port.
4. Compile and upload the sketch.
5. Insert a prepared microSD card into the DFPlayer Mini and power the player.

On startup, the firmware initializes the display, sets the volume to `20`, and
starts the first track. The current interface labels tracks as
`trackNNN.mp3` using the file number reported by the DFPlayer.

## Current Status

Version 1 includes the complete design sources and an initial working firmware
implementation. The firmware currently focuses on core playback, volume
control, and a minimal track-number interface; metadata browsing and richer
error reporting are not yet implemented.

Version 2 is currently in development. Its directory is reserved, but no
hardware or firmware artifacts have been published yet.
