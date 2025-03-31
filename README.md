Filename: Breakout_Board_LED_Test (FNK0091)

Controller: ESP32-S3-WROOM Board Lite (FNK0099)

Description:

- Includes all usable GPIO pins on the board.
- Wave pattern on both sides going up & down.
- Full spectrum rainbow hue effect on NeoPixel.
- Uses millis() for non-blocking operation.
- Adjust waveInterval value to change wave speed.

Notes:

- NeoPixel pin must be skipped in the array (48).
- Do not have anything connected to the breakout board GPIO pins - this is simply to test the board & LED functionality.
