#ifndef TREMOR_H
#define TREMOR_H

#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

// Function prototype
void detect_tremor(String &label, float &intensity);
void getdata();
void acc_setup();
#endif
