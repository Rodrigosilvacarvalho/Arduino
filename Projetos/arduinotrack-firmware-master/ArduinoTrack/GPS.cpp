/*
GPS Data Parser for ArduinoTrack
Copywrite 2011-2015 - Zack Clobes (W0ZC), Custom Digital Services, LLC

This file is part of ArduinoTrack.

ArduinoTrack is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ArduinoTrack is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ArduinoTrack.  If not, see <http://www.gnu.org/licenses/>.

Version History:
Version 1.0.4 - October 18, 2015 - Found bug where GPS latitude and longitude were exceeding the buffer.  Corrected and cleaned up some old code.
Version 1.0.3 - October 7, 2015 - Eliminated excess Serial.print's.  Cleaned up old comments.
Version 1.0.2 - April 27, 2015 - Added the possibility to capture GNGGA and GNRMC strings (instead of just GP***).
Version 1.0.1 - February 27, 2015 - Added initialization to the private member variables.  Cleaned up some old comments.
Version 1.0.0 - January 15, 2015 - Finalized the basic configuration and licensed GPS under the GPL3 license.


*/

#include "GPS.h"





//Public Methods
GPS::GPS(void) {
  
  //Constructor - initialize the vars
  _szTemp[0] = 0;
	_iTempPtr = 0;
	_bFoundStart = false;
	_bRMCComplete = false;
	_bGGAComplete = false;  
 
	strcpy(_szLatitude, "1234.5678");
	strcpy(_szLongitude, "12345.6789");
	strcpy(_szGPSDate, "000000"); 
	_currTime.hh =_currTime.mm = _currTime.ss = 0;
	_cLatitudeHemi = 'N';
	_cLongitudeHemi = 'W';
	_iFixQuality = 0;
	_bFixValid = false;
	_iNumSats = 0;
	_fAltitude = 0.0;
	_fKnots = 0.0;
	_fCourse = 0.0;
 
  _lastDecodedMillis = millis();	
}

void GPS::clearInputBuffer(void) {
  //need to flush out the old data before importing new, or else you can wind up with odd combinations of old headers and new tails
  _szTemp[0] = 0;
  _iTempPtr = 0;
  _bFoundStart = false;
}
  
  

void GPS::addChar(char c) {
	//first make sure we still have room in the _szTemp for another char (and null termiation)
	if (_iTempPtr >= (_MAX_SENTENCE_LEN - 2)) {
		//we're full and we apparently didn't find an end of string - throw the szTemp away and lets start over
		_szTemp[0] = 0;
		_iTempPtr = 0;
		_bFoundStart = false;
	}

	if (_bFoundStart == false) {
		//we are getting a new character, but we don't have a valid start to a sentence yet - see if this is a $
		
		if (c == '$') {
			//we have a new sentence - start storing it to the temp var
			_iTempPtr = 0;
			_bFoundStart = true;
			
			_szTemp[_iTempPtr++] = c;
			_szTemp[_iTempPtr] = 0;			//always make sure our resulting string is null terminated
		}
		//this was a start, so it couldn't have been the finish - just return;
		
		return;
	} else {
		//we were already collecting a valid sentence - lets add it to the array
		
		//first make sure it's not the END of the sentence.
		if ((int)c <= 0x0d) {
			//this was probably a \r or \n, but could be some other sort of invalid (null or Control) char
			
			//look at the first few chars of the array to see if it's RMC or GGA.
			if (_szTemp[1] == 'G' && (_szTemp[2] == 'P' || _szTemp[2] == 'N') && _szTemp[3] == 'R' && _szTemp[4] == 'M' && _szTemp[5] == 'C') {
				//we have the start of an RMC string

                                Serial.println(_szTemp);
				_bRMCComplete = true;    //set a flag indicating that an RMC sentence has been received, therefore we have valid data

				parseRMC();

				_bGotNewRMC = true;      //set a temporary flag indicating that we got a new RMC sentence
				_lastDecodedMillis = millis();    //keep track of the time when we last received a sentence
			
			} else if (_szTemp[1] == 'G' && (_szTemp[2] == 'P' || _szTemp[2] == 'N') && _szTemp[3] == 'G' && _szTemp[4] == 'G' && _szTemp[5] == 'A') {
				//we have the start of an GGA string

                                Serial.println(_szTemp);				
				_bGGAComplete = true;
				parseGGA();
				_bGotNewGGA = true;      //set a temporary flag indicating that we got a new RMC sentence
				_lastDecodedMillis = millis();    //keep track of the time when we last received a sentence
			}
			
			//even if we didn't find an RMC or GGA string, we still made it to the end - throw out whatever is still in szTemp
			_szTemp[0] = 0;
			_iTempPtr = 0;
			_bFoundStart = false;
			
		} else {
			//this is just a "normal char".  Add it to the array and quit
			_szTemp[_iTempPtr++] = c;
			_szTemp[_iTempPtr] = 0;			//always make sure our resulting string is null terminated
			return;
		}
	}
}





