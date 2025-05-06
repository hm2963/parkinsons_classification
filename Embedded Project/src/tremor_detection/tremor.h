#ifndef TREMOR_H
#define TREMOR_H

#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

// Function prototype
void acc_setup();
void collectFFTData();
void detect_tremor(String &label, float &intensity);
void acc_setup();
#endif
