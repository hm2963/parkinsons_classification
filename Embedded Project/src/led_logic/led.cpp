#include "led.h"

const int ledPin = 13;

void initLED() { //replace function name
  pinMode(ledPin, OUTPUT);
}

void handleLEDDK(float intensity) { //replace function name and inputs, must be same as header file
  //replace logic - this is example
  if (intensity > 0.5) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handleLEDTM(float intensity) { //replace function name and inputs, must be same as header file
  // put logic here
}

void handleLEDN(float intensity) { //replace function name and inputs, must be same as header file
  // put logic here
}

