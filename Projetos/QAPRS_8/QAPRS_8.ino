
#include <string.h>
#include <Arduino.h>
#include <ArduinoQAPRS.h>
#include <SPI.h>
#include <TinyGPS++.h>
TinyGPSPlus gps;



String lat,lon,ns,ew;
String aprs = "";
String GPSStringbuf = "";
String GPSString = "";


char * packet_buffer  = "                                                                                                                     \n ";
char from_addr[] = "PT-RSC";
char dest_addr[] = "APDR14";
char relays[] = "WIDE1 1";

#define LED_PIN     13             


void setup()
{
  Serial.begin(9600);            // data rate GPS receiver
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 
  QAPRS.init(3,2);

}


void loop()
{
 
 
    while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
    
  
  if (gps.location.isValid())
  {
    Serial.print("!");
    Serial.print(gps.location.lat(), 5);
    Serial.print("S/");
    Serial.print(gps.location.lng(), 5);
    Serial.print("W#");
    Serial.println(); 
  


// combining part strings for APRS 
   aprs = "!" + (gps.location.lat(), 5) + "S" + "/" + (gps.location.lng(), 5) + "W" + ">#";

// convert string to char_array
   int str_len = aprs.length() + 1; 
   char char_array[str_len];
   aprs.toCharArray(char_array, str_len);

// send via packet_buffer 
   packet_buffer = char_array;
   Serial.print("Send:    "),Serial.println(packet_buffer); 
   QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
   }
   
    
    packet_buffer = "teste";
    
 
     packet_buffer = "!1625.40S/05106.48W#EM_VOO";  // Rampa Sul Barra
      
     digitalWrite(LED_PIN, HIGH);
     QAPRS.send(from_addr, '7', dest_addr, '0', relays, packet_buffer);
     digitalWrite(LED_PIN, LOW);
     delay(1000); 

 delay(5000);

  }

}




