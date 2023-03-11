

/* this is an prototype, code need to be changed for every button so need to flash arduino for each button but of course it can be solved much more elegant */

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int code = 6249743;
int i;

void setup() {

  Serial.begin(9600);
 
  // Transmitter is connected to Arduino Pin #10 
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  mySwitch.setPulseLength(600);
 
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1); 
}

void loop() {

  /* Same switch as above, but using decimal code */
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  i = 0;
  //cde is send 4 times
  while (i < 4)
  {
    mySwitch.send(code, 24);
    i++;
    delay(400);
  }
  digitalWrite(13,LOW);
  delay(3000);
}
