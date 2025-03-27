/****************************************************************
  Filename   : Breakout_Board_LED_Test (FNK0091)
  
  Controller : ESP32-S3-WROOM Board Lite (FNK0099)

  Description: - Includes all GPIO pins on the board.
				       - Wave pattern on both sides going up & down.
				       - Full spectrum rainbow hue effect on NeoPixel.
               - Uses millis() for non-blocking operation.
               - Adjust waveInterval to change wave speed.

  Notes      : - NeoPixel pin must be skipped in the array (48).
               - Do not have anything connected to the breakout
                 board GPIO pins - this is simply to test the 
                 board & LED functionality.
****************************************************************/


#include <Arduino.h>
#include <FastLED.h>

// NeoPixel configuration
#define NEOPIX_COUNT 1
#define NEOPIX_PIN 48
CRGB leds[NEOPIX_COUNT];

// GPIO wave pattern configuration
const int leftPins[] = {4, 5, 6, 7, 16, 17, 18, 8, 3, 46, 9, 10, 11, 12, 13, 14};
const int rightPins[] = {1, 2, 42, 41, 40, 39, 38, 37, 36, 35, 0, 45, 47, 21, 20, 19}; // skip neopixel pin (48)
const int leftCount = sizeof(leftPins) / sizeof(leftPins[0]);
const int rightCount = sizeof(rightPins) / sizeof(rightPins[0]);

// Timing variables
unsigned long previousWaveTime = 0;
unsigned long previousRainbowTime = 0;
const unsigned long waveInterval = 50;    // speed of GPIO wave (ms)
const unsigned long rainbowInterval = 20; // speed of rainbow effect (ms)

// State variables
bool movingDown = true;
int currentPosition = 0;
uint8_t hue = 0; // hue value for rainbow (0-255)

void setup() {
  // Initialize NeoPixel
  FastLED.addLeds<WS2812, NEOPIX_PIN, GRB>(leds, NEOPIX_COUNT);
  FastLED.setBrightness(50);
  
  // Initialize all GPIO pins
  for (int i = 0; i < leftCount; i++) {
    pinMode(leftPins[i], OUTPUT);
    digitalWrite(leftPins[i], LOW);
  }
  for (int i = 0; i < rightCount; i++) {
    pinMode(rightPins[i], OUTPUT);
    digitalWrite(rightPins[i], LOW);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle Rainbow Effect on NeoPixel
  if (currentMillis - previousRainbowTime >= rainbowInterval) {
    previousRainbowTime = currentMillis;
    
    // Cycle through hue values (0-255)
    leds[0] = CHSV(hue++, 255, 255); // full saturation and brightness
    FastLED.show();
    
    if (hue >= 255) hue = 0; // reset hue after full spectrum
  }

  // Handle GPIO wave pattern (up/down on both sides)
  if (currentMillis - previousWaveTime >= waveInterval) {
    previousWaveTime = currentMillis;
    
    // Turn off all GPIO LEDs first
    for (int i = 0; i < leftCount; i++) digitalWrite(leftPins[i], LOW);
    for (int i = 0; i < rightCount; i++) digitalWrite(rightPins[i], LOW);
    
    // Light current position
    if (currentPosition < leftCount) digitalWrite(leftPins[currentPosition], HIGH);
    if (currentPosition < rightCount) digitalWrite(rightPins[currentPosition], HIGH);
    
    // Update position
    if (movingDown) {
      currentPosition++;
      if (currentPosition >= max(leftCount, rightCount)) {
        movingDown = false;
        currentPosition = min(leftCount, rightCount) - 1;
      }
    }
	else {
      currentPosition--;
      if (currentPosition < 0) {
        movingDown = true;
        currentPosition = 0;
      }
    }
  }
}
