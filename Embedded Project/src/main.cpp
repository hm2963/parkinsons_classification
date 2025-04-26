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
  float freq;

  detect_tremor(type, freq);   // Perform real detection

  if (type == "normal") {
    handleLEDN(freq);
    handleSoundN(freq);
  } else if (type == "tremor") {
    handleLEDTM(freq);
    handleSoundTM(freq);
  } else if (type == "dyskinesia") {
    handleLEDDK(freq);
    handleSoundDK(freq);
  } else {
    Serial.println("Unrecognized movement or noise.");
    // Optional: Turn off LEDs or set a neutral color
  }

  delay(1000);  // Small delay before next detection cycle
}
