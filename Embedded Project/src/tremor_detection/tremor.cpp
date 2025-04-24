#include "tremor.h"

void detect_tremor(String &label, float &intensity) {
  // Mock example, replace with sensor logic
  label = "mild";
  intensity = random(1, 100) / 100.0;
}

#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;

void acc_setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void getdata() {
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.println(Z);

  delay(1000);
}

