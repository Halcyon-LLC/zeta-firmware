#include "Arduino.h"

#define FSR_PIN0 A0
#define FSR_PIN1 A1
#define FSR_PIN2 A2
#define FSR_PIN3 A3
#define FSR_PIN4 A4
#define BAUD_RATE 9600

int fsr_reading0;
int fsr_reading1;
int fsr_reading2;
int fsr_reading3;
int fsr_reading4;
float vo0;
float vo1;
float vo2;
float vo3;
float vo4;
float fsr;
float force;

void setup() { Serial.begin(BAUD_RATE); }

void loop() {
  fsr_reading0 = analogRead(FSR_PIN0);
  fsr_reading1 = analogRead(FSR_PIN1);
  fsr_reading2 = analogRead(FSR_PIN2);
  fsr_reading3 = analogRead(FSR_PIN3);
  fsr_reading4 = analogRead(FSR_PIN4);

  vo0 = map(fsr_reading0, 0, 1023, 0, 5000);
  vo1 = map(fsr_reading1, 0, 1023, 0, 5000);
  vo2 = map(fsr_reading2, 0, 1023, 0, 5000);
  vo3 = map(fsr_reading3, 0, 1023, 0, 5000);
  vo4 = map(fsr_reading4, 0, 1023, 0, 5000);
  // fsr = (500000 - vo * 100000) / vo;
  // force = (1 / fsr) * 1000.0;

  Serial.print("{1 , 2 , 3 , 4 , 5} : { ");
  Serial.print(vo0);
  Serial.print(" , ");
  Serial.print(vo1);
  Serial.print(" , ");
  Serial.print(vo2);
  Serial.print(" , ");
  Serial.print(vo3);
  Serial.print(" , ");
  Serial.print(vo4);
  Serial.println(" , }");
  // Serial.println(vo);

  // Serial.print("Force read: ");
  // Serial.print(force);
  // Serial.println(" g");

  delay(100);
}
