#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"
#include <math.h> 

#define SAMPLES 64
#define MAX_SIZE (SAMPLES >> 1)


#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

int peak_indices[MAX_SIZE]; 

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

  // (Optional) smoothing
  applyMovingAverage(vReal, MAX_SIZE, 1);
  PrintVector(vReal, samples>>1, SCL_FREQUENCY);

  // Find peaks directly from vReal[]
  int num_peaks = find_peaks(vReal, MAX_SIZE, peak_indices, 0.05, 4);

  // Print detected peaks
  for (int i = 0; i < num_peaks; i++) {
    int bin_index = peak_indices[i];
    double frequency = (bin_index * samplingFrequency) / samples;  // Frequency for the peak bin
  
    Serial.print("Peak at bin ");
    Serial.print(bin_index);
    Serial.print(" corresponds to frequency: ");
    Serial.print(frequency, 2);  // Print frequency with 2 decimal places
    Serial.print(" Hz, with magnitude: ");
    Serial.println(vReal[bin_index]);
  }
  


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

int find_peaks(const double* magnitudes, int size, int* peak_indices, 
  double threshold_ratio = 0.3, int min_distance = 1) {
  double max_val = 0.0;
  int peak_count = 0;

// Find max magnitude
  for (int i = 0; i < size; ++i) {
    if (magnitudes[i] > max_val) {
      max_val = magnitudes[i];
    }
  }

if (max_val == 0.0) return 0; // Avoid divide-by-zero

for (int i = 1; i < size - 1; ++i) {
  double prev = magnitudes[i - 1];
  double curr = magnitudes[i];
  double next = magnitudes[i + 1];
  double norm = curr / max_val;

  if (norm >= threshold_ratio && curr > prev && curr > next) {
    if (peak_count == 0 || (i - peak_indices[peak_count - 1]) >= min_distance) {
      peak_indices[peak_count++] = i;
    } else if (curr > magnitudes[peak_indices[peak_count - 1]]) {
      peak_indices[peak_count - 1] = i; // Replace weaker peak
    }
  }
}

return peak_count;
}




