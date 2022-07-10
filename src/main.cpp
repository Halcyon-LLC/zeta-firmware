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

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(BAUD_RATE);
}

void loop() {
  digitalWrite(2, LOW);
  delay(1);
  fsr_reading0 = analogRead(FSR_PIN0);
  digitalWrite(2, HIGH);

  vo0 = map(fsr_reading0, 0, 1023, 0, 5000);
  Serial.println(vo0);
  delay(999);
}
