#include "Arduino.h"
#include "Mux.h"
using namespace admux;

#define ENABLE 30
#define IN_S0 52
#define IN_S1 50
#define IN_S2 22
#define IN_S3 26
#define OUT_S0 46
#define OUT_S1 48
#define OUT_S2 28
#define OUT_S3 24

#define VOUT_M1 A0
#define VIN_M1 A1

const int NUM_GND = 16;
const int NUM_PWR = 16;

Mux power(Pinset(IN_S0, IN_S1, IN_S2, IN_S3));
Mux ground(Pinset(OUT_S0, OUT_S1, OUT_S2, OUT_S3));

void setup() {
  Serial.begin(115200);

  pinMode(ENABLE, OUTPUT);
  pinMode(IN_S0, OUTPUT);
  pinMode(IN_S1, OUTPUT);
  pinMode(IN_S2, OUTPUT);
  pinMode(IN_S3, OUTPUT);
  pinMode(OUT_S0, OUTPUT);
  pinMode(OUT_S1, OUTPUT);
  pinMode(OUT_S2, OUTPUT);
  pinMode(OUT_S3, OUTPUT);
}

int get_Vin() {
  power.write(HIGH, 0);
  ground.write(HIGH, 0);

  return analogRead(VIN_M1);
}

void loop() {
  int vin = get_Vin();

  int readings[NUM_GND * NUM_PWR] = {};

  // Iterate through all DEMUX channels
  int pointIndex = 0;
  for (int row = 0; row < NUM_GND; row++) {
    // Select the row and pass voltage
    ground.write(HIGH, row);

    for (int col = 0; col < NUM_PWR; col++) {
      // Read each spot and store it
      power.write(HIGH, col);
      pointIndex = (row * NUM_PWR) + col;
      readings[pointIndex] = analogRead(VOUT_M1);
    }
  }

  String dataSent = String(vin) + ',';
  for (int i = 0; i < (NUM_GND * NUM_PWR); i++) {
    char delimiter = (i + 1) < (NUM_GND * NUM_PWR) ? ',' : '\n';
    dataSent = dataSent + String(readings[i]) + delimiter;
  }
  Serial.print(dataSent);
}
