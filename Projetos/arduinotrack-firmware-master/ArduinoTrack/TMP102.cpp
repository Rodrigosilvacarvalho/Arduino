/*
TMP102.cpp
Copywrite 2011-2015 - Zack Clobes (W0ZC), Custom Digital Services, LLC


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

Version History:
Version 1.0.0 - August 15, 2015 - Finalized the basic configuration and licensed GPS under the GPL3 license.
*/

#include "TMP102.h"
#include <Wire.h>


TMP102::TMP102()
// Base library type
{
}


char TMP102::begin()
// Initialize library for subsequent pressure measurements
{
  // Start up the Arduino's "wire" (I2C) library:
  Wire.begin();

  // Success!
  return(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char TMP102::getTemperature(double &T)
// Retrieve a temperature reading.
// Requires begin() to be called once prior to retrieve calibration parameters.
// T: external variable to hold result.
// Returns 1 if successful, 0 if I2C timeout.
{
  Wire.requestFrom(TMP102_ADDR, 2); 

  _milliTimeout = millis() + TMP102_TIMEOUT;    //wait up to 1 second for this data
  byte MSB = 0;
  byte LSB = 0;
  
  //get the first byte
  while ((millis() < _milliTimeout) && !Wire.available());    //wait for data to come in
  if (Wire.available()) {
    MSB = Wire.read();
  } else {
    return 0;
  }

  //get the second byte
  while ((millis() < _milliTimeout) && !Wire.available());    //wait for data to come in
  if (Wire.available()) {
    LSB = Wire.read();
  } else {
    return 0;
  }

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  T = TemperatureSum*0.0625;    //convert the raw data back to Celcius temp


  return(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
