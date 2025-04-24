#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();   // initialize the Circuit Playground library
  Serial.begin(9600);
}
 
void loop() {
   float movementX, movementY, movementZ, movement;
   uint16_t sound;
   if(CircuitPlayground.slideSwitch()) {   // sense & play when slide whitch at "+"
      movementX = abs(CircuitPlayground.motionX());  // read the X motion (absolute value)
      movementY = abs(CircuitPlayground.motionY());  // read the Y motion (absolute value)
      movementZ = abs(CircuitPlayground.motionZ());  // read the Z motion (absolute value)
      movement = movementX + movementY + movementZ;  // aggregate the movement sensed
      Serial.println(movement);

      if (movement > 10.0) {
         sound = (int) map(movement, 10.0, 60.0, 440.0, 1760.0);
         CircuitPlayground.playTone(sound, 500);
       }
   }
}