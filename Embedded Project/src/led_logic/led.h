#ifndef LED_H
#define LED_H

#include <Arduino.h>

// Initialize NeoPixels
void initLED();

// LED handling functions for each condition
void handleLEDN(float frequency);     // Normal (0-3 Hz) - Green
void handleLEDTM(float frequency);    // Tremor (3-5 Hz) - Yellow
void handleLEDDK(float frequency);    // Dyskinesia (5-7 Hz) - Red
void handleLEDBoth(float intensity);
// Helper function to calculate brightness based on frequency
int calculateBrightness(float freq, float minFreq, float maxFreq);

#endif