//Private functions
void GPS::parseRMC() {
//This function parses the time and altitude from the GGA string that is stored in the variable being passed
//0         10        20        30        40        50        60        70
//01234567890123456789012345678901234567890123456789012345678901234567890
//$GPRMC,224831,A,3805.5827,N,09755.0740,W,000.0,000.0,240806,005.9,E*65

	char sz[9];			//temp var
	
	char* ptrTemp;
	
	ptrTemp = &_szTemp[7];			//set this pointer to the hours digit

	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.hh = atoi(sz);
	ptrTemp += 2;		//incr the pointer to minutes
	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.mm = atoi(sz);
	ptrTemp += 2;		//incre the pointer to seconds
	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.ss = atoi(sz);
	
	
	ptrTemp = skipToNext(ptrTemp);			//skip thru the rest of the chars in the time

	//see if we have valid fix (A) or invalid (V)
	getString(ptrTemp, sz, 1);
	if (sz[0] == 'A') {
		_bFixValid = true;
	} else {
		_bFixValid = false;
	}
	ptrTemp = skipToNext(ptrTemp);


	//get the Latitude
	getString(ptrTemp, _szLatitude, _MAX_LATITUDE_LEN);
  if (_szLatitude[0] == '\0') {
    //the longitude was empty
	  strcpy(_szLatitude, "0000.0000");
  }
	ptrTemp = skipToNext(ptrTemp);

	//get Latitude Hemisphere
	getString(ptrTemp, sz, 1);
	if (sz[0] == 'S') {
		_cLatitudeHemi = 'S';
	} else {
		_cLatitudeHemi = 'N';
	}
	ptrTemp = skipToNext(ptrTemp);


	//get the Longitude
	getString(ptrTemp, _szLongitude, _MAX_LONGITUDE_LEN);
  if (_szLongitude[0] == '\0') {
    //the longitude was empty
	  strcpy(_szLongitude, "00000.0000");
  }
	ptrTemp = skipToNext(ptrTemp);

	//get Latitude Hemisphere
	getString(ptrTemp, sz, 1);
	if (sz[0] == 'E') {
		_cLongitudeHemi = 'E';
	} else {
		_cLongitudeHemi = 'W';
	}
	ptrTemp = skipToNext(ptrTemp);

	//get speed in knots
	getString(ptrTemp, sz, 6);
	_fKnots = atof(sz);
	ptrTemp = skipToNext(ptrTemp);

	//get course of track
	getString(ptrTemp, sz, 6);
	_fCourse = atof(sz);
	ptrTemp = skipToNext(ptrTemp);	
	
	//get date
	getString(ptrTemp, _szGPSDate, 7);
	
	
}
void GPS::parseGGA() {
	//This function parses the time and altitude from the GGA string that is stored in the variable being passed
	//0         10        20        30        40        50        60        70
	//01234567890123456789012345678901234567890123456789012345678901234567890
	//$GPGGA,232440   ,3804.3322 ,N,09756.1222 ,W,6,03,2.7,485.1,M,-26.7,M,,*
	//$GNGGA,162315.00,3805.57830,N,09755.05519,W,1,03,1.86,577.4,M,-26.1,M,,*76
	char sz[9];			//temp var
	
	char* ptrTemp;
	
	ptrTemp = &_szTemp[7];			//set this pointer to the hours digit
	//Serial.println("Parsing GGA");

	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.hh = atoi(sz);
	ptrTemp += 2;		//incr the pointer to minutes
	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.mm = atoi(sz);
	ptrTemp += 2;		//incre the pointer to seconds
	
	strncpy(sz, ptrTemp, 2);
	sz[2] = 0;		//null terminate the string
	_currTime.ss = atoi(sz);
	
	ptrTemp = skipToNext(ptrTemp);			//skip thru the rest of the chars in the time

	//get the Latitude
	getString(ptrTemp, _szLatitude, _MAX_LATITUDE_LEN);
  if (_szLatitude[0] == '\0') {
    //the longitude was empty
    strcpy(_szLatitude, "0000.0000");
  }
	ptrTemp = skipToNext(ptrTemp);

	//get Latitude Hemisphere
	getString(ptrTemp, sz, 1);
	if (sz[0] == 'S') {
		_cLatitudeHemi = 'S';
	} else {
		_cLatitudeHemi = 'N';
	}
	ptrTemp = skipToNext(ptrTemp);


	//get the Longitude
	getString(ptrTemp, _szLongitude, _MAX_LONGITUDE_LEN);
  if (_szLongitude[0] == '\0') {
    //the longitude was empty
	  strcpy(_szLongitude, "00000.0000");
  }
	ptrTemp = skipToNext(ptrTemp);

	//get Latitude Hemisphere
	getString(ptrTemp, sz, 1);
	if (sz[0] == 'E') {
		_cLongitudeHemi = 'E';
	} else {
		_cLongitudeHemi = 'W';
	}
	ptrTemp = skipToNext(ptrTemp);

	//get position fix quality
	getString(ptrTemp, sz, 2);
	_iFixQuality = atoi(sz);
	ptrTemp = skipToNext(ptrTemp);
	
	//get number of sats received
	getString(ptrTemp, sz, 3);
	_iNumSats = atoi(sz);
	ptrTemp = skipToNext(ptrTemp);
	
	ptrTemp = skipToNext(ptrTemp);		//skip over horizontal dilution
	
	getString(ptrTemp, sz, 8);
	_fAltitude = atof(sz);
}


