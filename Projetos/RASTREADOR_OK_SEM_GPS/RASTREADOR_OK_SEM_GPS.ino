#include <Arduino.h>
#include <ArduinoQAPRS.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


TinyGPSPlus gps;
SoftwareSerial ss(5, 6);

#define LED_PIN     13


char * packet_buffer  = "                                                                    \n ";
char from_addr[] = "PT-RSC";
char dest_addr[] = "APDR14";
char relays[] = "WIDE1 1";


void setup(){
 	
	QAPRS.init(3,2);
  
  Serial.begin(115200);
  ss.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
}

void GPS(){
    while (ss.available() > 0)
    if (gps.encode(ss.read()))
    
  
  if (gps.location.isValid())
  {
    Serial.print("!");
    Serial.print(gps.location.lat(), 5);
    Serial.print("S/");
    Serial.print(gps.location.lng(), 5);
    Serial.print("W#");
    Serial.println(); 
  }

}

void loop() {

 

    //packet_buffer = "!1625.40S/05106.48WS#";
    
 
    
    digitalWrite(LED_PIN, HIGH);
    QAPRS.send(from_addr, '7', dest_addr, '0', relays);
    digitalWrite(LED_PIN, LOW);
    
    Serial.print("PT-RSC-7>APDR14,WIDE1-1:");
   void GPS();
    Serial.println("");

delay (500);

}
