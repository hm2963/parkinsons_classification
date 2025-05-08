#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"
#include <math.h> 

// Accelerometer usage Setup
void acc_setup() {
  CircuitPlayground.begin(); 
}

//grab values in all axes
double x,y,z;
double getMagnitude(double &x, double &y, double &z){
  x = CircuitPlayground.motionX();
  y = CircuitPlayground.motionY();
  z = CircuitPlayground.motionZ();

  return sqrt(x * x + y * y + z * z); //find the magnitude
}

// ---- FFT Configuration ----
const uint16_t samples = 64;               // Sweet spot with enough resolution
const double samplingFrequency = 21.33;    // 64 samples / 3 seconds

double vReal[samples];
double vImag[samples];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

// Collect Magnitude Data from Accelerometer
void collectFFTData() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = getMagnitude(x,y,z);
    vImag[i] = 0.0;
    delay(1000.0 / samplingFrequency);
  }
}

// Detect Tremor using FFT
void detect_tremor(String &label, float &intensity) {
  collectFFTData(); //grab data from accelerometer

  FFT.dcRemoval(); //remove gravity
  FFT.windowing(FFTWindow::Hann, FFTDirection::Forward);	//chose to use Hann Windowing as it yielded better results
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  applyMovingAverage(vReal, samples >> 1, 2); //apply moving average algo to each data

  // Sum magnitudes in tremor and dyskinesia frequency ranges
  double binWidth = (samplingFrequency / samples); // ~0.333 Hz per bin
  double tremorSum = 0.0;
  double dyskinesiaSum = 0.0;
  double totalEnergy = 0.0;

  const double MIN_TOTAL_ENERGY = 50;  //cumulative total energy must be higher than this to be considered

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
  }

  // calculate the ratios, will be used for conditions
  double tremorRatio = tremorSum / totalEnergy;
  double dyskinesiaRatio = dyskinesiaSum / totalEnergy;

  // set label and intensity fo appropriate values
  if (totalEnergy < MIN_TOTAL_ENERGY) {
    label = "normal";
    intensity = 0.1;
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
    intensity = 0.1;
  }

  intensity = constrain(intensity, 0.0, 1.2); // Cap for LED scaling
}

// moving average algorithm to smooth out the data
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

  // Copy the smoothed values
  for (uint16_t i = 0; i < bufferSize; i++) {
    vData[i] = temp[i];
  }
}
