/*********************************************************************
This code emulates a Avery Scales

Created by Jadon Miller, Miller Industrial Services - 2/15/2025
License: The Unlicense
*********************************************************************/

#include <Arduino.h>

bool avery = true;

void setup()
{
  if (avery)
  {
    Serial1.begin(9600);
  }
  else
  {
    Serial1.begin(115200);
  }
}

void loop()
{
  if (Serial1.available() > 0)
  {
    char c = Serial1.read();

    float weight = random(0, 999) / 100.0; // Generate a random weight between 0 and 9.99 with 2 decimal places

    if (c == 'p' && avery)
    {
      Serial1.print("GROSS WT:     " + String(weight) + " lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n");
    }
    else if (c == 'S' && !avery)
    {
      Serial1.print("S S       " + String(weight) + " kg\r\n");
    }
  }
}
