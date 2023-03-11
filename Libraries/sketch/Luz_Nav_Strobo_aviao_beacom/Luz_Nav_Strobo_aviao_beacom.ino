/* Led RGB para avioes  led ponta de asa verde 
e vermelho com flash branco e beacom vermelha
*/
 
#include <RGBLed.h>

RGBLed led1(11, 10, 9, COMMON_CATHODE);
RGBLed led2(8, 7, 6,   COMMON_CATHODE);
RGBLed led3(5, 4, 3,   COMMON_CATHODE);

void setup() {
  unsigned int delayvar = 100;//You can chane this but it wont do much
  
  pinMode(13, OUTPUT);  //define pins as output
  digitalWrite(13, LOW);
  delay(delayvar);


  // Set color to Led1 Red
  led1.setColor(255, 0, 0);
  delay(100);

  
    // Set color to Led2 Gren
  led2.setColor(0, 255, 0);
  delay(100);

  // Set color to Led3 Red
  led3.setColor(255, 0, 0);
  delay(100);
  
}

void loop() {


  // Flash Branca color, during 250ms with interval 100ms
  led1.flash(255, 255, 255, 250, 100);
  
  // Flash Branca color, during 250ms with interval 100ms
  led2.flash(255, 255, 255, 250, 100);
  

  // Fade out red color, with 5 steps during 100ms
  led3.fadeOut(255, 0, 0, 5, 100);
  
  // Fade in green color, with 5 steps during 100ms
  led3.fadeIn(255, 0, 0, 5, 100);
  
}
