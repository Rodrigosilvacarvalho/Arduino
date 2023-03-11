#include <SoftwareSerial.h>
#include <Wire.h>           //Add Wire lib 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "KalmanFilter.h"
#include "MS5611.h"
#include "ASM5915.h"
#include "SoftTimer.h"

boolean DEBUG = true;

int powInitPin = 12;
int powCtrlPin = 9;
int sndCtrlPin = 8;

int sndValuePinAnalog = 1;

uint8_t powState =HIGH;
MS5611           barometer(Wire, 0x76);
ASM5915          diffBarometer(Wire, 0x28);

KalmanFilter   kfPressure;
KalmanFilter   kfDiffPressure;
KalmanFilter   kfTemperature;

SyncSerial     sync(1,2);

BeepModul beepModul( Vector<int> (3, (int []){5,6,7}));


float lastAltitude=0;
long  lastCalcTime=0;



String  getCmd(const String &cmd){
  unsigned int checksum_end,ai,bi;                                                 
  // Calculating checksum for data string        
  for (checksum_end = 0, ai = 0; ai < cmd.length(); ai++)
  {
    bi = (unsigned char)cmd[ai];
    checksum_end ^= bi;
  }
  String out( "$" + cmd + ((checksum_end < 16 )?"*0":"*") + String(checksum_end, HEX));
  return out;
}


void baroSendCB(Task *self)
{
  //$PTAS1,xxx,yyy,zzzzz,aaa
  long delatTime = millis() - lastCalcTime;
  lastCalcTime = millis();
  char buffer[9];
  long p=kfPressure.get();
  float altitude = barometer.calcAltitude(p, 101325); 
  float deltaAltitude = altitude - lastAltitude;
  lastAltitude = altitude;
  altitude *=3.2808399; //feet
  
  long dp=kfDiffPressure.get();
  float t=kfTemperature.get(); 
  
  float rho=(p+dp)/(t+273.15)/287.058;
  float Vair =(dp < 0)? 0 : sqrt(2*dp/rho/1000)*1.943844492;
  Vair *=1.8;
  
  float speedM_S = deltaAltitude*1000/delatTime;
  beepModul.setSpeed(speedM_S);
  int Vvert = 1.943844492 * speedM_S*10+200;
  String strVVert = "";
  if(Vvert>1 && Vvert < 399)
    strVVert = String(Vvert);
  
  String str_out = String(
  "PTAS1,"+strVVert+",,"+String(dtostrf(altitude+2000, 1, 2, buffer))+","+String((int)Vair));
  str_out = getCmd(str_out)+"\r\n";
  sync.send(str_out);
}

void baroUpdateCB(Task *self)
{
  kfPressure.update(barometer.getPressure());
  kfDiffPressure.update(diffBarometer.getPressure());
  kfTemperature.update(diffBarometer.getTemperature());
  if(DEBUG) {
    sync.send("Pressure: "+ String((int)kfPressure.get()) + " / DiffPressure: " + String((int)kfDiffPressure.get()) +" \r\n");
  }
}


void powCtrlCB(Task *self)
{
  long vcc = Akku::readVcc();
  if(millis() > 10000){
    if( vcc <= 3700 || digitalRead(powCtrlPin) == HIGH) {
      digitalWrite(powInitPin, LOW);
    }
  }
}






void beepOnCallBack(Task *task){
  beepModul.beep();
}

int lastBeepCtrl = LOW;

void beepCtrlCB(Task *task){
  int beepCtrl = digitalRead(sndCtrlPin);
  if( beepCtrl == HIGH && lastBeepCtrl == LOW)
  {
    if(DEBUG) sync.send("setNextLevel\r\n");
    beepModul.setNextLevel();
  }
  lastBeepCtrl = beepCtrl; 
}

Task beep( 500, beepOnCallBack);
Task beepCtrl( 1000, beepCtrlCB);
Task taskBaroSend  (1000,  baroSendCB);
Task taskBaroUpdate(40,  baroUpdateCB);
Task taskPowCtrl   (1000, powCtrlCB);


void setup(){
  // set the data rate for the SoftwareSerial port
  sync.begin(9600);
  
  sync.send("$PMTK314,0,1,0,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2D\r\n", 3, false);
  sync.send("$PMTK220,200*2B\r\n", 3, false);
  sync.send("$PMTK313,1*2E\r\n", 3, false);  

  
  
  // Disable GPS
  if(DEBUG) {
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
  }
  
  pinMode(powInitPin, OUTPUT);
  pinMode(sndCtrlPin, INPUT);
  digitalWrite(powInitPin, HIGH);
  diffBarometer.begin();

  barometer.begin();
  SoftTimer.add(&taskBaroSend);
  SoftTimer.add(&taskBaroUpdate);
  

  SoftTimer.add(&beep); 
  SoftTimer.add(&beepCtrl); 
  SoftTimer.add(&taskPowCtrl);
}
