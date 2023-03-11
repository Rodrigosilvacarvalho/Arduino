#include "U8glib.h"  
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
  
 
//U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET  

U8GLIB_ST7920_128X64_1X u8g(13, 11, 12 ,3); //Enable, RW, RS, RESET  
   
void setup() 
{  
  
  
  Serial.begin(9600);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  
} 
   
void loop() 
{  
  
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
   u8g.setColorIndex(1);
  
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 11, 35, "Arduino e Cia");  
  u8g.drawStr( 12, 35, "Arduino e Cia");  
  u8g.drawFrame(0,0,128,64);  
  u8g.drawFrame(2,2,124,60); 
  
  
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    u8g.drawStr(5,25, bmp.readTemperature()); 
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    
    Serial.println();
    delay(500);
 
}    
