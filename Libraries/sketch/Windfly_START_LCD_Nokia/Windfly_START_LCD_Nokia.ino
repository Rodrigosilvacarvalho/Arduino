
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <SPI.h>
#include <Wire.h>                      //i2c library
#include "BMP085.h"                    //bmp085 library, download from url link (1)
#include <Tone.h>                      //tone library, download from url link (3)
#include<stdlib.h>                     //we need that to use dtostrf() and convert float to string



Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);


///////////////////////////////////////// variables that You can test and try
short speaker_pin1 = 8;   //    9        //arduino speaker output -
short speaker_pin2 = 9;    //10          //arduino speaker output +
float vario_climb_rate_start = 0.5;      //minimum climb beeping value(ex. start climbing beeping at 0.4m/s)
float vario_sink_rate_start = -1.1;      //maximum sink beeping value (ex. start sink beep at -1.1m/s)
#define SAMPLES_ARR 25                   //define moving average filter array size (2->30), more means vario is less sensitive and slower, NMEA output
#define UART_SPEED 9600                //define serial transmision speed (9600,19200, etc...)
#define PROTOCOL 2                       //define NMEA output: 1 = $LK8EX1, 2 = FlymasterF1, 3 = Off-Line Test with Serial Monitor (Arduino) 
#define NMEA_OUT_per_SEC 1               // NMEA output string samples per second (1 to 20)
#define VOLUME 2                         // volume setting 0-no sound, 1-low sound volume, 2-high sound volume
/////////////////////////////////////////



//---------------------------------------------------------------------------------------------------------
BMP085   bmp085 = BMP085();              //set up bmp085 sensor
Tone     tone_out1;
Tone     tone_out2;
long     Temperature = 0;
long     Pressure = 101325;
float    Altitude=0;
const float p0 = 101325;                 //Pressure at sea level (Pa)
unsigned long get_time1 = millis();
unsigned long get_time2 = millis();
unsigned long get_time3 = millis();
int      my_temperature = 1;
char     altitude_arr[6];            //wee need this array to translate float to string 
char     vario_arr[6];               //wee need this array to translate float to string
char     batt_arr[6];
int      samples=40;
int      maxsamples=50;
float    alt[51];
float    tim[51];
float    beep;
float    Beep_period;

//---------------------------------------------------------------------------------------------------------

static long k[SAMPLES_ARR];
static long Averaging_Filter(long input);
static long Averaging_Filter(long input) // moving average filter function
{
  long sum = 0;
  for (int i = 0; i < SAMPLES_ARR; i++) {
    k[i] = k[i+1];
  }
  k[SAMPLES_ARR - 1] = input;
  for (int i = 0; i < SAMPLES_ARR; i++) {
    sum += k[i];
  }
  return ( sum / SAMPLES_ARR ) ;
}


//---------------------------------------------------------------------------------------------------------
void play_welcome_beep()                 //play only once welcome beep after turning on arduino vario
{
  
     
  for (int aa=300;aa<=1500;aa=aa+100)
  {
    tone_out1.play(aa,200);             // play beep on pin (note,duration)
   if (VOLUME==2){ tone_out2.play(aa+5,200);}             // play beep on pin (note,duration)
    delay(100);
  }
  for (int aa=1500;aa>=100;aa=aa-100)
  {
    tone_out1.play(aa,200);             // play beep on pin (note,duration)
   if (VOLUME==2){ tone_out2.play(aa+5,200);}             // play beep on pin (note,duration)
    delay(100);
  }
}

//---------------------------------------------------------------------------------------------------------

void gpsUpdate(){
   while(Serial.available()){
    
    if (Serial.available()) {
   }
    Serial.write(Serial.read());
   }
}
//---------------------------------------------  void setup  ------------------------------------------------------------

