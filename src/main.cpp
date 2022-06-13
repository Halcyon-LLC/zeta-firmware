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
double vo0;
float vo1;
float vo2;
float vo3;
float vo4;
float fsr0;
float fsr1;
float fsr2;
float fsr3;
float fsr4;
float force;

void setup() { Serial.begin(BAUD_RATE); }

void loop() {
  fsr_reading0 = analogRead(FSR_PIN0);
  // fsr_reading1 = analogRead(FSR_PIN1);
  // fsr_reading2 = analogRead(FSR_PIN2);
  // fsr_reading3 = analogRead(FSR_PIN3);
  // fsr_reading4 = analogRead(FSR_PIN4);

  vo0 = map(fsr_reading0, 0, 1023, 0, 5000);
  // vo1 = map(fsr_reading1, 0, 1023, 0, 5000);
  // vo2 = map(fsr_reading2, 0, 1023, 0, 5000);
  // vo3 = map(fsr_reading3, 0, 1023, 0, 5000);
  // vo4 = map(fsr_reading4, 0, 1023, 0, 5000);
  // fsr0 = (50000.0 / vo0) - 10.0;
  // fsr1 = (50000.0 / vo1) - 10.0;
  // fsr2 = (50000.0 / vo2) - 10.0;
  // fsr3 = (50000.0 / vo3) - 10.0;
  // fsr4 = (50000.0 / vo4) - 10.0;
  // force = (1 / fsr0) * 1000.0;

  // Serial.print(force);
  // Serial.print(" g    ");
  // Serial.print(vo0);
  // Serial.print(" , ");
  // Serial.print(vo1);
  // Serial.print(" , ");
  // Serial.print(vo2);
  // Serial.print(" , ");
  // Serial.print(vo3);
  // Serial.print(" , ");
  // Serial.print(vo4);
  // Serial.print("    ");

  // Serial.print(fsr0);
  // Serial.print(" , ");
  // Serial.print(fsr1);
  // Serial.print(" , ");
  // Serial.print(fsr2);
  // Serial.print(" , ");
  // Serial.print(fsr3);
  // Serial.print(" , ");
  // Serial.println(fsr4);
  Serial.println(vo0);

  // Serial.print("Force read: ");

  // Serial.println(" g");

  delay(100);
}
