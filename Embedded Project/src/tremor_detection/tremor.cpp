#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"
#include <math.h> 

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

// Accelerometer Setup
void acc_setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

double x,y,z;

double getMagnitude(double &x, double &y, double &z){
  x = CircuitPlayground.motionX();
  y = CircuitPlayground.motionY();
  z = CircuitPlayground.motionZ();

  return sqrt(x * x + y * y + z * z);
}

// ---- FFT Configuration ----
const uint16_t samples = 64;               // Must be power of 2
const double samplingFrequency = 21.33;    // 64 samples / 3 seconds

double vReal[samples];
double vImag[samples];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

// Collect Data from X-axis
void collectFFTData() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = getMagnitude(x,y,z);
    vImag[i] = 0.0;
    delay(1000.0 / samplingFrequency);   // ~46.8 ms per sample
  }
}

// Detect Tremor using FFT
void detect_tremor(String &label, float &frequency) {
  collectFFTData();

  FFT.dcRemoval();
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  frequency = FFT.majorPeak();   // Get dominant frequency in Hz
  // Serial.println("Detected Peak Frequency: " + String(frequency) + " Hz");
  applyMovingAverage(vReal,samples>>1,1);
  PrintVector(vReal, samples>>1, SCL_FREQUENCY);

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

  // Serial.println("Condition Detected: " + label);
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  Serial.println(">Hz:0:0|clr");
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	      break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
        break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
	      break;
    }
    if(scaleType!=SCL_PLOT)
    {
      Serial.print(">Hz:");
      Serial.print(abscissa, 6);
      if(scaleType==SCL_FREQUENCY)
        Serial.print(":");
      // Serial.print("");
    }
    Serial.print(vData[i], 4);
    Serial.println(":1627551892437|xy");

  }
  Serial.println("");
}

void applyMovingAverage(double *vData, uint16_t bufferSize, uint8_t windowSize) {
  double temp[bufferSize]; // temp buffer to store smoothed values

  for (uint16_t i = 0; i < bufferSize; i++) {
    double sum = 0;
    uint8_t count = 0;

    // Compute average over the window centered at i
    for (int j = -windowSize; j <= windowSize; j++) {
      int index = i + j;
      if (index >= 0 && index < bufferSize) {
        sum += vData[index];
        count++;
      }
    }
    temp[i] = sum / count;
  }

  // Copy back the smoothed values
  for (uint16_t i = 0; i < bufferSize; i++) {
    vData[i] = temp[i];
  }
}
