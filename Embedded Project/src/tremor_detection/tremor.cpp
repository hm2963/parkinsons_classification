#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"

// ---- FFT Configuration ----
const uint16_t samples = 64;               // Must be power of 2
const double samplingFrequency = 21.33;    // 64 samples / 3 seconds

double vReal[samples];
double vImag[samples];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

// Accelerometer Setup
void acc_setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

// Collect Data from X-axis
void collectFFTData() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = CircuitPlayground.motionX();
    vImag[i] = 0.0;
    delay(1000.0 / samplingFrequency);   // ~46.8 ms per sample
  }
}

// Detect Tremor using FFT
void detect_tremor(String &label, float &frequency) {
  collectFFTData();

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  frequency = FFT.majorPeak();   // Get dominant frequency in Hz
  Serial.println("Detected Peak Frequency: " + String(frequency) + " Hz");

  // Classify based on frequency
  if (frequency < 2.0) {
    label = "normal";
  } else if (frequency < 4.0) {
    label = "tremor";
  } else if (frequency < 6.0) {
    label = "dyskinesia";
  } else {
    label = "unknown";
  }

  Serial.println("Condition Detected: " + label);
}