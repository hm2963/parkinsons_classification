#include "tremor.h"
#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"

void detect_tremor(String &label, float &intensity) {
  // Mock example, replace with sensor logic
  label = "mild";
  intensity = random(1, 100) / 100.0;
}



float X, Y, Z;

void acc_setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
  CircuitPlayground.begin();
}

float getdataX(){
  X = CircuitPlayground.motionX();
  Serial.print("X: ");
  Serial.print(X);
  return X;
}

float getdataY(){
  Y = CircuitPlayground.motionY();
  Serial.print("  Y: ");
  Serial.print(Y);
  return Y;
}

float getdataZ() {
  Z = CircuitPlayground.motionZ();
  Serial.print("  Z: ");
  Serial.println(Z);
  return Z;
}

const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
// const double signalFrequency = 1000;
const double samplingFrequency = 5000;
// const uint8_t amplitude = 100;

float vReal[samples];
float vImag[samples];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency);

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03


void FFTify(float P)
{
  /* Build raw data */
  // double ratio = twoPi * signalFrequency / samplingFrequency; // Fraction of a complete cycle stored at each sample (in radians)
  for (uint16_t i = 0; i < samples; i++)
  {
    vReal[i] = getdataX();
    // vReal[i] = int8_t(amplitude * sin(i) / 2.0);/* Build data with positive and negative values*/
    //vReal[i] = uint8_t((amplitude * (sin(i * ratio) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
  PrintVector(vReal, samples>>1, SCL_PLOT);
  double x = FFT.majorPeak();
  // while(1); /* Run Once */
  // delay(2000); /* Repeat after delay */
}

void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType)
{
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
      Serial.print(abscissa, 6);
      if(scaleType==SCL_FREQUENCY)
        Serial.print("Hz");
      Serial.print(" ");
    }
    Serial.println(vData[i], 4);
  }
  Serial.println();
}