void setup()                // setup() function to setup all necessary parameters before we go to endless loop() function
{
  Serial.begin(UART_SPEED);       // set up arduino serial port
  Wire.begin();                   // lets init i2c protocol

  display.begin();
  // Ajusta o contraste do display
  display.setContrast(50);
  // Apaga o buffer e o display
  display.clearDisplay();
 
 
  display.setTextSize(1);
  display.setTextColor(BLACK);
  

//---------------------------------------------------------------------------------------------------------
  
 
  if (VOLUME==1){
  tone_out1.begin(speaker_pin1);       // piezo speaker output pin8 -
  pinMode(speaker_pin2, OUTPUT);      // set pin for output NMEA LED blink;
  digitalWrite(speaker_pin2, LOW); 
  } 
  else if (VOLUME==2){
  tone_out1.begin(speaker_pin1);       // piezo speaker output pin8 -
  tone_out2.begin(speaker_pin2);       // piezo speaker output pin9 +
  }
  
//---------------------------------------------------------------------------------------------------------
  
  
  bmp085.init(MODE_ULTRA_HIGHRES, p0, false); 
                            // BMP085 ultra-high-res mode, 101325Pa = 1013.25hPa, false = using Pa units
                            // this initialization is useful for normalizing pressure to specific datum.
                            // OR setting current local hPa information from a weather station/local airport (QNH).
  

 
//----------------------------------------Beeb Inicial -----------------------------------------------------------------
  
  play_welcome_beep();      //everything is ready, play "welcome" sound

  
  
//---------------------------------------------------------------------------------------------------------

}
float nmea_vario_cms =0;
float nmea_time_s=0;
float nmea_alt_m=0;
float nmea_old_alt_m=0;


char variostring[6], altstring[6]; // create string arrays

//--------------------------------------------  void loop  -------------------------------------------------------------

