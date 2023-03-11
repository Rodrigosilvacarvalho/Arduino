/**
 @file barometer.ino
 @brief This is an Example for the FaBo Barometer I2C Brick.

   http://fabo.io/204.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include <Wire.h>
#include <FaBoBarometer_MPL115.h>

FaBoBarometer faboBarometer;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("RESET");
  faboBarometer.begin();
}

void loop() {
  // 温度を表示
  Serial.print("Temp: ");
  Serial.println(faboBarometer.readTemp());
  // 海面気圧を表示
  Serial.print("hPa: ");
  Serial.println(faboBarometer.readHpa());
  // readHpa()に標高を渡す
  Serial.print("hPa: ");
  // 会津若松市の標高:212.0m
  Serial.println(faboBarometer.readHpa(212.0));
  delay(1000);
}
