/*
  DoubleTimer.ino - v1.0.3 - 2016-08-02

  Showcase of LcdProgressBarDouble library: displays a 2 progress bars in a single row in LCD display,
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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LcdProgressBarDouble lpg(&lcd, 1, lcdNumCols);

unsigned long duration = 5000; // 5000 milliseconds, 5 seconds
unsigned long startedMillis  = 0;

//--- Bar 1 or 2 to show?
const byte PROGRESS_BAR = 2;

void setup() {
  //-- Only useful for debugging purpose
  Serial.begin(115200);

  //-- initializing the LCD
  lcd.begin(2, lcdNumCols);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LcdProgressBar*2");
      
  delay(100);

  //-- initializing the progress bar
  initLpg();
}

//-- initializing the progress bars
void initLpg()
{
  //-- start time
  startedMillis = millis();

  //-- Set min and max values of only one bar
  if (PROGRESS_BAR == 1) {
    lpg.setRangeValue1(startedMillis, startedMillis + duration);
  } else {
    lpg.setRangeValue2(startedMillis, startedMillis + duration);
  }

  //-- Draw it
  lpg.draw(startedMillis);
}

void loop() {

  unsigned long currentMillis = millis();

  //-- draw progress bar
  lpg.draw(currentMillis);

  if ((unsigned long)(currentMillis - startedMillis) > duration) {
    //--- Bargraph duration's over: delay and start again!
    startedMillis += duration;

    if (PROGRESS_BAR == 1) {
      lpg.setRangeValue1(startedMillis, startedMillis + duration);
    } else {
      lpg.setRangeValue2(startedMillis, startedMillis + duration);
    }
  }

  // -- do some delay: frequent draw may cause broken visualization
  delay(100);
}