void loop(void)
{
  float time=millis();              //take time, look into arduino millis() function
  float vario=0;
  float N1=0;
  float N2=0;
  float N3=0;
  float D1=0;
  float D2=0;
 

//-------------------------------------------- Calculo Pressao ------------------------------------------------------------- 
  bmp085.calcTruePressure(&Pressure);                                   //get one sample from BMP085 in every loop
  long average_pressure = Averaging_Filter(Pressure);                   //put it in filter and take average, this averaging is for NMEA output
  Altitude = (float)44330 * (1 - pow(((float)Pressure/p0), 0.190295));  //take new altitude in meters from pressure sample, not from average pressure

//--------------------------------------------- Calculo Altitude ------------------------------------------------------------
 nmea_alt_m = (float)44330 * (1 - pow(((float)average_pressure/p0), 0.190295));
 
 if ((millis() >= (nmea_time_s+1000))){
 nmea_vario_cms = ((nmea_alt_m-nmea_old_alt_m))*100; 
 nmea_old_alt_m = nmea_alt_m;
 nmea_time_s = millis();
 }

//----------------------------------------------  Variometro -----------------------------------------------------------

  for(int cc=1;cc<=maxsamples;cc++){                                    //vario algorithm
    alt[(cc-1)]=alt[cc];                                                //move "altitude" value in every loop inside table
    tim[(cc-1)]=tim[cc];                                                //move "time" value in every loop inside table
  };  
  
 
  //now we have altitude-time tables
  alt[maxsamples]=Altitude;                                             //put current "altitude" value on the end of the table
  tim[maxsamples]=time;                                                 //put current "time" value on the end of the table
  float stime=tim[maxsamples-samples];
  for(int cc=(maxsamples-samples);cc<maxsamples;cc++){
    N1+=(tim[cc]-stime)*alt[cc];
    N2+=(tim[cc]-stime);
    N3+=(alt[cc]);
    D1+=(tim[cc]-stime)*(tim[cc]-stime);
    D2+=(tim[cc]-stime);
  };

 vario=1000*((samples*N1)-N2*N3)/(samples*D1-D2*D2);
 if ((time-beep)>Beep_period)                          // make some beep
  {
    beep=time;
    if (vario>vario_climb_rate_start && vario<=10 )
    {
      switch (VOLUME) 
      {
        case 0: 
          break;
        case 1:
          Beep_period=550-(vario*(30+vario));
          tone_out1.play((1400+(200*vario)),420-(vario*(20+vario))); //when climbing make faster and shorter beeps
        case 2:
          Beep_period=550-(vario*(30+vario));
          tone_out1.play((1400+(200*vario)),420-(vario*(20+vario))); //when climbing make faster and shorter beeps
          tone_out2.play((1406+(200*vario)),420-(vario*(20+vario)));
      }               
    } else if (vario >10) 
    {
     
    switch (VOLUME) 
      {
        case 0: 
          break;
        case 1:
          Beep_period=160;
          tone_out1.play(3450,120);                          //spike climb rate
        case 2:
          Beep_period=160;
          tone_out1.play(3450,120);                          //spike climb rate
          tone_out2.play(3456,120);
      }               
    } else if (vario< vario_sink_rate_start){           //if you have high performace glider you can change sink beep to -0.95m/s ;)
     
     switch (VOLUME) 
      {
        case 0: 
          break;
        case 1:
          Beep_period=200;
          tone_out1.play(300,340);
        case 2:
          Beep_period=200;
          tone_out1.play(300,340);
          tone_out2.play(320,340);
      }               
    }
  }


//------------------------------------------------ Temperatura ---------------------------------------------------------

if (millis() >= (get_time2+1000))      //every second get temperature and battery level
  {
    bmp085.getTemperature(&Temperature); // get temperature in celsius from time to time, we have to divide that by 10 to get XY.Z
    my_temperature = Temperature/10 -5;
//------------------------------------------------ Bateria Voltagem ---------------------------------------------------------

    //Battery level
    uint8_t vccPixels = 5;
    uint8_t bat_x = 70;
    uint8_t bat_y = 2;
    display.drawRect(bat_x + 2, bat_y, 10, 6, BLACK);
    display.fillRect(bat_x, bat_y + 2, 2, 2, BLACK);
    display.fillRect(bat_x + 3 + 8 - vccPixels, bat_y + 1, vccPixels, 4, BLACK);

    display.display();



  }


 
//--------------------------------------------- Flymaster F1 Protocol ------------------------------------------------------------

            // $VARIO,fPressure,fVario,Bat1Volts,Bat2Volts,BatBank,TempSensor1,TempSensor2*Checksum
   
if ((millis() >= (get_time3+(1000/NMEA_OUT_per_SEC))) && (PROTOCOL == 2))       //every NMEA_OUT/second send NMEA output over serial port (Flymaster F1 Protocol)
  {
    char buffer[9];
    
    String str_out =
    String("VARIO,"
    +String(Pressure,DEC)
    + String(",")
    +String(dtostrf((nmea_vario_cms/10),0,0,vario_arr))
    +String(",")
    +String("0")
    +String(",0,1")
    +String(",")
    +String(my_temperature,DEC)
    +String(",0"));
     
     unsigned int checksum_end,ai,bi;                                                 // Calculating checksum for data string
    for (checksum_end = 0, ai = 0; ai < str_out.length(); ai++)
    {
      bi = (unsigned char)str_out[ai];
      checksum_end ^= bi;
    }
    
    Serial.print("$");                     //print first sign of NMEA protocol
    Serial.print(str_out);                 // print data string
    Serial.print("*");                     //end of protocol string
    Serial.println(checksum_end,HEX);      //print calculated checksum on the end of the string in HEX
    
   String str_out2 =                                                                 //combine all values and create part of NMEA data string output
   String("PGRMZ,"
  +String(dtostrf(nmea_old_alt_m, 1, 2, buffer)) 
  + String(",")
  +"m,3");

                                                
    for (checksum_end = 0, ai = 0; ai < str_out2.length(); ai++)
    {
      bi = (unsigned char)str_out2[ai];
      checksum_end ^= bi;
    }
    Serial.print("$");                     //print first sign of NMEA protocol
    Serial.print(str_out2);                 // print data string
    Serial.print("*");                     //end of protocol string
    Serial.println(checksum_end,HEX);      //print calculated checksum on the end of the string in HEX 
   
   gpsUpdate();
    get_time3 = millis();
  }
//------------------------------------------ LCD  DISPLAY ---------------------------------------------------------------
    // Retangulo Menu
  
    display.drawRoundRect(0,0, 84 ,48, 3, 3);
   
    display.setCursor(3,3);
    display.print("12");
    display.print(":");
    display.print("00");
    display.print(":");
    display.print("05");
    

    display.setTextSize(2);
    display.setCursor(3,13);
    display.print(dtostrf(Altitude,0,0,altitude_arr));
    display.setCursor(45,13);
    display.print(" m");
    
    
    display.setTextSize(1);
    display.setCursor(45,40);
    display.print(average_pressure,DEC); 
    display.setTextSize(1);
   display.setCursor(75,40);
    display.print("P");
    
    
     display.setTextSize(2);
     display.setCursor(10,30);
     display.print(dtostrf((nmea_vario_cms/10),0,0,vario_arr));
     display.setTextSize(1);
     display.setCursor(30,30);
     display.print(" m/s");
    
    display.setTextSize(1);
    display.setCursor(60,30);
    display.print(my_temperature,DEC);
    display.setCursor(75,30);
    display.print("C");
    

    
     display.display();
 
   display.clearDisplay();
  
}
//The End
