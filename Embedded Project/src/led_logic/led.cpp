#include "led.h"

#include <Adafruit_CircuitPlayground.h>

// Initialize NeoPixels
void initLED() {
  CircuitPlayground.begin();
}

// Helper function to map frequency to brightness (0-100)
int calculateBrightness(float freq, float minFreq, float maxFreq) {
  float normalized = (freq - minFreq) / (maxFreq - minFreq);
  int brightness = int(normalized * 100);  // NeoPixel brightness scale is 0-100
  if (brightness < 10) brightness = 10;    // Ensure minimum visibility
  if (brightness > 100) brightness = 100;
  return brightness;
}

// Handle Normal (0-3 Hz) - Green
void handleLEDN(float frequency) {
  int brightness = calculateBrightness(frequency, 0.0, 2.0);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);  // Green
  }
}

// Handle Tremor (3-5 Hz) - Yellow
void handleLEDTM(float frequency) {
  int brightness = calculateBrightness(frequency, 2.0, 4.0);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 0);  // Yellow (Red + Green)
  }
}

// Handle Dyskinesia (5-7 Hz) - Red
void handleLEDDK(float frequency) {
  int brightness = calculateBrightness(frequency, 4.0, 6.0);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);  // Red
  }
}
