/*
    Copyright (C) 2013 Lukasz Nidecki SQ5RWU
	
    This file is part of ArduinoQAPRS.

    ArduinoQAPRS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ArduinoQAPRS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduinoQAPRS; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
 #include <string.h>
 #include <Arduino.h>
 #include <ArduinoQAPRS.h>
 #include <SPI.h>

char * packet_buffer  = "                                                                    \n ";
char floatbuf[32];
char gradbuf[4];
char from_addr[] = "DJ7OO";      // change according to your needs
char dest_addr[] = "APRS  ";     // change according to your needs
char relays[] = "WIDE2 1";       // change according to your needs 

String lat,lon,ns,ew;
String aprs = "";
String GPSStringbuf = "";
String GPSString = "";
int ledPin = 13;                  // LED test pin
int status,alti; 
boolean stringComplete = false;

///////////////////////////////////////////////////////// 
void setup()
{
  Serial.begin(9600);            // data rate GPS receiver
  Serial.println("WAIT!");
  delay(100);
  pinMode(ledPin, OUTPUT);       // initialize LED pin
  GPSString.reserve(100);        // reserving 100 characters
  Serial.begin (9600);         
  QAPRS.init(3,2);
  delay(2000); 
}

/////////////////////////////////////////////////////////// 
void loop()
{
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read(); 
    GPSString += inChar;
    if (inChar == '\n') {
    stringComplete = true;
    }
  }
  if (stringComplete) 
  {    
    if(GPSString.indexOf("$GPGGA") != -1) // check for correct header
    {
//    Serial.println(GPSString);
    
//----------------Latitude ----------------------------------------------------------------      
      GPSStringbuf = GPSString.substring(position(2), position(3) - 1); 
      lat = GPSStringbuf.substring(0,7);

//----------------NS---------------------------------------------------      
      GPSStringbuf = GPSString.substring(position(3), position(4) - 1); 
      ns = GPSStringbuf;

//----------------Longitude -------------------------------------------                  
      GPSStringbuf = GPSString.substring(position(4), position(5) - 1); 
      lon = GPSStringbuf.substring(0,8);
 
//----------------EW----------------------------------------------------      
      GPSStringbuf = GPSString.substring(position(5), position(6) - 1); 
      ew = GPSStringbuf;



//----------------Altitude ( feet ) ------------------------------------
      GPSStringbuf = GPSString.substring(position(9), position(10) - 1); 
      GPSStringbuf.toCharArray(floatbuf, sizeof(floatbuf)); 
      alti = (atoi(floatbuf))*2.54;     



// combining part strings for APRS 
   aprs = "!" + lat + ns + "/" + lon + ew + ">A=" + alti + "#";

// convert string to char_array
   int str_len = aprs.length() + 1; 
   char char_array[str_len];
   aprs.toCharArray(char_array, str_len);

// send via packet_buffer 
   packet_buffer = char_array;
   Serial.print("Send:    "),Serial.println(packet_buffer); 
   QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
   }

  

}



