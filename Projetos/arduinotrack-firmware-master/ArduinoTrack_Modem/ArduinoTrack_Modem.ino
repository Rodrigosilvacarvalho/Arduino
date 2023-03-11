/*
ArduinoTrack_Modem
Copywrite 2011-2016 - Zack Clobes (W0ZC), Custom Digital Services, LLC


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

ArduinoTrack is a trademark of Custom Digital Services, LLC.


Version History:
Version 3.0.0 - April 23, 2016 - No major changes to firmware, only fixing depreciated data type, and moving the install install instructions into readme.txt.  Bringing to version 3.0 for consistency with ArduinoTrack firmware.
Version 2.1.3 - May 23, 2015 - Adjusted the firmware programming instructions.
Version 2.1.0 - May 3, 2015 - Minor changes to code to bring compliance with PCB version 1.10.
Version 2.0.3 - March 29, 2015 - Simple change to instructions only to accomidate Arduino IDE 1.6.2.  Also changed high fuse to keep eeprom values from being reset during firmware flash.
Version 2.0.2 - February 27, 2015 - Altered the HighBit to prevent the erasure of eeprom data after firmware updates. Uploaded firmware to GitHub.
Version 2.0.1 - February 14, 2015 - Updated tnc.cpp to include additional debugging and fixed an issue with PWM audio on PIN_AUDIO_OUT.
Version 2.0.0 - January 15, 2015 - Integrated the common TNC library into the modem Sketch.  Licensed ArduinoTrack_Modem
  under the GPL3 license.
Version 1.9.0 - January 8, 2015 - Moved the arySineHigh and arySineLow into Flash ROM to conserve SRAM.


See www.projecttraveler.org for full construction and programming instructions.

*/

#define FIRMWARE_VERSION "3.0.0"



#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>

#include <SoftwareSerial.h>
#include "TNC.h"


#define PIN_HOST_RX 11    //opposite of how it's configured on the 'host' controller
#define PIN_HOST_TX 12



SoftwareSerial HostController(PIN_HOST_RX, PIN_HOST_TX);
TNC oTNC;




//------------------------------------------ Variables for the internal modulation ------------------------------------------
//Pin assignments
#define PIN_AUDIO_OUT 3
#define PIN_AUDIO_IN A0
#define PIN_PTT_OUT 2


#define BAUD_GENERATOR_COUNT 20
#define TONE_HIGH_STEPS_PER_TICK 6001
#define TONE_LOW_STEPS_PER_TICK 3273

//Sinewave lookup tables for high and low tones (the difference is the amplitude)
PROGMEM const unsigned char _arySineHigh[] = {128, 131, 134, 137, 140, 144, 147, 150, 153, 156,
	159, 162, 165, 168, 171, 174, 177, 179, 182, 185,
	188, 191, 193, 196, 199, 201, 204, 206, 209, 211,
	213, 216, 218, 220, 222, 224, 226, 228, 230, 232,
	234, 235, 237, 239, 240, 241, 243, 244, 245, 246,
	248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
	254, 255, 255, 255, 255, 255, 255, 255, 254, 254,
	254, 253, 253, 252, 251, 250, 250, 249, 248, 246,
	245, 244, 243, 241, 240, 239, 237, 235, 234, 232,
	230, 228, 226, 224, 222, 220, 218, 216, 213, 211,
	209, 206, 204, 201, 199, 196, 193, 191, 188, 185,
	182, 179, 177, 174, 171, 168, 165, 162, 159, 156,
	153, 150, 147, 144, 140, 137, 134, 131, 128, 125,
	122, 119, 116, 112, 109, 106, 103, 100, 97, 94,
	91, 88, 85, 82, 79, 77, 74, 71, 68, 65,
	63, 60, 57, 55, 52, 50, 47, 45, 43, 40,
	38, 36, 34, 32, 30, 28, 26, 24, 22, 21,
	19, 17, 16, 15, 13, 12, 11, 10, 8, 7,
	6, 6, 5, 4, 3, 3, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 2, 2, 3,
	3, 4, 5, 6, 6, 7, 8, 10, 11, 12,
	13, 15, 16, 17, 19, 21, 22, 24, 26, 28,
	30, 32, 34, 36, 38, 40, 43, 45, 47, 50,
	52, 55, 57, 60, 63, 65, 68, 71, 74, 77,
	79, 82, 85, 88, 91, 94, 97, 100, 103, 106,
	109, 112, 116, 119, 122, 125};


