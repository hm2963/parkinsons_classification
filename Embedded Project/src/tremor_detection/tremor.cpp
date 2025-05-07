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
void detect_tremor(String &label, float &intensity) {
  collectFFTData();

  FFT.dcRemoval();
  FFT.windowing(FFTWindow::Hann, FFTDirection::Forward);	
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  applyMovingAverage(vReal, samples >> 1, 2);
  PrintVector(vReal, samples >> 1, SCL_FREQUENCY);

  // Sum magnitudes in tremor and dyskinesia frequency ranges
  double binWidth = (samplingFrequency / samples); // ~0.333 Hz per bin
  double tremorSum = 0.0;
  double dyskinesiaSum = 0.0;
  double totalEnergy = 0.0;

  const double MIN_TOTAL_ENERGY = 30;  // <<< Experiment with this value

  for (uint16_t i = 1; i < (samples / 2); i++) {
    double freq = i * binWidth;
    double mag = vReal[i];
    double energy = mag * mag;

    if (mag >= 0.3) {  // Threshold to ignore small/noisy signals
      totalEnergy += energy;
    
      if (freq >= 3.0 && freq <= 5.0) {
        tremorSum += energy;
      } else if (freq > 5.0 && freq <= 7.0) {
        dyskinesiaSum += energy;
      }
    }

    // Print magnitude for each frequency bin
    // Serial.print("Freq: ");
    // Serial.print(freq, 2);
    // Serial.print(" Hz, Mag: ");
    // Serial.print(mag, 4);
    // Serial.print(", Energy: ");
    // Serial.println(energy, 4);
  }

  // Print summary stats
  // Serial.print("Total Energy: ");
  // Serial.println(totalEnergy, 4);
  // Serial.print("Tremor Energy Sum: ");
  // Serial.println(tremorSum, 4);
  // Serial.print("Dyskinesia Energy Sum: ");
  // Serial.println(dyskinesiaSum, 4);

  double tremorRatio = tremorSum / totalEnergy;
  double dyskinesiaRatio = dyskinesiaSum / totalEnergy;

  Serial.print("Tremor Ratio: ");
  Serial.println(tremorRatio, 4);
  Serial.print("Dyskinesia Ratio: ");
  Serial.println(dyskinesiaRatio, 4);

  if (totalEnergy < MIN_TOTAL_ENERGY) {
    label = "normal";
    intensity = 10.0;
  }
  else if (tremorRatio >= 0.16 && dyskinesiaRatio >= 0.16) {
    label = "both";
    intensity = fmax(tremorRatio, dyskinesiaRatio);
  } 
  else if (tremorRatio >= 0.33) {
    label = "tremor";
    intensity = tremorRatio;
  } 
  else if (dyskinesiaRatio >= 0.33) {
    label = "dyskinesia";
    intensity = dyskinesiaRatio;
  } 
  else {
    label = "normal";
    intensity = 10.0;
  }

  intensity = constrain(intensity, 0.0, 1.2); // Cap for LED scaling
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
      // Serial.print(vData[i], 4);
      Serial.print(abscissa, 6);
      if(scaleType==SCL_FREQUENCY)
        Serial.print(":");
      // Serial.print("");
    }
    Serial.print(vData[i], 4);
    // Serial.print(abscissa, 6);
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
