/*********************************************************************
This code emulates a Avery Scales

Created by Jadon Miller, Miller Industrial Services - 2/15/2025
License: The Unlicense
*********************************************************************/

#include <Arduino.h>

void setup() {
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available() > 0) {
    char c = Serial.read();

    if (c == 'p') {
      Serial1.print("GROSS WT:     0.00 lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n");
    }
  }
}
