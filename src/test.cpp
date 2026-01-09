/*********************************************************************
This code emulates a Avery Scales

Created by Jadon Miller, Miller Industrial Services - 2/15/2025
License: The Unlicense
*********************************************************************/

#include <Arduino.h>

bool avery = true;

void setup() {
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available() > 0) {
    char c = Serial1.read();

    if (c == 'p' && avery) {
      Serial1.print("GROSS WT:     5.16 lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n");
    }
    else if (c == 'S' && !avery) {
      Serial1.print("S S      5.16 kg\r\n");
    }
  }
}
