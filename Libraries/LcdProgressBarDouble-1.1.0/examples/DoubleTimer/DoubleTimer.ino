/*
  DoubleTimer.ino - v1.0.2 - 2016-08-01

  Showcase of LcdProgressBarDouble library: displays a 2 progress bars in a single row in LCD display -based on time-
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

unsigned long duration1 = 20000; // 8000 milliseconds, 8 seconds
unsigned long duration2 = 10000; // 2000 milliseconds, 2 seconds
unsigned long startedMillis1  = 0;
unsigned long startedMillis2  = 0;

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
  startedMillis1 = millis();
  startedMillis2 = startedMillis1;

  //-- Set min and max values
  lpg.setMinValues(startedMillis1);
  lpg.setMaxValues(startedMillis1 + duration1, startedMillis2 + duration2);

  //-- Draw it
  lpg.draw(startedMillis1);
}

void loop() {

  unsigned long currentMillis = millis();

  //-- draw progress bar
  lpg.draw(currentMillis);

  if ((unsigned long)(currentMillis - startedMillis1) > duration1) {
    //--- Bargraph 1 duration's over: delay and start again!
    startedMillis1 += duration1;
    //lpg.setMinValue1(startedMillis1);
    //lpg.setMaxValue1(startedMillis1 + duration1);

    //--- Shortcut of 2 previous methods
    lpg.setRangeValue1(startedMillis1, startedMillis1 + duration1);
  }

  if ((unsigned long)(currentMillis - startedMillis2) > duration2) {
    //--- Bargraph 2 duration's over: delay and start again!
    startedMillis2 += duration2;
    
    //lpg.setMinValue2(startedMillis2);
    //lpg.setMaxValue2(startedMillis2 + duration2);

    //--- Shortcut of 2 previous methods
    lpg.setRangeValue2(startedMillis2, startedMillis2 + duration2);
  }

  // -- do some delay: frequent draw may cause broken visualization
  delay(100);
}