void GPS::getString(char *ptrHaystack, char *ptrFound, int iMaxLength) {
	//Extracts the string (up to iMaxLength) from the char*, and returns a pointer
	
	*ptrFound = '\0';			//be sure that the Found is null terminated even if we didn't find anything
	
	int i = 0;
	while (ptrHaystack[i] != '\0' && ptrHaystack[i] != ',' && i < (iMaxLength - 1)) {
		
		//this is just a char - copy it to the array
		ptrFound[i] = ptrHaystack[i];
		ptrFound[i+1] = '\0';		//null terminate the return		
		
		i++;
	}
}


bool GPS::validateGPSSentence(char *szGPSSentence, int iNumCommas, int iMinLength) {
	//checks to make sure we have a string that is null terminated, has the appropriate # of commas, and has valid chars within it
	// Pass it the string to test, and the number of commas that should be included for this type of string
	
	int iCommaCount = 0;
	int iCharCount = 0;
	
	while (szGPSSentence[iCharCount] != '\0' && iCharCount < _MAX_SENTENCE_LEN) {
		if (szGPSSentence[iCharCount] == ',') iCommaCount++;			//we found a comma, incre the counter
		
		iCharCount++;
	}
	if (iCharCount < iMinLength) return false;
	if (iNumCommas != iCommaCount) return false;
		
	//we passed all of the tests - return true
	return true;
	
}

char* GPS::skipToNext(char *ptr) {
	//takes a pointer, advances through until it either hits a null or gets past the next comma
	//ptr++;		//we always have to advance at least once
	while (*ptr != ',' && *ptr != '\0') {
		//just a char - incr to the next one
		ptr++;	
	}
	
	if (*ptr == ',') ptr++;			//if we landed on a comma, advance one
	return ptr;
}

void GPS::getLatitude(char *sz) {
  byte i = 0;
  sz[0] = 0x00;   //always make sure we return null if no valid data in the source

  if (_bGGAComplete || _bRMCComplete) {
    while(_szLatitude[i] > 0x00 && i < _MAX_LATITUDE_LEN) {
      sz[i] = _szLatitude[i];
      i++;
      sz[i] = 0x00;   //always null terminate the end
    }
  }
}

void GPS::getLongitude(char *sz) {
  byte i = 0;
  sz[0] = 0x00;   //always make sure we return null if no valid data in the source

  if (_bGGAComplete || _bRMCComplete) {
    while(_szLongitude[i] > 0x00 && i < _MAX_LONGITUDE_LEN) {
      sz[i] = _szLongitude[i];
      i++;
      sz[i] = 0x00;   //always null terminate the end
    }
  }
}

