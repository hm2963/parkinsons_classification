#include "led.h"
#include <Adafruit_CircuitPlayground.h>

// Initialize NeoPixels
void initLED() {
  CircuitPlayground.begin();
}

// Helper: map intensity (0.0 to ~1.5) to brightness (0 to 100)
int calculateBrightness(float intensity) {
  int brightness = int(intensity * 100);
  // if (brightness < 10) brightness = 10;    // Ensure visible output
  // if (brightness > 100) brightness = 100;  // Cap at max brightness
  return brightness;
}

// Handle Normal - Green
void handleLEDN(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0);  // Green
  }
}

// Handle Tremor (3–5 Hz) - Yellow
void handleLEDTM(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 0);  // Yellow
  }
}

// Handle Dyskinesia (5–7 Hz) - Red
void handleLEDDK(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);  // Red
  }
}

// Both (Tremor + Dyskinesia) - Purple
void handleLEDBoth(float intensity) {
  int brightness = calculateBrightness(intensity);
  CircuitPlayground.strip.setBrightness(brightness);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 240, 130, 240);  // Purple
  }
}
