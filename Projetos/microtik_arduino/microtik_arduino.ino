
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display

SoftwareSerial mikrotik(10, 11); // RX, TX

bool validDataFound = false;
char buffer[100];

void setup()  
{
 Serial.begin(9600);
 mikrotik.begin(9600);
 lcd.init();
 lcd.backlight();
 writeLcd("Reading Mikrotik");
}

void loop() 
{
 storeData();
 writeLcd(buffer);
}

void writeLcd(char * name)
{
 lcd.clear();
 lcd.setCursor(1,0);//Collum 1, Row 0 
 lcd.print("Arduino + Mikrotik");
 lcd.setCursor(3,1);//Collum 3, Row 1
 lcd.print("Mikrotik name:");
 lcd.setCursor(3,2);//Collum 3, Row 2  
 lcd.print(name);
 
}

void findMarker()
{
 byte tester;
 while(1)
 {
  if(mikrotik.available()>0)
     {
       tester = mikrotik.read();
       Serial.write(tester);
     } 
  if(tester == 0x3A)break; //Means de marker : has been found   
 }
}
 
void storeData()
{
uint8_t i =0;
byte temp;
buffer[0] = '\0';
findMarker();  
while(1)
{
  if(mikrotik.available()>0)
  {
    temp = mikrotik.read();
    if(temp != 0x20 )
     {
      //Insert the byte on the char array
      validDataFound = true;
      buffer[i++] = temp;
      buffer[i] = '\0';
     }
    if(validDataFound == true && temp == 0x20)
     {  
      validDataFound = false;
      break;   
     }      
   }     
 }
Serial.println(buffer);
}

