#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"

// FFT Configuration
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

// Collect Data: Use magnitude of X, Y, Z
void collectFFTData() {
  for (int i = 0; i < samples; i++) {
    float x = CircuitPlayground.motionX();
    float y = CircuitPlayground.motionY();
    float z = CircuitPlayground.motionZ();

    // Calculate magnitude
    vReal[i] = sqrt(x * x + y * y + z * z);
    vImag[i] = 0.0;

    delay(1000.0 / samplingFrequency);  // Sampling delay (~46.8 ms per sample)
  }
}

// Detect Tremor or Dyskinesia
void detect_tremor(String &label, float &intensity) {
  collectFFTData();

  // Basic movement filter: low average magnitude means device is still
  double sumMag = 0.0;
  for (int i = 0; i < samples; i++) {
    sumMag += vReal[i];
  }
  double avgMag = sumMag / samples;
  if (avgMag < 1.0) {
    label = "normal";
    intensity = 0.0;
    Serial.println("No significant movement detected. Avg Magnitude: " + String(avgMag));
    return;
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  double tremorSum = 0.0;
  double dyskinesiaSum = 0.0;

  // Get frequency bin resolution
  double binWidth = (samplingFrequency / samples); // ~0.333 Hz per bin

  for (int i = 0; i < (samples / 2); i++) {
    double freq = i * binWidth;

    if (freq >= 1.0 && freq <= 2.0) {
      tremorSum += vReal[i];
    } else if (freq > 3.0 && freq <= 4.0) {
      dyskinesiaSum += vReal[i];
    }
  }

  const double threshold = 4.0;  // Increased threshold to reduce false positives

  // Detect both symptoms if both exceed threshold
  if (tremorSum > threshold && dyskinesiaSum > threshold) {
    label = "both";
    intensity = max((tremorSum - threshold), (dyskinesiaSum - threshold)) / threshold;
  }
  else if (tremorSum > threshold) {
    label = "tremor";
    intensity = (tremorSum - threshold) / threshold;
  }
  else if (dyskinesiaSum > threshold) {
    label = "dyskinesia";
    intensity = (dyskinesiaSum - threshold) / threshold;
  } else {
    label = "normal";
    intensity = 0.0;
  }

  Serial.println("Condition Detected: " + label);
  Serial.println("Intensity: " + String(intensity));
}