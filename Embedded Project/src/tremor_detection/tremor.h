#ifndef TREMOR_H
#define TREMOR_H

#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

// Function prototype
void acc_setup();
void collectFFTData();
void detect_tremor(String &label, float &frequency);
void getdata();
void acc_setup();
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);
void applyMovingAverage(double *vData, uint16_t bufferSize, uint8_t windowSize);
int find_peaks(const double* magnitudes, int size, int* peak_indices, 
    double threshold_ratio = 0.3, int min_distance = 1);
#endif
