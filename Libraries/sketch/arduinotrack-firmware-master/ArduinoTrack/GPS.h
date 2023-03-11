/*
GPS Data Parser for ArduinoTrack
Copywrite 2011-2015 - Zack Clobes (W0ZC), Custom Digital Services, LLC

This file is part of ArduinoTrack.

ArduinoTrack is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ArduinoTrack is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ArduinoTrack.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GPS_h
#define GPS_h

#include "Arduino.h"
#include <string.h>

#define _MAX_SENTENCE_LEN 88
#define _MAX_LATITUDE_LEN 12
#define _MAX_LONGITUDE_LEN 13
#define _METERS_TO_FEET 3.2808399


struct udtTime {
  int hh;
  int mm;
  int ss;
};

class GPS
{
  public:
    GPS(void);
    void clearInputBuffer(void);
    void addChar(char c);
    void getLatitude(char *sz);
    void getLongitude(char *sz);

   
    inline void getGPSTime(int *Hour, int *Minute, int *Second)
    {
    	if (_bGGAComplete || _bRMCComplete) {
    		*Hour = _currTime.hh;
    		*Minute = _currTime.mm;
    		*Second = _currTime.ss;    		
    	} else {
    		*Hour = 0;
    		*Minute = 0;
    		*Second = 0;
    	}
    }	
    
    inline int getGPSSeconds() {
      if (_bGGAComplete || _bRMCComplete) {
        return _currTime.ss;
      } else {
        return -100;      //return an invalid seconds value to show that there was a problem
      }
    }		

    inline void getGPSDate(char *sz) {
    	if (_bRMCComplete) strcpy(sz, _szGPSDate);
    	else sz[0] = '\0';
    }


    inline char LatitudeHemi() {
    	if (_bGGAComplete || _bRMCComplete) return _cLatitudeHemi;
    	else return ' ';
    }
    inline char LongitudeHemi() {
    	if (_bGGAComplete || _bRMCComplete) return _cLongitudeHemi;
    	else return ' ';
    }


    inline float Altitude() { 
    	if (_bGGAComplete) return _fAltitude;
    	else return 0;
    }
    inline float AltitudeInFeet() {
      if (_bGGAComplete) return (_fAltitude * _METERS_TO_FEET);
      else return 0;
    }
    inline float Knots() { 
    	if (_bRMCComplete) return _fKnots;
    	else return 0;
    } 
    inline float Course() { 
    	if (_bRMCComplete) return _fCourse;
    	else return 0;
    } 
        
    inline int FixQuality() { 
    	if (_bGGAComplete) return _iFixQuality;
    	else return 0;
    }
    inline int NumSats() { 
    	if (_bGGAComplete) return _iNumSats;
    	else return 0;
    }
    inline bool FixValid() { 
    	if (_bRMCComplete) return _bFixValid;
    	else return false;
    }    
    
    inline void ClearSentenceFlags() {
      _bGotNewRMC = false;
      _bGotNewGGA = false;
    }
    
    inline bool GotNewRMC() {
      return _bGotNewRMC;
    }
    
    inline bool GotNewGGA() {
      return _bGotNewGGA;
    }
    
    inline unsigned long LastDecodedMillis() {
      return _lastDecodedMillis;
    }
    
  

	private:
		void parseGGA();
		void parseRMC();
		bool validateGPSSentence(char *szGPSSentence, int iNumCommas, int iMinLength);
		void getString(char *ptrHaystack, char *ptrFound, int iMaxLength);
	  char* skipToNext(char *ptr);
	  
	  
		// properties
		char _szTemp[_MAX_SENTENCE_LEN];
		char _szLatitude[_MAX_LATITUDE_LEN];
		char _cLatitudeHemi;
		char _szLongitude[_MAX_LONGITUDE_LEN];
		char _cLongitudeHemi;
		int _iFixQuality;
		bool _bFixValid;
		int _iNumSats;
		float _fAltitude;
		float _fKnots;
		float _fCourse;
		char _szGPSDate[7];
		
		unsigned long _lastDecodedMillis;
		
		udtTime _currTime;
		
		
		int _iTempPtr;
		
		bool _bRMCComplete;
		bool _bGGAComplete;

		bool _bGotNewRMC;
		bool _bGotNewGGA; 
                
		bool _bFoundStart;
	};


#endif
