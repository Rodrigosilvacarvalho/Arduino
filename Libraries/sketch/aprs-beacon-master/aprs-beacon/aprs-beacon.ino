#include <Arduino.h>
#include <ArduinoQAPRS.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


#define TX_PIN  2                          // PTT
#define TX_DISABLE_PIN  3
#define GPS_TX_PIN  7                     
#define GPS_RX_PIN  6                     // ligado no TX GPS 
#define GPS_BAUD    9600
#define LED_PIN     13

char packet_buffer  = "                                                                    \n ";
char from_addr[] = "PT-RSC";  // your callsign
char dest_addr[] = "APDR14";
char relays[] =    "WIDE1 1";
char aprs_icon =   '>';

TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);


void setup() {

  ss.begin(GPS_BAUD);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
   Serial.begin(115200);
  QAPRS.init(TX_DISABLE_PIN, TX_PIN);             // Pino2 PTT Pino 3 botao para desabilitar APRS
}

void loop() {
  delay(1000);

while (ss.available() > 0)
    if (gps.encode(ss.read()))
   
    
    packet_buffer = (gps.location.lat()), (gps.location.lng());
    
    digitalWrite(LED_PIN, HIGH);
    QAPRS.send(from_addr, '7', dest_addr, '0', relays, packet_buffer);
    Serial.print(packet_buffer);
    digitalWrite(LED_PIN, LOW);
  

  delay(1000);	// wait 60 secs
 
}
