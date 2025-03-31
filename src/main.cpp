/****************************************************************
  Filename   : Breakout_Board_LED_Test (FNK0091)
  
  Controller : ESP32-S3-WROOM Board Lite (FNK0099)

  Description: - Includes all GPIO pins on the board.
               - Wave pattern using paired left/right LEDs.
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

// Define a structure for LED pairs
struct LEDPair {
  int leftPin;
  int rightPin; // -1 means no pair (for pin 10)
};

// Define LED pairs (opposite left and right pins, use -1 for pin 10's pair)
const LEDPair ledPairs[] = {
// L | R 
  { 4,  1}, // pair 0
  { 5,  2}, // pair 1
  { 6, 42}, // pair 2
  { 7, 41}, // pair 3
  {15, 40}, // pair 4
  {16, 39}, // pair 5
  {17, 38}, // pair 6
  {18, 37}, // pair 7
  { 8, 36}, // pair 8
  { 3, 35}, // pair 9
  {46,  0}, // pair 10
  { 9, 45}, // pair 11
  {10, -1}, // pair 12 (would be 48, using -1)
  {11, 47}, // pair 13
  {12, 21}, // pair 14
  {13, 20}, // pair 15
  {14, 19}  // pair 16
};

const int pairCount = 17; // 0-16

// Timing variables
unsigned long previousWaveTime = 0;
unsigned long previousRainbowTime = 0;
const unsigned long waveInterval = 50;    // speed of GPIO wave (ms)
const unsigned long rainbowInterval = 20; // speed of rainbow effect (ms)

// State variables
bool movingDown = true;
int currentPosition = 0;
uint8_t hue = 0; // hue value for rainbow (0-255)

// SETUP
void setup() {
  // Initialize NeoPixel
  FastLED.addLeds<WS2812, NEOPIX_PIN, GRB>(leds, NEOPIX_COUNT);
  FastLED.setBrightness(50);
  
  // Initialize all GPIO pins in the pairs
  for (int i = 0; i < pairCount; i++) {
    pinMode(ledPairs[i].leftPin, OUTPUT);
    
    if (ledPairs[i].rightPin != -1) {
      pinMode(ledPairs[i].rightPin, OUTPUT);
    }
  }
}

// MAIN LOOP
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

  // Handle GPIO wave pattern (up/down with paired LEDs)
  if (currentMillis - previousWaveTime >= waveInterval) {
    previousWaveTime = currentMillis;
    
    // Turn off all GPIO LEDs first
    for (int i = 0; i < pairCount; i++) {
      digitalWrite(ledPairs[i].leftPin, LOW);
      if (ledPairs[i].rightPin != -1) {
        digitalWrite(ledPairs[i].rightPin, LOW);
      }
    }
    
    // Start with the 1st top pair
    digitalWrite(ledPairs[currentPosition].leftPin, HIGH);
    if (ledPairs[currentPosition].rightPin != -1) {
      digitalWrite(ledPairs[currentPosition].rightPin, HIGH);
    }
    
    // Handle wave direction and position updates
    if (movingDown) {
      // Moving down
      currentPosition++; // move to next position
      
      // Check if we've reached the bottom of the board
      if (currentPosition >= pairCount) {
          movingDown = false;              // reverse direction (now moving up)
          currentPosition = pairCount - 1; // stay at last valid position (avoid array overflow)
      }
    } 
    else {
      // Moving up
      currentPosition--; // move to previous position
      
      // Check if we've reached the top of the board
      if (currentPosition < 0) {
          movingDown = true;   // reverse direction (now moving down)
          currentPosition = 0; // stay at first position (avoid negative index)
      }
    }
  }
}
