/*
TNC Data Collector for ArduinoTrack
Copywrite 2011-2015 - Zack Clobes (W0ZC), Custom Digital Services, LLC

This file is part of ArduinoTrack.

ArduinoTrack is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ArduinoTrack is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ArduinoTrack.  If not, see <http://www.gnu.org/licenses/>.

Version History:
Version 1.0.1 - February 14, 2015 - Fixed issue with errant null being transmitted, audio was moved from A1 to D3, in order to correctly utilize native PWM.
Version 1.0.0 - January 15, 2015 - Finalized the basic configuration and licensed GPS under the GPL3 license.

*/

#include "TNC.h"

TNC::TNC(void) {
  _iSZLen = -1;
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::initInternal(int pinPTT) {
  _pinPTT = pinPTT;    //keep track of the pin used for PTT
  _modulateInternally = true;


  _configTimers();
  _startTimer1ISR();  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::initKISS(int pinTx, int pinRx) {
  _modulateInternally = false;
  _pinTx = pinTx;
  _pinRx = pinRx;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitStart(char *szDest, char destSSID, char *szCall, char callSSID, char *szPath1, char path1SSID, char *szPath2, char path2SSID, bool bUsePath) {
  //Assembles the header of the transmitted packet
  byte i;


  if (!_modulateInternally) {
    //A couple of 0xC0's to get the modem's attention
    xmitChar(0xC0);
    xmitChar(0xC0);
  }
  
  //Add the destination address
  for (i=0; i<6; i++) {
    xmitChar(szDest[i] << 1);
  }
  xmitChar(destSSID << 1);
  
  //Add the from callsign
  for (i=0; i<6; i++) {
    xmitChar(szCall[i] << 1);
  }
  
  if ((szPath1[0] != ' ') && (bUsePath)) {
    //there's a path
    
    //don't end the callsign section
    xmitChar((callSSID << 1) | 0x00);    //DON'T flag the last bit with a 1 to indicate end of string

    //dump the Path1
    for (int j=0; j<6; j++) {
      xmitChar(szPath1[j] << 1);
    }
    if (szPath2[0] != ' ') {
      //there's a second path
      
      //don't end the Path1 section
      xmitChar((path1SSID << 1) | 0x00);    //DON'T flag the last bit with a 1 to indicate end of string
      
      for (int k=0; k<6; k++) {
        xmitChar(szPath2[k] << 1);
      }
      
      //Max of 2 paths, so always end here
      xmitChar((path2SSID << 1) | 0x01);    //flag the last bit with a 1 to indicate end of string
      
    } else {
      //This was the only path - end it
      xmitChar((path1SSID << 1) | 0x01);    //flag the last bit with a 1 to indicate end of string
    }
  } else {
    //this is the end of the callsign, there was no Paths
    xmitChar((callSSID << 1) | 0x01);    //flag the last bit with a 1 to indicate end of string
  }

  
  xmitChar(0x03);    //Control Byte
  xmitChar(0xF0);    //PID    
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitEnd(void) {

  if (_modulateInternally) {
    //we are using the internal modulator
  	
    //kick off the transmission
    _iTxState = 0;
    _iSZPos = 0;
    
    _iTxDelayRemaining = TX_DELAY_LEN;
  
    _CRC = 0xFFFF;    //init the CRC variable
    
    digitalWrite(_pinPTT, HIGH);    //push the PTT
    
    _startTimer1ISR();
  
    //wait for the state machine to get to a State 5, which is when it shuts down the transmitter.
    while (_iTxState != 5) {
      delay(100);    //just wait patiently until the packet is done
    }

  } else {
    //we are connecting to an external serial TNC
    xmitChar('\n');    //terminate the packet with a newline character
    

    Serial.print(F("Sending TNC: "));

    SoftwareSerial KISS(_pinTx, _pinRx);
    KISS.begin(9600);
    for (int i=0; i<=_iSZLen; i++) {
      KISS.print(_szXmit[i]);
      Serial.print("> ");
      Serial.println(_szXmit[i], HEX);
    } 
  }
  xmitFlush();		//re-init the SZ back to zero.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitString(char *sz) {
  while (*sz != 0) {
    xmitChar(*sz);
    sz++;
  }	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitChar(char c) {
  if (_iSZLen < (MAX_SZXMIT_SIZE - 1)) {
    //we still have room in the array
    _szXmit[++_iSZLen] = c;
    _szXmit[_iSZLen + 1] = '\0';    //null terminate
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitFloat(float f) {
  //prints a float to the TNC, with a single decimal point of resolution.  Sufficient for voltages and temperatures.
  
  if (f < 0) {
    xmitChar('-');    //negative sign
    f = f * -1;        //convert the negative number to positive for the rest of the processing
  }
  
  xmitLong((long)int(f), false);  //prints the int part
  xmitChar('.'); // print the decimal point
  int iDec = (f - int(f)) * 10;    //calculate the decimal point portion
  xmitLong((long)iDec, false) ; 
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitLong(long lNumToSend, boolean bLeadingZero) {
//This function writes a long numeric data type to the packet, zero padded to 6 digits (for the /A= Altitude report
  int iCnt = 0;
  
  iCnt = 0;
  while (lNumToSend >= 100000) {
    lNumToSend -= 100000;
    iCnt++;
  }
  if (iCnt == 0) {
    if (bLeadingZero == true) {
      xmitChar('0');
    }
  } else {
    xmitChar(iCnt + 48);
  }
  
  iCnt = 0;
  while (lNumToSend >= 10000) {
    lNumToSend -= 10000;
    iCnt++;
  }

  if (iCnt == 0) {
    if (bLeadingZero == true) {
      xmitChar('0');
    }
  } else {
    xmitChar(iCnt + 48);
  }
  
  iCnt = 0;
  while (lNumToSend >= 1000) {
    lNumToSend -= 1000;
    iCnt++;
  }
  if (iCnt == 0) {
    if (bLeadingZero == true) {
      xmitChar('0');
    }
  } else {
    xmitChar(iCnt + 48);
  }
  
  iCnt = 0;
  while (lNumToSend >= 100) {
    lNumToSend -= 100;
    iCnt++;
  }
  if (iCnt == 0) {
    if (bLeadingZero == true) {
      xmitChar('0');
    }
  } else {
    xmitChar(iCnt + 48);
  }
  
  iCnt = 0;
  while (lNumToSend >= 10) {
    lNumToSend -= 10;
    iCnt++;
  }
  if (iCnt == 0) {
    if (bLeadingZero == true) {
      xmitChar('0');
    }
  } else {
    xmitChar(iCnt + 48);
  }
  
  xmitChar(lNumToSend + 48);        //always send the ones digit
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::_calcCRC(byte iBit) {
  unsigned int xor_int;
  
  //iBit = iBit & 0x01;    //strip of all but LSB
  xor_int = _CRC ^ iBit;				// XOR lsb of CRC with the latest bit
  _CRC >>= 1;									// Shift 16-bit CRC one bit to the right

  if (xor_int & 0x0001) {					// If XOR result from above has lsb set
    _CRC ^= 0x8408;							// Shift 16-bit CRC one bit to the right
  }
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean TNC::getNextBit(void) {
  static int iRotatePos = 0;
  boolean bOut = 0;
  

  switch (_iTxState) {
  case 0:
    //TX Delay transmitting flags
    _bNoStuffing = true;
    bOut = ((0x7e & (1 << iRotatePos)) != 0);    //get the bit
  
    
    if (iRotatePos != 7) iRotatePos++;
    else {
      //rotate around for the next bit

      iRotatePos = 0;
     
      if (_iTxDelayRemaining > 0) {
        //we have more txdelays
        _iTxDelayRemaining--;
      } else {
        _iTxState = 1;    //we need to drop down to the next state
      }
    }
    break;
  case 1:
    //normal packet payload
    _bNoStuffing = false;
    bOut = ((_szXmit[_iSZPos] & (1 << iRotatePos)) != 0);    //get the bit
  
    if (iRotatePos != 7) iRotatePos++;
    else {
      //we need to get the next byte if possible

      iRotatePos = 0;
      
      if (_iSZPos < _iSZLen) {
        //we have more bytes
        _iSZPos++;
      } else {
        _iTxState = 2;    //drop down and transmit the CRC bytes
      }
    }
    
    
    //calculate the CRC for the data portion of the packet
    _calcCRC(bOut);
    
    break;  
  case 2:  
    //Transmit the CRC Check bits (first byte)
  
    bOut = (((lowByte(_CRC) ^ 0xff) & (1 << iRotatePos)) != 0);    //get the bit
  
    if (iRotatePos != 7) iRotatePos++;
    else {
      iRotatePos = 0;
      _iTxState = 3;    //we need to drop down to the next state
    }  
    break;
  
  case 3:
    //Transmit the CRC Check bits (second byte)
    bOut = (((highByte(_CRC) ^ 0xff) & (1 << iRotatePos)) != 0);    //get the bit
  
    if (iRotatePos != 7) iRotatePos++;
    else {
      iRotatePos = 0;
      _iTxState = 4;    //we need to drop down to the next state
    }  
    break;

  case 4:  
    //send end-of-packet flag
  
    _bNoStuffing = true;
    bOut = ((0x7e & (1 << iRotatePos)) != 0);    //get the bit
  
    if (iRotatePos != 7) iRotatePos++;
    else {
      _iTxState = 5;    //we need to drop down to the next state
    } 
    break;


  case 5:
    //done - shut the transmitter down
    bOut = 1;    //bogus bit so we have something to return.  Should be a moot point, since the ISR is shutting down.
    iRotatePos = 0;    //reset to get ready for next byte
    _stopTimer1ISR();
//    analogWrite(PIN_AUDIO_OUT, 127);
    digitalWrite(_pinPTT, LOW);    //Unkey the transmitter
  
    break;
    
  default:
    bOut = 2;
  }

  return bOut;  
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean TNC::noBitStuffing(void) {
  return _bNoStuffing;  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TNC::xmitLen(void) {
	return _iSZLen;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::xmitFlush(void) {
	//flushes out the SZ that was about to be transmitted.  Called at the end of each transmission.
	
	_iSZLen = -1;      //reset back to a clean buffer   
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::_configTimers(void) {
  TCCR1A = 0x00;
  TCCR1B = 0x09;    //Set WGM12 high, and set CS=001 (which is clk/1);
  
  OCR1A = TIMER1_SEED;    //should be 76 according to math with clk/8, 608 if clk/1

  //Timer2 drives the PWM frequency.  We need it sufficiently higher than the 1200/2200hz tones
  TCCR2B = 0<<CS22 | 0<<CS21 | 1<<CS20;      //sets timer2 prescaler to clk - Approx 32kHz PWM freq.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::_startTimer1ISR(void){
  sbi(TIMSK1, OCIE1A);
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TNC::_stopTimer1ISR(void) {
  cbi(TIMSK1, OCIE1A);
}

