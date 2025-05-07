#include <Arduino.h>
#include "tremor_detection/tremor.h"
#include "led_logic/led.h"

void setup() {
  initLED();
  acc_setup();
}

void loop() {
  String type;
  float intensity;

  detect_tremor(type, intensity);   // Use FFT magnitude sums for intensity

  if (type == "normal") {
    handleLEDN(intensity);
  } else if (type == "tremor") {
    handleLEDTM(intensity);
  } else if (type == "dyskinesia") {
    handleLEDDK(intensity);
  } else if (type == "both") {
    handleLEDBoth(intensity);  // New LED logic for both tremor and dyskinesia
  } else {
    Serial.println("Unrecognized movement or noise.");
    // Optional: turn off LEDs or show a neutral color
  }

  delay(1000);  // Wait 1 second before next FFT scan
}