//Max 192 (3db down from High)
PROGMEM const unsigned char _arySineLow[] = {128, 129, 130, 132, 133, 134, 135, 136, 137, 139,
	140, 141, 142, 143, 144, 145, 146, 147, 149, 150,
	151, 152, 153, 154, 155, 156, 157, 158, 158, 159,
	160, 161, 162, 163, 164, 164, 165, 166, 167, 167,
	168, 169, 169, 170, 170, 171, 171, 172, 172, 173,
	173, 174, 174, 174, 175, 175, 175, 175, 175, 176,
	176, 176, 176, 176, 176, 176, 176, 176, 176, 176,
	175, 175, 175, 175, 175, 174, 174, 174, 173, 173,
	172, 172, 171, 171, 170, 170, 169, 169, 168, 167,
	167, 166, 165, 164, 164, 163, 162, 161, 160, 159,
	158, 158, 157, 156, 155, 154, 153, 152, 151, 150,
	149, 147, 146, 145, 144, 143, 142, 141, 140, 139,
	137, 136, 135, 134, 133, 132, 130, 129, 128, 127,
	126, 124, 123, 122, 121, 120, 119, 117, 116, 115,
	114, 113, 112, 111, 110, 109, 107, 106, 105, 104,
	103, 102, 101, 100, 99, 98, 98, 97, 96, 95,
	94, 93, 92, 92, 91, 90, 89, 89, 88, 87,
	87, 86, 86, 85, 85, 84, 84, 83, 83, 82,
	82, 82, 81, 81, 81, 81, 81, 80, 80, 80,
	80, 80, 80, 80, 80, 80, 80, 80, 81, 81,
	81, 81, 81, 82, 82, 82, 83, 83, 84, 84,
	85, 85, 86, 86, 87, 87, 88, 89, 89, 90,
	91, 92, 92, 93, 94, 95, 96, 97, 98, 98,
	99, 100, 101, 102, 103, 104, 105, 106, 107, 109,
	110, 111, 112, 113, 114, 115, 116, 117, 119, 120,
	121, 122, 123, 124, 126, 127};

//------------------------------------------ END Variables for the internal modulation ------------------------------------------



void setup() {

  HostController.begin(9600);
  Serial.begin(19200);      //used for debug output back to the dev environment



  pinMode(PIN_PTT_OUT, OUTPUT);
  pinMode(PIN_AUDIO_OUT, OUTPUT);

  oTNC.initInternal(PIN_PTT_OUT);      //Define this as an internally modulated signal.  The PINS are defined in TNC.h.

  Serial.print(F("Firmware Version: "));
  Serial.println(FIRMWARE_VERSION);

}

void loop() {

  byte iTemp;

  if (HostController.available() > 0) {
    //there's serial data to read in - grab it and add it to the array
    iTemp = HostController.read();

    //Dump the the raw data coming in from the host controller
    Serial.print("< ");
    Serial.println(iTemp, HEX);


    // if we find a null, CR, LF, or 0xC0 then we're going to assume that this is the end
    //
    // Note 0xC0 seems to come from standard KISS mode TNC's.  I forget what the significance is now...
    if (iTemp != 0x00 && iTemp != 0x0a && iTemp != 0x0d && iTemp != 0xc0) {
      //we got a character, but it wasn't the end-of-transmission char - just add it to the array
      oTNC.xmitChar(iTemp);    //The xmitChar method adds this character into SZ String array

    } else {
      //we have a end-of-tranmission character

      //make sure we have enough data to be worth transmitting
      if (oTNC.xmitLen() < 10) {
        Serial.println(F("Flushing incomplete packet"));
        oTNC.xmitFlush();			//this was an incomplete packet - maybe a wake-up call from the host.  Just discard it
      } else {
        //now we have something worth sending
        Serial.print(F("Transmitting "));
        Serial.print(oTNC.xmitLen());
        Serial.println(F(" characters..."));

        oTNC.xmitEnd();    //xmitEnd will start the timer interrupt that will be used to module the packet.
        //At the end of xmitEnd, it flushes the buffer and gets ready for the next packet.
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_COMPA_vect) {
  static boolean bBaudFlip = 0;
  static byte iStuffZero = 0;
  static boolean bStuffBit = false;


  static byte iRateGen;
  static unsigned int iTonePhase = 0;      //two byte variable.  The highByte contains the element in arySine that should be output'ed
  static boolean bToneHigh = 0;


  //digitalWrite(PIN_TP_ISRTIME, HIGH);    //measure how long the ISR is taking to execute

  //increment the phase counter.  It will overflow automatically at > 65535
  if (bToneHigh) {
    analogWrite(PIN_AUDIO_OUT, (pgm_read_byte_near(_arySineHigh + highByte(iTonePhase))));
    iTonePhase += TONE_HIGH_STEPS_PER_TICK;
  } else {
    analogWrite(PIN_AUDIO_OUT, (pgm_read_byte_near(_arySineLow + highByte(iTonePhase))));
    iTonePhase += TONE_LOW_STEPS_PER_TICK;
  }

  iRateGen--;


  if (iRateGen == 0) {
    //it's time for the next bit

    //used to measure the baud rate frequency.  Can be deleted for production
    //digitalWrite(PIN_TP_BAUDRATE, bBaudFlip);
    //bBaudFlip = !bBaudFlip;

    if (bStuffBit) {
      //we hit the stuffing counter  - we don't need to get the next bit yet, just change the tone and send one bit
      bToneHigh = !bToneHigh;
      iStuffZero = 0;

      bStuffBit = false;    //reset this so we don't keep stuffing

    } else {
      //this is just a normal bit - grab the next bit from the szString

      if (oTNC.getNextBit() == 0) {
        //we only flip the output state if we have a zero

        //Flip Bit
        bToneHigh = !bToneHigh;
        iStuffZero = 0;
      } else {
        //it's a 1, so send the same tone...

        iStuffZero++;      //increament the stuffing counter

        //if there's been 5 in a row, then we need to stuff an extra bit in, and change the tone for that one
        if (iStuffZero == 5 && !oTNC.noBitStuffing()) {
          bStuffBit = true;      //once we hit five, we let this fifth (sixth?) one go, then we set a flag to flip the tone and send a bogus extra bit next time
        }
      }
    }

    iRateGen = BAUD_GENERATOR_COUNT;
  }

  //digitalWrite(PIN_TP_ISRTIME, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




