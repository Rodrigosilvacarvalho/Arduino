/*
  DoubleBarPot.ino - v1.0.2 - 2016-08-01

  Showcase of LcdProgressBarDouble library: displays a 2 progress bars in a single row in LCD display (from 2 trimmer potentiometers),
  which is previously initialized. This library uses LiquidCrystal library for displaying.

  Copyright (c) 2016 Wilfried Loche.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/
#include <LiquidCrystal.h>
#include <LcdProgressBarDouble.h>

byte lcdNumCols = 16; // -- number of columns in the LCD

const int pinR4 = A4;
const int pinR5 = A5;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LcdProgressBarDouble lpg(&lcd, 1, lcdNumCols);

void setup()
{
  //-- Only useful for debugging purpose
  Serial.begin(115200);

  //-- initializing the LCD
  lcd.begin(2, lcdNumCols);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LcdProgressBar*2");

  //-- Set min and max values
  //*** (0 is default minimum value)
  //*** (1023 is maximum value for both bars)
  lpg.setMaxValues(1023);
      
  delay(50);
}

void loop()
{
  int r4 = analogRead(pinR4);
  int r5 = analogRead(pinR5);
  
  //-- draw progress bars
  lpg.draw(r4, r5);

  // -- do some delay: frequent draw may cause broken visualization
  delay(50);
}

