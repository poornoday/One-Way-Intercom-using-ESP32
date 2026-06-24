# ESP32 One-Way Intercom

## Overview

This project implements a real-time wireless one-way intercom system using two ESP32 development boards. Audio is captured using an INMP441 I2S microphone, transmitted wirelessly via UDP over Wi-Fi, and reproduced using the ESP32 DAC connected to a PAM8403 amplifier and speaker.

## Features

* Real-time audio transmission
* ESP32 to ESP32 wireless communication
* UDP-based low-latency streaming
* INMP441 digital microphone support
* Built-in voice activity indication LED
* DAC audio playback
* Compact and low-cost design

## Hardware Components

* 2 × ESP32 Development Boards
* INMP441 I2S Microphone
* PAM8403 Audio Amplifier
* Speaker
* Jumper Wires
* Power Supply

## Working Principle

### Transmitter

1. ESP32 operates as a Wi-Fi Access Point.
2. INMP441 captures audio through I2S.
3. Audio samples are converted to 16-bit PCM.
4. Audio packets are sent through UDP.

### Receiver

1. ESP32 connects to transmitter Wi-Fi.
2. UDP packets are received.
3. Audio samples are reconstructed.
4. ESP32 DAC outputs analog audio.
5. PAM8403 amplifies the signal for speaker playback.

## Pin Connections

### INMP441 → ESP32

| INMP441 | ESP32  |
| ------- | ------ |
| WS      | GPIO15 |
| SD      | GPIO32 |
| SCK     | GPIO14 |
| VCC     | 3.3V   |
| GND     | GND    |

### DAC Output

| ESP32  | PAM8403     |
| ------ | ----------- |
| GPIO25 | Audio Input |
| GND    | GND         |

## Future Improvements

* Two-way communication
* Audio compression
* Noise suppression
* OLED display integration
* Battery-powered operation

## Technologies Used

* ESP32
* Arduino IDE
* Wi-Fi
* UDP Networking
* I2S Audio Interface

## Author

K Poornoday Sriram
