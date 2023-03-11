ArduinoTrack
Copywrite 2011-2016 - Zack Clobes (W0ZC), Custom Digital Services, LLC


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

ArduinoTrack is a trademark of Custom Digital Services, LLC.


## Programming Instructions ##
The ArduinoTrack/ArduinoTrack_Modem firwmare is designed for use with the Arduino 1.6.8+ development environment.  There are three programming 
scenarios that must be considered.  

The first known as the _Combined mode is where the ArduinoTrack functions as both the "Arduino" and the "Modem/TNC".  To program the combined 
board, use the instructions below.

The second mode is where a standard Arduino is used to host the Arduino code, and then the ArduinoTrack shield is mated to the Arduino. For this
use case, the Arduino is programmed with the Arduino firmware with the "#define AT_COMBINED" line commented out, and programmed to a standard
Arduino board as normal.  Then, for the ArduinoTrack shield, it is programmed using the instructions below, with the ArduinoTrack_Modem firmware.


Step 1:
Be sure you have the Arduino 1.6.8+ development environment installed.  These instructions are specifically for the 1.6+ version of Arduino, 
downloadable from http://arduino.cc/.

Step 2:
With the Arduino software CLOSED, copy the file boards.local.txt from this repository into 
"C:\Program Files (x86)\Arduino\hardware\arduino\avr\"

Step 3:
Launch the Arduino software.

Step 4:
On a separate Arduino Uno or similar, install the Arduino ISP Example Sketch the the Uno.

Step 5:
Plug the ArduinoTrack shield into Arduino that is programmed as an ISP programmer.

Step 6: (First time only)
Open a command prompt on the PC.
  C:\> cd \Program Files (x86)\Arduino\hardware\tools\avr\bin

  * Replace "comX" with whatever Com port your ArduinoISP is enumerating on.
  C:\Program Files (x86)\Arduino\hardware\tools\avr\bin> avrdude.exe -p m328p  -b 19200 -c avrisp -C ..\etc\avrdude.conf -P comX -U hfuse:w:0xD6:m -U lfuse:w:0xFF:m -U efuse:w:0x05:m

  # This will write the fuses to the new processor.  This is only necessary to perform once after assembling a new board.  You can verify the settings with the following:
  C:\Program Files (x86)\Arduino\hardware\tools\avr\bin> avrdude.exe -p m328p  -b 19200 -c avrisp -C ..\etc\avrdude.conf -P comX -U hfuse:r:highfuse:h -U lfuse:r:lowfuse:h -U efuse:r:exfuse:h
  C:\Program Files (x86)\Arduino\hardware\tools\avr\bin> cat highfuse
  C:\Program Files (x86)\Arduino\hardware\tools\avr\bin> cat lowfuse
  C:\Program Files (x86)\Arduino\hardware\tools\avr\bin> cat exfuse

Step 7:
From the Tools-Board menu, select the ArduinoTrack Shield from the list of targets.  From the
Tools-Port menu, select the appropriate COM port for the ISP programmer Arduino.

Step 8:
From the Tools-Programmer menu, select Arduino as ISP from the list.

Step 9:
To program the shield, go to Sketch-Upload Using Programmer.  The Sketch will compile and will program to the ATMega328 processor similarly
to how a normal Arduino with the bootloader is programmed.  However, in this situation, no bootloader is required on the shield.




See www.projecttraveler.org for full construction and programming instructions.




## Version History Information prior to v3.0.0 ##
Version 2.1.10 - October 18, 2015 - Fixed bug in GPS object where strings were exceeding their buffers.  Fixing the buffer overflow fixed an issue with temp/pressure showing 0 with GPS lock.
Version 2.1.9 - October 7, 2015 - Reduced the maximum transmit size from 250 to 200 bytes in TNC.h.  Fixed bug with long tx-delays on Simple time and on Speed-based beaconing.
Version 2.1.8 - August 31, 2015 - Minor fix to correct IAT/Pressure from not transmitting.  Altering instructions to reflect Arduino 1.6.5 IDE change.
Version 2.1.7 - August 13, 2015 - Added support for TMP102 outside air temp.  Also added piezo speaker to exercise routines.
Version 2.1.5 - June 9, 2015 - Added exercising routine to test new boards.
Version 2.1.4 - June 6, 2015 - Fixed a small logic error, and eliminated some serial debugging no longer needed.  Added a manual configuration option to reset configs to defaults.
Version 2.1.3 - May 23, 2015 - Fix issue with Ublox fix quality showing up as 'na'.  Initializing greeting now displays firmware version.  Adjusted the firmware programming instructions.
Version 2.1.0 - May 3, 2015 - Several changes to accomidate the version 1.10 of the PCB, including support for Ublox GPS's, switching from 1-wire to SPI for temp/baro pressure, and enabling more IO pins to be available for addons.
Version 2.0.3 - March 29, 2015 - Simple change to instructions only to accomidate Arduino IDE 1.6.2.  Also changed high fuse to keep eeprom values from being reset during firmware flash.
Version 2.0.2 - February 27, 2015 - Added a configuration annunciation, altered the GPS constructor.  Altered the HighBit to prevent the erasure of eeprom data after firmware updates. Uploaded firmware to GitHub.
Version 2.0.1 - February 14, 2015 - Updated tnc.cpp to include additional debugging and fixed an issue with PWM audio on PIN_AUDIO_OUT.  Numerous small changes to sync codebase between ArduinoTrack and _Combined.
Version 2.0.0 - January 15, 2015 - Integrated the common TNC library into the Combined Sketch.  Licensed ArduinoTrack_Combined under the GPL3 license.
Version 1.2.1 - January 8, 2015 - All functionality back and operations for combined tracker.
Version 1.2.0 - January 5, 2015 - Basic functionality now running on standalone (combined) tracker.
Version 1.1.0 - November 8, 2014 - Modifications to run with ArduinoTrack Shield version 1.0.0.  Adjusted timing delays on annunciator to sound more natural.
Version 1.0.2 - September 12, 2014 - Fixed issue with altitude based tracking transmitting continuously.  Fixed negative temperatures being transmitted incorrectly.
Version 1.0.1 - July 1, 2014 -
Version 1.0.0 - June 7, 2014 - Finalized the configuration for GPSL 2014.
Version 0.9.5 - March 9, 2014 - Introduced the PC-based "Configurator" tool.  Saves all configs to EEPROM.
Version .900 - July 16, 2011 - Ready for GPSL Traveler Flight 2011b
Version .901 - August 27, 2011 - Added mycall and Path statements to the KPC3 Init TNC, split the SSID's out into separate Chars,
Version .950 - May 31, 2012
      * Numerous fixes to get Arudino Track to compile in the v1.0 IDE
      * Switched to stock SoftwareSerial.h library instead of stand-alone
      * Changed time slots for PT-2012a (GPSL) flight.
Version .951 - Oct 18, 2012
      * Updated for 2012b flight.
      * Removed the SSID's that were split in .901.  Decided that it was easier to have a single Char array when it comes to dumping
         the values out to the TNC's