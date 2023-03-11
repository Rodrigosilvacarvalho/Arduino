/*
TMP102.cpp
Copywrite 2011-2015 - Zack Clobes (W0ZC), Custom Digital Services, LLC


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TMP102_h
#define TMP102_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class TMP102
{
  public:
    TMP102(); // base type

      char begin();
			// call pressure.begin() to initialize TMP102 before use
			// returns 1 if success, 0 if failure (bad component or I2C bus shorted?)
		
      char getTemperature(double &T);
			// return temperature measurement
			// places returned value in T variable (deg C)
			// returns 1 for success, 0 for fail


  private:	
    unsigned long _milliTimeout;
};

#define TMP102_ADDR 0x48 // 7-bit address
#define TMP102_TIMEOUT 250

#endif
