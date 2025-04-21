#include "sound.h"
#include <Arduino.h>

const int ledPin = 13;

void initSound() { //replace function name
  pinMode(ledPin, OUTPUT);
}

void handleSoundDK(float intensity) { //replace function name and logic
  if (intensity > 0.5) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handleSoundTM(float intensity) { //replace function name and logic
  //replace logic
}

void handleSoundN(float intensity) { //replace function name and logic
  //replace logic
}
