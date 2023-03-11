/*
  LcdProgressBarDouble.cpp - library v1.1.0 - 2017-02-10

  LcdProgressBarDouble is an Arduino library for displaying a 2 progress bars in a single row in LCD display,
  which is previously initialized. This library uses LiquidCrystal library for displaying.

  Copyright (c) 2016 Wilfried Loche.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  https://create.arduino.cc/projecthub/wloche/lcdprogressbardouble-707d5c
  https://github.com/wloche/LcdProgressBarDouble
*/
#include "Arduino.h"
#include "LcdProgressBarDouble.h"

#include <LiquidCrystal.h>

// -- character with one bar

#ifdef LCDPROGRESSBAR_USE_PROGMEM
	const byte LcdProgressBarDouble::bars[] PROGMEM =
	{
		   // 0
			B11000,
			B11000,
			B11000,
			B00000,
			B00000,
			B00000,
			B00000,
			B00000
		,
		   // 1
			B00000,
			B00000,
			B00000,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		,
		   // 2
			B11000,
			B11000,
			B11000,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		,
		   // 3
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B00000,
			B00000,
			B00000
		,
			// 4
			B00000,
			B00000,
			B00000,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
		,
			// 5
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		,
			// 6
			B11000,
			B11000,
			B11000,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
		,
			// 7
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
	};
#else
	byte LcdProgressBarDouble::bars[8][8] =
	{
		{   // 0
		    B11000,
		    B11000,
		    B11000,
		    B00000,
		    B00000,
		    B00000,
		    B00000,
		    B00000
		},
		{   // 1
			B00000,
			B00000,
			B00000,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		},
		{   // 2
			B11000,
			B11000,
			B11000,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		},
		{   // 3
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B00000,
			B00000,
			B00000
		},
		{	// 4
			B00000,
			B00000,
			B00000,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
		},
		{	// 5
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B11000,
			B11000,
			B00000
		},
		{	// 6
			B11000,
			B11000,
			B11000,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
		},
		{	// 7
			B11111,
			B11111,
			B11111,
			B00000,
			B00000,
			B11111,
			B11111,
			B00000
		}
	};
#endif

#ifdef LCDPROGRESSBAR_USE_PROGMEM
	const byte LcdProgressBarDouble::intToBars[] PROGMEM =
	{
		/*  0 */ 16,
		/*  1 */ 16,
		/*  2 */ 1,
		/*  3 */ 4,
		/*  4 */ 16,
		/*  5 */ 16,
		/*  6 */ 16,
		/*  7 */ 16,
		/*  8 */ 0,
		/*  9 */ 16,
		/* 10 */ 2,
		/* 11 */ 6,
		/* 12 */ 3,
		/* 13 */ 16,
		/* 14 */ 5,
		/* 15 */ 7
	};
#else
	byte LcdProgressBarDouble::intToBars[16] = {
		/*  0 */ 16,
		/*  1 */ 16,
		/*  2 */ 1,
		/*  3 */ 4,
		/*  4 */ 16,
		/*  5 */ 16,
		/*  6 */ 16,
		/*  7 */ 16,
		/*  8 */ 0,
		/*  9 */ 16,
		/* 10 */ 2,
		/* 11 */ 6,
		/* 12 */ 3,
		/* 13 */ 16,
		/* 14 */ 5,
		/* 15 */ 7
	};
#endif

LcdProgressBarDouble::LcdProgressBarDouble(LiquidCrystal* lcd, int row, int numCols)
{
  _lcd             = lcd;
  _numCols         = numCols;
  _computedNumCols = numCols * 2;
  _row             = row;

#ifdef LCDPROGRESSBAR_USE_PROGMEM
  byte tmpBar[8];
#endif

  //--- creating characters
  for (byte i = 0; i < 8; ++i) {

#ifdef LCDPROGRESSBAR_USE_PROGMEM
	  for (byte j = 0; j < 8; ++j) {
		  tmpBar[j] = pgm_read_byte_near(bars + i*8 + j);
	  }
	  _lcd->createChar(i, tmpBar);
#else
	  _lcd->createChar(i, bars[i]);
#endif

  }
}



void LcdProgressBarDouble::setMinValues(unsigned long minValue1, unsigned long minValue2)
{
  _minValues[0] = minValue1;
  _minValues[1] = (minValue2 == 0) ? minValue1 : minValue2;
}

void LcdProgressBarDouble::setMinValue1(unsigned long minValue1)
{
  _minValues[0] = minValue1;
}

void LcdProgressBarDouble::setMinValue2(unsigned long minValue2)
{
  _minValues[1] = minValue2;
}



void LcdProgressBarDouble::setMaxValues(unsigned long maxValue1, unsigned long maxValue2)
{
  setMaxValue1(maxValue1);
  setMaxValue2((maxValue2 == 0) ? maxValue1 : maxValue2);
}

void LcdProgressBarDouble::setMaxValue1(unsigned long maxValue1)
{
  // Allow max = min to disable the progress bar
  if (maxValue1 < _minValues[0]) {
    #ifdef LCDPROGRESSBAR_DEBUG
      Serial.println("*LcdProgressBarDouble::setMaxValue() error: maxValue1 <= _minValues1");
      Serial.print("minValues1="); Serial.println(_minValues[0]);
      Serial.print("maxValue1="); Serial.println(maxValue1);
      delay(100);
    #endif
    exit(100);
  }
  _maxValues[0]         = maxValue1;
  _computedMaxValues[0] = maxValue1 - _minValues[0];

  #ifdef LCDPROGRESSBAR_DEBUG
    Serial.print("_minValues1="); Serial.print(_minValues[0]);
    Serial.print(", _maxValues1="); Serial.println(_maxValues[0]);
  #endif
}

