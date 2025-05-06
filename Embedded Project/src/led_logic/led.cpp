#include "led.h"

#include <Adafruit_CircuitPlayground.h>

// Initialize NeoPixels
void initLED() {
  CircuitPlayground.begin();
}

// Helper function to map intensity to brightness (0-100)
int calculateBrightness(float intensity) {
  int brightness = int(intensity * 100);
  if (brightness < 10) brightness = 10;
  if (brightness > 100) brightness = 100;
  return brightness;
}

// Handle Normal (low intensity movement) - Green
void handleLEDN(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);  // Green
  }
}

// Handle Tremor (3-5 Hz) - Yellow
void handleLEDTM(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 0);  // Yellow
  }
}

// Handle Dyskinesia (5-7 Hz) - Red
void handleLEDDK(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);  // Red
  }
}

// Handle Both Conditions Detected - Orange (blend of Red and Yellow)
void handleLEDBoth(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 128, 0);  // Orange
  }
}
