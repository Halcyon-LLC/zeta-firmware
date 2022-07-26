#include "Arduino.h"
#include "Mux.h"
using namespace admux;

#define ENABLE_MUX_PIN 0
#define DEMUX_SEL0 23
#define DEMUX_SEL1 25
#define DEMUX_SEL2 27
#define DEMUX_SEL3 29
#define MUX_SEL0 47
#define MUX_SEL1 49
#define MUX_SEL2 51
#define MUX_SEL3 53
#define ANALOG_READ_PIN A15

const int NUM_GND = 4;
const int NUM_PWR = 8;

Mux power(Pinset(MUX_SEL0, MUX_SEL1, MUX_SEL2, MUX_SEL3));
Mux ground(Pinset(DEMUX_SEL0, DEMUX_SEL1, DEMUX_SEL2, DEMUX_SEL3));

void setup() {
  Serial.begin(9600);
  // Serial.begin(115200);
  pinMode(ENABLE_MUX_PIN, OUTPUT);

  pinMode(MUX_SEL0, OUTPUT);
  pinMode(MUX_SEL1, OUTPUT);
  pinMode(MUX_SEL2, OUTPUT);
  pinMode(MUX_SEL3, OUTPUT);
  pinMode(DEMUX_SEL0, OUTPUT);
  pinMode(DEMUX_SEL1, OUTPUT);
  pinMode(DEMUX_SEL2, OUTPUT);
  pinMode(DEMUX_SEL3, OUTPUT);

  // digitalWrite(MUX_SEL1, LOW);
  // digitalWrite(MUX_SEL2, LOW);
  digitalWrite(MUX_SEL3, LOW);
  // digitalWrite(DEMUX_SEL1, LOW);
  digitalWrite(DEMUX_SEL2, LOW);
  digitalWrite(DEMUX_SEL3, LOW);
}

void loop() {
  int readings[NUM_GND * NUM_PWR] = {};
  digitalWrite(ENABLE_MUX_PIN, LOW);
  digitalWrite(DEMUX_SEL0, LOW);
  digitalWrite(DEMUX_SEL1, LOW);
  digitalWrite(DEMUX_SEL2, LOW);
  digitalWrite(MUX_SEL0, LOW);
  digitalWrite(MUX_SEL1, LOW);

  // Iterate through all DEMUX channels
  int pointIndex = 0;
  for (int row = 0; row < NUM_GND; row++) {
    // Select the row and pass voltage
    ground.write(HIGH, row);

    for (int col = 0; col < NUM_PWR; col++) {
      // Read each spot and store it
      power.write(HIGH, col);
      pointIndex = (row * NUM_PWR) + col;
      readings[pointIndex] = analogRead(ANALOG_READ_PIN);
    }
  }

  String dataSent = "";
  for (int i = 0; i < (NUM_GND * NUM_PWR); i++) {
    char delimiter = (i + 1) < (NUM_GND * NUM_PWR) ? ',' : '\n';
    dataSent = dataSent + String(readings[i]) + delimiter;
  }
  Serial.print(dataSent);

  delay(200);
}
