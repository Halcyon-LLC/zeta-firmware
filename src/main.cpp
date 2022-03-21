#include "Arduino.h"

#define FSR_PIN A7
#define BAUD_RATE 9600

int fsr_reading;
float vo;
float fsr;
float force;

void setup() { Serial.begin(BAUD_RATE); }

void loop() {
  fsr_reading = analogRead(FSR_PIN);
  // TODO: Fix mapping to reflect the logarithmic change
  vo = map(fsr_reading, 0, 1023, 0, 5000);
  fsr = (500000 - vo * 100000) / vo;
  force = (1 / fsr) * 1000.0;

  Serial.println(vo);
  // Serial.println(vo);

  // Serial.print("Force read: ");
  // Serial.print(force);
  // Serial.println(" g");

  delay(100);
}
