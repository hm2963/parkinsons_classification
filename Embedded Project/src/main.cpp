#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"


double findDominantFrequency(double* vReal);

void detect_tremor(String &label, float &intensity) {
  label = "mild";
  intensity = random(1, 100) / 100.0;
}

// Gravity estimation
float gravityX = 0.0, gravityY = 0.0, gravityZ = 0.0;
float alpha = 0.8; // smoothing factor for low-pass filter

double X, Y, Z;

const uint16_t samples = 64; // Must be a power of 2
const double samplingFrequency = 20; // Hz

// Arrays for FFT
double vRealx[samples], vImagx[samples];
double vRealy[samples], vImagy[samples];
double vRealz[samples], vImagz[samples];

// FFT objects
ArduinoFFT<double> FFTx = ArduinoFFT<double>(vRealx, vImagx, samples, samplingFrequency);
ArduinoFFT<double> FFTy = ArduinoFFT<double>(vRealy, vImagy, samples, samplingFrequency);
ArduinoFFT<double> FFTz = ArduinoFFT<double>(vRealz, vImagz, samples, samplingFrequency);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
  CircuitPlayground.begin();
}

double getMotionX() {
  X = CircuitPlayground.motionX();
  gravityX = alpha * gravityX + (1 - alpha) * X;
  return X - gravityX;
}

double getMotionY() {
  Y = CircuitPlayground.motionY();
  gravityY = alpha * gravityY + (1 - alpha) * Y;
  return Y - gravityY;
}

double getMotionZ() {
  Z = CircuitPlayground.motionZ();
  gravityZ = alpha * gravityZ + (1 - alpha) * Z;
  return Z - gravityZ;
}

void loop() {
  // Collect samples
  for (uint16_t i = 0; i < samples; i++) {
    vRealx[i] = getMotionX();
    vImagx[i] = 0.0;
    vRealy[i] = getMotionY();
    vImagy[i] = 0.0;
    vRealz[i] = getMotionZ();
    vImagz[i] = 0.0;
    delay(1000.0 / samplingFrequency);
  }

  // Process FFT for X
  FFTx.dcRemoval();
  FFTx.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFTx.compute(FFTDirection::Forward);
  FFTx.complexToMagnitude();

  // Process FFT for Y
  FFTy.dcRemoval();
  FFTy.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFTy.compute(FFTDirection::Forward);
  FFTy.complexToMagnitude();

  // Process FFT for Z
  FFTz.dcRemoval();
  FFTz.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFTz.compute(FFTDirection::Forward);
  FFTz.complexToMagnitude();

  // Find dominant frequency for Z (you can also do for X and Y if you want)
  double dominantFreqZ = findDominantFrequency(vRealz);

  Serial.print("Dominant Frequency (Z-axis): ");
  Serial.print(dominantFreqZ, 2);
  Serial.println(" Hz");

  double dominantFreqX = findDominantFrequency(vRealx);

  Serial.print("Dominant Frequency (X-axis): ");
  Serial.print(dominantFreqX, 2);
  Serial.println(" Hz");

  double dominantFreqY = findDominantFrequency(vRealy);

  Serial.print("Dominant Frequency (Y-axis): ");
  Serial.print(dominantFreqY, 2);
  Serial.println(" Hz");
}

double findDominantFrequency(double *vReal) {
  double maxMagnitude = 0;
  uint16_t maxIndex = 0;
  for (uint16_t i = 1; i < (samples / 2); i++) { // skip DC component
    if (vReal[i] > maxMagnitude) {
      maxMagnitude = vReal[i];
      maxIndex = i;
    }
  }
  return (maxIndex * 1.0 * samplingFrequency) / samples;
}


// #include <Arduino.h>
// #include "tremor_detection/tremor.h"
// #include "led_logic/led.h"
// #include "sound_logic/sound.h"


// void setup() {
//   initLED();  // put init function from your feature
//   initSound(); // put init function from your feature
//   acc_setup();
// }

// void loop() {
//   String type;
//   float intensity;

//   getdata();

//   detect_tremor(type, intensity);  // modifies type & intensity by reference
//   Serial.println("Tremor: " + type + ", intensity: " + String(intensity));

//   if(type == "normal"){
//     handleLEDN(intensity);
//     handleSoundN(intensity);
//   } 

//   else if(type == "tremor"){
//     handleLEDTM(intensity);
//     handleSoundTM(intensity);
//   } 
  
//   else if (type == "dyskinesia") {
//     handleLEDDK(intensity);
//     handleSoundDK(intensity);
//   }

//   //default value, meaning didnt change variable or undefined
//   else { 
//     // must have logic for if detection did not happen
//   }

//   delay(1000);
// }