#include <Arduino.h>
#include "tremor_detection/tremor.h"
#include "led_logic/led.h"
#include "sound_logic/sound.h"

void setup() {
  initLED();
  initSound();
  acc_setup();
}

void loop() {
  String type;
  float intensity;

  detect_tremor(type, intensity);   // Real detection using FFT sums

  if (type == "normal") {
    handleLEDN(intensity);
    handleSoundN(intensity);
  } else if (type == "tremor") {
    handleLEDTM(intensity);
    handleSoundTM(intensity);
  } else if (type == "dyskinesia") {
    handleLEDDK(intensity);
    handleSoundDK(intensity);
  } else if (type == "both") {
    handleLEDBoth(intensity);
    // Add: handleSoundBoth(intensity); if you implement it
  } else {
    Serial.println("Unrecognized movement or noise.");
    // Optional: reset LEDs/sound
  }

  delay(1000);
}