#include <Arduino.h>
#include "tremor_detection/tremor.h"
#include "led_logic/led.h"
#include "sound_logic/sound.h"


void setup() {
  initLED();  // put init function from your feature
  initSound(); // put init function from your feature
}

void loop() {
  String type;
  float intensity;

  detect_tremor(type, intensity);  // modifies type & intensity by reference
  Serial.println("Tremor: " + type + ", intensity: " + String(intensity));

  if(type == "normal"){
    handleLEDN(intensity);
    handleSoundN(intensity);
  } 

  else if(type == "tremor"){
    handleLEDTM(intensity);
    handleSoundTM(intensity);
  } 
  
  else if (type == "dyskinesia") {
    handleLEDDK(intensity);
    handleSoundDK(intensity);
  }

  //default value, meaning didnt change variable or undefined
  else { 
    // must have logic for if detection did not happen
  }

  delay(1000);
}