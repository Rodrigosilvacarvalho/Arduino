
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <Wire.h>
#include <BMP085.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

BMP085 dps = BMP085();
long Temperature = 0, Pressure = 0, Altitude = 0;


void setup()  
{
    
   Wire.begin();
   delay(1000);
    dps.init();   
   

     
  display.begin();
  // Ajusta o contraste do display
  display.setContrast(45);
  // Apaga o buffer e o display
  display.clearDisplay();
  // Define tamanho do texto e cor
  display.setTextSize(1);
  display.setTextColor(BLACK);

} 
void loop()

{

  // Retangulo pressao
  display.drawRoundRect(0,0, 84 ,48, 3, 3);
  
  
   display.setCursor(3,3);
    display.print("12");
    display.print(":");
    display.print("00");
    display.print(":");
    display.print("05");


    display.setCursor(56,2);
    display.print("3D");

     display.setCursor(60,15);
     display.print(Temperature);
     display.print("C");


    
     display.setTextSize(2);
     display.setCursor(3,33);
     display.print(Altitude);
     display.print(" m");
    
    display.setTextSize(1);
    display.setCursor(10,15);
    display.setTextSize(2);
    display.print("Pressure");
    display.setTextSize(1);
    display.setCursor(60,20);
    display.print(" Hpa");

  display.display();
  

    //Battery level
    uint8_t vccPixels = 5;
    uint8_t bat_x = 70;
    uint8_t bat_y = 2;
    display.drawRect(bat_x + 2, bat_y, 10, 6, BLACK);
    display.fillRect(bat_x, bat_y + 2, 2, 2, BLACK);
    display.fillRect(bat_x + 3 + 8 - vccPixels, bat_y + 1, vccPixels, 4, BLACK);

    display.display();
  // Aguarda 1 segundos para efetuar nova leitura
  delay(1000);
}