void LcdProgressBarDouble::setMaxValue2(unsigned long maxValue2)
{
  // Allow max = min to disable the progress bar
  if (maxValue2 < _minValues[1]) {
    #ifdef LCDPROGRESSBAR_DEBUG
      Serial.println("*LcdProgressBarDouble::setMaxValue() error: maxValue2 <= _minValues2");
      Serial.print("minValues2="); Serial.println(_minValues[1]);
      Serial.print("maxValue2="); Serial.println(maxValue2);
      delay(100);
    #endif
    exit(100);
  }
  _maxValues[1] 		= maxValue2;
  _computedMaxValues[1] = maxValue2 - _minValues[1];

  #ifdef LCDPROGRESSBAR_DEBUG
    Serial.print("_minValues2="); Serial.print(_minValues[1]);
    Serial.print(", _maxValues2="); Serial.println(_maxValues[1]);
  #endif
}


void LcdProgressBarDouble::setRangeValue1(unsigned long minValue1, unsigned long maxValue1)
{
  setMinValue1(minValue1);
  setMaxValue1(maxValue1);
}

void LcdProgressBarDouble::setRangeValue2(unsigned long minValue2, unsigned long maxValue2)
{
  setMinValue2(minValue2);
  setMaxValue2(maxValue2);
}


void LcdProgressBarDouble::disableBar1()
{
	setRangeValue1(0, 0);
}
void LcdProgressBarDouble::disableBar2()
{
	setRangeValue2(0, 0);
}


void LcdProgressBarDouble::drawValues(unsigned long value1, unsigned long value2)
{
  draw(value1, value2);
}

void LcdProgressBarDouble::draw(unsigned long value1, unsigned long value2)
{
  value2 = (value2 == 0) ? value1 : value2;

  #ifdef LCDPROGRESSBAR_DEBUG
    Serial.print("#1# value="); Serial.print(value1);
    Serial.print(", _minValues="); Serial.print(_minValues[0]);
    Serial.print(", _maxValues="); Serial.println(_maxValues[0]);

    Serial.print("#2# value="); Serial.print(value2);
    Serial.print(", _minValues="); Serial.print(_minValues[1]);
    Serial.print(", _maxValues="); Serial.println(_maxValues[1]);
  #endif

  byte progressPos[2] = {0, 0};

  if (_minValues[0] == _maxValues[0]) {
	//--- Bar1 not initiated: no display
	progressPos[0] = 0;
  } else if (value1 > _minValues[0]) {
	//--- Progress started
    if (value1 >= _maxValues[0]) {
	  //--- Maximum reached
      progressPos[0] = _computedNumCols;
    } else {
	  //--- Progress in between
      progressPos[0] = round((float)(value1 - _minValues[0]) / (float)(_computedMaxValues[0]) * _computedNumCols);
    }
  }

  if (_minValues[1] == _maxValues[1]) {
	//--- Bar2 not initiated: no display
	progressPos[1] = 0;
  } else if (value2 > _minValues[1]) {
	//--- Progress started
    if (value2 >= _maxValues[1]) {
	  //--- Maximum reached
      progressPos[1] = _computedNumCols;
    } else {
	  //--- Progress in between
      progressPos[1] = round((float)(value2 - _minValues[1]) / (float)(_computedMaxValues[1]) * _computedNumCols);
    }
  }

  if (_previousProgressPos[0] != progressPos[0] || _previousProgressPos[1] != progressPos[1]) {
    #ifdef LCDPROGRESSBAR_DEBUG
      Serial.print("> progressPos1=");    Serial.print(progressPos[0]);
      Serial.print(" > progressPos2="); Serial.println(progressPos[1]);
    #endif

    _previousProgressPos[0] = progressPos[0];
    _previousProgressPos[1] = progressPos[1];

    boolean bits[4];
    for (int i = 0; i < _computedNumCols; ++i) {
	  if (i % 2 == 0) {
		//--- Store information
		bits[0] = (progressPos[0] > i);
		bits[2] = (progressPos[1] > i);
	  } else {
		//--- Display
		bits[1] = (progressPos[0] > i);
		bits[3] = (progressPos[1] > i);

		int mask = 0;
        int tmp;
		for (int i = 0; i < 4; ++i) {
		    tmp = bits[i];
		    mask |= tmp << (3 - i);
        }

        #ifdef LCDPROGRESSBAR_DEBUG
          char m[20];
          sprintf(m, "%x", mask);
          Serial.print("#"); Serial.print(i); Serial.print("# Mask:"); Serial.print(mask); Serial.print(", "); Serial.println(m);
        #endif

        _lcd->setCursor(i / 2, _row);
        if (mask == 0) {
	      _lcd->print(' ');
	    } else {
			#ifdef LCDPROGRESSBAR_USE_PROGMEM
				_lcd->write(pgm_read_byte_near(intToBars + mask));
			#else
				_lcd->write(intToBars[mask]);
			#endif
	    }
	  }
    }
    delay(10);
  }
}
