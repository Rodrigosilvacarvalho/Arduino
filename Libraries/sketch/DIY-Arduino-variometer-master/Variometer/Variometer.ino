#include <EEPROM.h>
#include <EEPROMAnything.h>
#include <MenuBackend.h>
#include <Encoder.h>
#include <Wire.h>  //i2c library
#include <toneAC.h>
#include <Adafruit_BMP085.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RTClib.h>

#include "U8glib.h"

/////////////////////////////////////////
bool initialisation = false;  //If true, reset and update eeprom memory at arduino start
/////////////////////////////////////////

//////////////////RTC///////////////////////
RTC_DS1307 rtc;
#define DATE_DAY 1
#define DATE_MONTH 2
#define DATE_YEAR 3
#define DATE_HOUR 4
#define DATE_MINUTE 5
uint8_t conf_date_displayed = 0;

uint16_t date_year;
uint8_t date_month;
uint8_t date_day;
uint8_t date_hour;
uint8_t date_minute;
/////////////////////////////////////////

//////////////////ENCODER///////////////////////
#define Enter 12
#define ENCODER_STEP 4

Encoder knob(3, 2);
long knobPosition = 0;
int lastEnterState = HIGH;
/////////////////////////////////////////

//////////////////MENU/////////////////////////
bool menuUsed = false;
bool menuUsed_last = false;
bool varioState = false;
uint8_t stat_displayed = 1;
bool stat_blink_status = false;

#define MENU_RIGHT 0
#define MENU_LEFT 1
#define MENU_VARIO 2
#define MENU_TARE 10
#define MENU_QNH 11
#define MENU_ALTITUDE 12
#define MENU_MONTEE 13
#define MENU_DESCENTE 14
#define MENU_LIGHT 15
#define MENU_CONTRASTE 16
#define MENU_DATE 17
#define MENU_STAT 20
#define MENU_RECRESET 21

void menuChangeEvent(MenuChangeEvent changed);
void menuUseEvent(MenuUseEvent used);

//this controls the menu backend and the event generation
MenuBackend menu = MenuBackend(menuUseEvent, menuChangeEvent);

MenuItem m_vario = MenuItem(NULL, MENU_VARIO); //Vario
MenuItem m_options = MenuItem(NULL, MENU_RIGHT); //Options
MenuItem m_stats = MenuItem(NULL, MENU_RIGHT); //Records
MenuItem m_retour = MenuItem(NULL, MENU_LEFT); //Retour

MenuItem m_tare = MenuItem(NULL, MENU_TARE); //Tare
MenuItem m_qnh = MenuItem(NULL, MENU_QNH); //QNH
MenuItem m_altitude = MenuItem(NULL, MENU_ALTITUDE); //Altitude
MenuItem m_montee = MenuItem(NULL, MENU_MONTEE); //Montée
MenuItem m_descente = MenuItem(NULL, MENU_DESCENTE); //Descente
MenuItem m_light = MenuItem(NULL, MENU_LIGHT); //eclairage
MenuItem m_contrast = MenuItem(NULL, MENU_CONTRASTE); //contrast
MenuItem m_date = MenuItem(NULL, MENU_DATE); //date

MenuItem m_retour2 = MenuItem(NULL, MENU_LEFT); //Retour
MenuItem m_stat = MenuItem(NULL, MENU_STAT); //Stat
MenuItem m_recreset = MenuItem(NULL, MENU_RECRESET); //Reset records

/////////////////////////////////////////

//////////////////ECRAN///////////////////////

U8GLIB_ST7920_128X64_1X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

/////////////////////////////////////////

/////////////////////VARIO/////////////////////////
Adafruit_BMP085_Unified bmp085 = Adafruit_BMP085_Unified(10085); //set up bmp085 sensor
#define ALTI_TRIGGER 4 //Trigger to start and stop chrono in meter
float Altitude;
int altitude_temp;
uint8_t chrono_cpt = 0;

float vario = 0;
float vario_old = 0;
float vario_diff = 0;
uint8_t timeNoPauseBeep = 0;
bool noSound = false;
unsigned long get_timeBeep = millis();
uint8_t beepLatency = 0;

bool is_vario_button_push = false;
uint16_t average_vcc = 0;             //variable to hold the value of Vcc from battery
float average_pressure;
unsigned long get_time1 = millis();

uint8_t push_write_eeprom = 6;
float    my_temperature;
float    alt;
float    tim;
float    cumul_alt;

#define memoryBase 32
// Configuration structure (89 bits)
struct Conf
{
  int8_t vario_climb_rate_start;    //minimum climb beeping value(ex. start climbing beeping at 0.4m/s)
  int8_t vario_sink_rate_start;    //maximum sink beeping value (ex. start sink beep at -1.1m/s)
  int currentAltitude;
  uint8_t light_cpt;
  uint8_t contrast_default;
  bool volume;
  float p0;
  uint8_t stat_index;
} conf = {
  3, -11 , 0, 0, 50, true, 1040.00, 0
};

// Statistic structure (112 bits)
#define  NB_STATS 8
struct Stat
{
  uint32_t chrono_start;
  uint16_t chrono;
  int alti_max;
  int alti_min;
  uint8_t txchutemax;
  uint8_t txchutemin;
  uint16_t cumul_alt;
}  stat = {
  0, 0, -20000, 20000, 0, 0, 0
};

float getVarioClimbRateStart()
{
  return (float)conf.vario_climb_rate_start / 10;
}

float getVarioSinkRateStart()
{
  return (float)conf.vario_sink_rate_start / 10;
}

void readStat(uint8_t index = conf.stat_index, Stat &value = stat)
{
  EEPROM_readAnything(sizeof(Conf) + sizeof(Stat) * index, value);
}

void writeStat(uint8_t index = conf.stat_index, Stat &value = stat)
{
  EEPROM_writeAnything(sizeof(Conf) + sizeof(Stat) * index, value);
}

void incrementStatIndex()
{
  conf.stat_index = (conf.stat_index == NB_STATS - 1) ? 0 : conf.stat_index + 1;
  EEPROM_writeAnything(0, conf);
  readStat();
}

void resetStat(uint8_t index = conf.stat_index, bool sound = true)
{
  stat.chrono_start = 0;
  stat.chrono = 0;
  stat.alti_max = -20000;
  stat.alti_min = 20000;
  stat.txchutemax = 0;
  stat.txchutemin = 0;
  stat.cumul_alt = 0;
  writeStat(index);

  altitude_temp = Altitude;

  chrono_cpt = 0;

  if (sound) {
    playConfirmMelody();
  }
}

void resetAllStats()
{
  for (uint8_t i = 0; i < NB_STATS; i++) {
    readStat(i);
    resetStat(i, false);
  }
  conf.stat_index = 0;
  EEPROM_writeAnything(0, conf);
  readStat(0);

  playConfirmMelody();
}

void playConfirmMelody()
{
  if (true == conf.volume) {
    toneAC(700, 500);
  }
}

void resetTimer(unsigned long &timer)
{
  timer = millis();
}

void renderChrono(Stat value = stat)
{
  uint16_t s = (value.chrono == 0 && value.chrono_start != 0) ? rtc.now().unixtime() - value.chrono_start : value.chrono;
  uint8_t h = floor(s / 3600);
  s -= h * 3600;
  uint8_t m = floor(s / 60);
  s -= m * 60;

  u8g.print(h);
  u8g.print(F(":"));
  renderZero(m);
  u8g.print(m);
  u8g.print(F(":"));
  renderZero((int)s);
  u8g.print(s);
}

void renderVario()
{
  if (true == varioState) {

    DateTime now = rtc.now();
    display.clearDisplay();

    //ALtitude
    display.setTextColor(BLACK);
    display.setTextSize(2);
    u8g.print((int)Altitude);
    display.setTextSize(1);
    u8g.print(F("m"));

    //Time
    u8g.setPrintPos(55, 0);
    renderZero(now.hour());
    u8g.print(now.hour());
    u8g.setPrintPos(66, 0);
    u8g.print(F(":"));
    u8g.setPrintPos(72, 0);
    renderZero(now.minute());
    u8g.print(now.minute());

    //Chrono
    display.setCursor(0, 41);
    u8g.print(F("M"));
    u8g.print(conf.stat_index + 1);
    u8g.print(F(" "));
    renderChrono();

    if (now.second() % 2 == 0) {
      //Battery level
      uint8_t vccPixels = getVccPixels();
      uint8_t bat_x = 72;
      uint8_t bat_y = 9;
      display.drawRect(bat_x + 2, bat_y, 10, 6, BLACK);
      display.fillRect(bat_x, bat_y + 2, 2, 2, BLACK);
      display.fillRect(bat_x + 3 + 8 - vccPixels, bat_y + 1, vccPixels, 4, BLACK);
    }
    else {
      // set up my_temperature
      display.setCursor(62, 9);
      bmp085.getTemperature(&my_temperature);
      u8g.print((int)my_temperature);
      u8g.drawPixel(75, 9, BLACK);
      u8g.drawPixel(76, 9, BLACK);
     u8g.drawPixel(74, 10, BLACK);
      u8g.drawPixel(74, 11, BLACK);
      u8g.drawPixel(75, 12, BLACK);
      u8g.drawPixel(76, 12, BLACK);
      u8g.drawPixel(77, 10, BLACK);
      u8g.drawPixel(77, 11, BLACK);
      u8g.setCursor(79, 9);
      u8g.print(F("C"));
    }

    //Vario
    display.setTextColor(WHITE, BLACK);
    display.setCursor(48, 24);
    display.print(F("m/s"));
    display.setTextSize(2);
    display.setCursor(0, 16);

    float vario_abs = abs(vario);
    display.print((vario <= -0.05) ? F("-") : (vario >= 0.05) ? F("+") : F(" "));
    uint8_t m = floor(vario_abs);
    display.print(m);
    display.print(F("."));
    display.print(round(10 * vario_abs) - (10 * m));

    //vario bar
    if (vario >= 0)
      display.fillRect(42, 32, round(abs(vario) * 10), 8, BLACK);
    else
      display.drawRect(42, 32, -round(abs(vario) * 10), 8, BLACK);

    display.display();
  }
}


void renderVolume(bool volume)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);

  conf.volume = volume;

  push_write_eeprom = 0;
  resetTimer(get_time1); //stop the refresh rendering vario

  display.println(F("Sons:"));
  (false == conf.volume) ? display.print(F("Off")) : display.print(F("On"));
  display.display();
  //playConfirmMelody();
}

float updateConfItem(float var, uint8_t dir = 2, float increment = 1)
{
  // save the configuration if a parameter is validate.
  if (menuUsed_last == true && menuUsed == false) {
    menuUsed_last = false;
    EEPROM_writeAnything(0, conf);
  }
  // otherwise, update the parameter.
  else if (menuUsed) {
    menuUsed_last = menuUsed;
    if  (dir == MENU_RIGHT)
      var += increment;
    else if (dir == MENU_LEFT)
      var -= increment;
  }
  return var;
}

void renderStatItem(float value, const __FlashStringHelper *unit, bool integer = false)
{
  menuUsed = false;
  display.setTextColor(BLACK);

  if (true == integer) {
    display.print((int)value);
  }
  else {
    display.print(value, 1);
  }
  display.print(unit);
}

void renderZero(int value)
{
  if (value <  10)
    display.print(F("0"));
}


void renderDateTime(DateTime d, uint8_t bold = 0)
{
  //display date
  if (bold == DATE_DAY)
    display.setTextColor(WHITE, BLACK);
  renderZero(d.day());
  display.print(d.day());
  
  display.setTextColor(BLACK);
  display.print(F("/"));

  if (bold == DATE_MONTH)
    display.setTextColor(WHITE, BLACK);
  renderZero(d.month());
  display.print(d.month());

  display.setTextColor(BLACK);
  display.print(F("/"));

  if (bold == DATE_YEAR)
    display.setTextColor(WHITE, BLACK);
  renderZero(d.year() - 2000);
  display.print(d.year() - 2000); //reduces the length of the year string

  display.setTextColor(BLACK);
  display.print(F(" "));

  //display time
  if (bold == DATE_HOUR)
    display.setTextColor(WHITE, BLACK);
  renderZero(d.hour());
  display.print(d.hour());

  display.setTextColor(BLACK);
  display.print(F(":"));

  if (bold == DATE_MINUTE)
    display.setTextColor(WHITE, BLACK);
  renderZero(d.minute());
  display.print(d.minute());

  //display.setTextColor(BLACK);
}

void renderMenu(MenuItem newMenuItem = menu.getCurrent(), uint8_t dir = 2)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);

  if (newMenuItem.getShortkey() != MENU_LEFT) {
    if (newMenuItem.getShortkey() < 10)
      display.println(F("Accueil"));
    else if (newMenuItem.getShortkey() >= 10 && newMenuItem.getShortkey() < 20)
      display.println(F("Options"));
    else if (newMenuItem.getShortkey() == MENU_RECRESET)
      display.println(F("Statistiques"));
  }

  if (newMenuItem.getShortkey() != MENU_STAT) {

    display.setTextSize(2);
    display.println(newMenuItem.getName());

    if (!menuUsed)
      display.setTextColor(BLACK);
  }

  if (varioState == true && menuUsed) {
    varioState = false;
    menuUsed = false;
  }

  //sous-menu (valeur)
  if (varioState == false) {

    switch (newMenuItem.getShortkey()) {

      case MENU_VARIO:
        {
          if (menuUsed) {
            varioState = true;
            menuUsed = false;
          }
        }
        break;

      case MENU_TARE:
        {
          if (menuUsed) {
            menuUsed = false;
            display.print(F("Ok"));

            sensors_event_t event;
            bmp085.getEvent(&event);
            conf.p0 = event.pressure;

            //prevent chrono start and beeping
            resetAltitudeSensor();

            EEPROM_writeAnything(0, conf);
          }
        }
        break;

      case MENU_QNH:
        {
          conf.p0 = updateConfItem(conf.p0, dir, 1);
          //prevent chrono start and beeping
          resetAltitudeSensor();

          display.print(conf.p0);
          display.setTextSize(1);
          display.print(F("hPa"));
        }
        break;

      case MENU_ALTITUDE:
        {
          conf.currentAltitude = updateConfItem(conf.currentAltitude, dir, 5);
          //prevent chrono start and beeping
          resetAltitudeSensor();

          display.print(conf.currentAltitude);
          display.setTextSize(1);
          display.print(F("m"));
        }
        break;


      case MENU_MONTEE:
        {
          conf.vario_climb_rate_start = updateConfItem(conf.vario_climb_rate_start, dir, 1);

          if (conf.vario_climb_rate_start < 1) {
            conf.vario_climb_rate_start = 0;
            display.print(F("Off"));
          }
          else {
            display.print(getVarioClimbRateStart(), 1);
            display.setTextSize(1);
            display.print(F("m/s"));
          }
        }
        break;

      case MENU_DESCENTE:
        {
          conf.vario_sink_rate_start = updateConfItem(conf.vario_sink_rate_start, dir, 1);

          if (conf.vario_sink_rate_start >= 0) {
            conf.vario_sink_rate_start = 0;
            display.print(F("Off"));
          }
          else {
            display.print(getVarioSinkRateStart(), 1);
            display.setTextSize(1);
            display.print(F("m/s"));
          }
        }
        break;

      case MENU_LIGHT:
        {
          conf.light_cpt = updateConfItem(conf.light_cpt, dir, -1);

          if  (conf.light_cpt <= 0)
            conf.light_cpt = 0;

          if (conf.light_cpt >= 5) {
            conf.light_cpt = 5;
            display.print(F("Off"));
          }
          else {
            display.print(5 - conf.light_cpt);
          }
          updateBrightness();
        }
        break;

      case MENU_CONTRASTE:
        {
          conf.contrast_default = updateConfItem(conf.contrast_default, dir, 1);

          if (conf.contrast_default <= 0) {
            conf.contrast_default = 0;
          }
          else if (conf.contrast_default >= 100) {
            conf.contrast_default = 100;
          }

          display.print(conf.contrast_default);
          display.setContrast(conf.contrast_default);
        }
        break;

      case MENU_DATE:
        {
          display.setTextSize(1);

          if (menuUsed_last == false) {

            menuUsed_last = true;

            DateTime now = rtc.now();
            renderDateTime(now);

            date_year = now.year();
            date_month = now.month();
            date_day = now.day();
            date_hour = now.hour();
            date_minute = now.minute();
          }
          else {

            if  (dir != MENU_RIGHT && dir != MENU_LEFT) {

              if (conf_date_displayed < 5) {

                menuUsed_last = true;
                menuUsed = true;
                conf_date_displayed++;
              }
              else {
                menuUsed = false;
                menuUsed_last = true;
                conf_date_displayed = 0;
                rtc.adjust(DateTime(date_year, date_month, date_day, date_hour, date_minute, 0));
                display.setTextColor(BLACK);
                renderDateTime(rtc.now());
              }
            }

            switch (conf_date_displayed) {

              case DATE_DAY:
                if (dir == MENU_RIGHT) (date_day >= 31) ? date_day = 1 : date_day++;
                if (dir == MENU_LEFT) (date_day == 1) ? date_day = 31 : date_day--;
                //display.println(F("Jour"));
                break;
              case DATE_MONTH:
                if (dir == MENU_RIGHT) (date_month >= 12) ? date_month = 1 : date_month++;
                if (dir == MENU_LEFT) (date_month == 1) ? date_month = 12 : date_month--;
                //display.println(F("Mois"));
                break;
              case DATE_YEAR:
                if (dir == MENU_RIGHT) (date_year >= 2099) ? date_year = 2000 : date_year++;
                if (dir == MENU_LEFT) (date_year == 2000) ? date_year = 2099 : date_year--;
                //display.println(F("Annee"));
                break;
              case DATE_HOUR:
                if (dir == MENU_RIGHT) (date_hour >= 23) ? date_hour = 0 : date_hour++;
                if (dir == MENU_LEFT) (date_hour == 0) ? date_hour = 23 : date_hour--;
                //display.println(F("Heure"));
                break;
              case DATE_MINUTE:
                if (dir == MENU_RIGHT) (date_minute >= 59) ? date_minute = 0 : date_minute++;
                if (dir == MENU_LEFT)(date_minute == 0) ? date_minute = 59 : date_minute--;
                //display.println(F("Minute"));
                break;
            }

            display.setTextColor(BLACK);
            renderDateTime(DateTime(date_year, date_month, date_day, date_hour, date_minute, 0), conf_date_displayed);
            //display.print(date_conf[conf_date_displayed]);
          }
        }
        break;

      case MENU_STAT:
        {
          menuUsed = false;
          Stat stat_to_display;
          readStat(stat_displayed - 1, stat_to_display);

          if (stat_to_display.chrono == 0) {
            display.print(F("M"));
            display.print(stat_displayed);
            display.print(F(" vide"));
          }
          else {

            if (!stat_blink_status) {                          
              display.setTextColor(BLACK);
              renderDateTime(DateTime(stat_to_display.chrono_start));
            }
            else {
              display.print(F("M"));
              display.print(stat_displayed);              
              display.setTextColor(BLACK);
            }
            
            stat_blink_status = !stat_blink_status;
            display.println();

            display.print(F("Chrono:"));
            renderChrono(stat_to_display);
            display.println();

            display.print(F("Alt max:"));
            renderStatItem(stat_to_display.alti_max, F("m"), true);
            display.println();

            display.print(F("Alt min:"));
            renderStatItem(stat_to_display.alti_min, F("m"), true);
            display.println();

            display.print(F("m/s:"));
            renderStatItem(-(float)stat_to_display.txchutemax / 10, F("|"));
            renderStatItem((float)stat_to_display.txchutemin / 10, F(""));
            display.println();

            display.print(F("Cumul:"));
            renderStatItem(round(stat_to_display.cumul_alt), F("m"), true);
            display.println();
          }
        }
        break;

      case MENU_RECRESET:
        {
          if (menuUsed) {
            menuUsed = false;
            resetAllStats();
            display.setTextColor(BLACK);
            display.print(F("Ok"));
          }
        }
        break;
    }
  }
  display.display();
}

//this function builds the menu and connects the correct items together
void menuSetup()
{
  m_vario.name = F("Vario"); //Vario
  m_options.name = F("Options"); //Options
  m_stats.name = F("Stats"); //Stats
  m_retour.name = F("Retour"); //Retour
  m_tare.name = F("Tare"); //Tare
  m_qnh.name = F("QNH"); //QNH
  m_altitude.name = F("Alti"); //Altitude
  m_montee.name = F("Montee"); //Montée
  m_descente.name = F("Desc"); //Descente
  m_light.name = F("Light"); //eclairage
  m_contrast.name = F("Contra"); //eclairage
  m_date.name = F("Date"); //date
  m_retour2.name = F("Retour"); //Retour
  m_recreset.name = F("Reset"); //Reset records

  /*
  	This is the structure of the modelled menu

          Vario
          Stats
                  Retour
                  Statistiques
                  Reset
          Options
                  Retour
                  Tare
                  Altitude
                  Montee
                  Descente
                  Light
                  Contra
                  Date

  */
  m_vario.addAfter(m_stats);
  m_stats.addAfter(m_options);
  m_stats.addRight(m_stat);
  m_stat.addBefore(m_retour2);
  m_stat.addAfter(m_recreset);
  m_retour2.addLeft(m_vario);

  m_options.addRight(m_tare);
  m_tare.addBefore(m_retour);
  m_tare.addAfter(m_qnh);
  m_qnh.addAfter(m_altitude);
  m_altitude.addAfter(m_montee);
  m_montee.addAfter(m_descente);
  m_descente.addAfter(m_light);
  m_light.addAfter(m_contrast);
  m_contrast.addAfter(m_date);
  m_retour.addLeft(m_vario);

  menu.use(m_vario);
}

/*
  Here all menu use events are handled
*/
void menuUseEvent(MenuUseEvent used)
{
  if (used.item.getShortkey() == MENU_RIGHT) {
    menu.moveRight();
  }
  else if (used.item.getShortkey() == MENU_LEFT) {
    menu.moveLeft();
  }
  else
    menuUsed = !menuUsed;

  renderMenu();
}

/*
  That is when the user changes the menu
*/
void menuChangeEvent(MenuChangeEvent changed)
{
  renderMenu(changed.to);
}

/*
  screen brightness. AnalogWrite values from 0 to 255
*/
void updateBrightness()
{
  analogWrite(PIN_LIGHT, conf.light_cpt * 51);
}


uint8_t getVccPixels()
{
  uint16_t real_bat = (int)(4.89 * analogRead(A0));
  //Serial.println(analogRead(A0));
  //Serial.println(real_bat);
  average_vcc = (average_vcc == 0) ? real_bat : (int)(average_vcc * 0.94 + real_bat * 0.06);

  uint8_t pixels = map(average_vcc, 3350, 4200, 0, 8);
  if (pixels > 8)
    pixels = 8;
  else if (pixels < 1)
    pixels = 1;

  return pixels;
}


uint8_t getBeepLatency(float variation)
{
  int latency = 150 - (variation * 30);
  return (latency < 70) ? 70 : (latency > 150) ? 150 : latency;
}


uint16_t getBeepFrequency(float variation)
{
  int frequency = 690 + (150 * variation);
  return (frequency < 100) ? 100 : (frequency > 1300) ? 1300 : frequency;
}


void updateAltitude(bool reset = false)
{
  // get a new sensor event
  sensors_event_t event;
  bmp085.getEvent(&event);
  // put it in smooth filter and take average
  average_pressure = (true == reset) ? event.pressure : average_pressure * 0.95 + event.pressure * 0.05;
  // take new altitude in meters
  Altitude = bmp085.pressureToAltitude(conf.p0, average_pressure) + conf.currentAltitude;
}


void resetAltitudeSensor()
{
  updateAltitude(true);
  altitude_temp = Altitude;
  alt = Altitude;
  tim = millis();
}

/*
  make some beeps...
*/
void makeBeeps()
{
  if (true == conf.volume && (millis() >= (get_timeBeep + beepLatency) || timeNoPauseBeep <= 40))
  {
    resetTimer(get_timeBeep);

    noSound = (timeNoPauseBeep <= 40) ? false : !noSound;

    if (false == noSound) {

      float varioClimbRateStart = getVarioClimbRateStart();
      float varioSinkRateStart = getVarioSinkRateStart();

      //beep even if vario has negative value but vario is climbing
      float variation = (vario < varioClimbRateStart && vario_diff >= varioClimbRateStart && varioClimbRateStart != 0) ? vario_diff : vario;

      if (timeNoPauseBeep <= 40) {
        timeNoPauseBeep++;
        beepLatency = 150;
      }
      else {
        beepLatency = getBeepLatency(variation);
      }

      if ((vario > varioClimbRateStart && varioClimbRateStart != 0) || (vario < varioSinkRateStart && varioSinkRateStart != 0)) {
        //when climbing make faster and shorter beeps
        //toneAC(getBeepFrequency(variation), 10, beepLatency, true);
        toneAC(getBeepFrequency(variation), beepLatency);
      }
      else {
        toneAC(0);
        timeNoPauseBeep = 0;
      }
    } else {
      beepLatency = beepLatency / 2;
    }
  }
}


void setup()
{
  //Serial.begin(9600);

  // clear the configuration
  if (initialisation) {
    EEPROM_writeAnything(0, conf);
    resetAllStats();
  }

  // load the configuration
  EEPROM_readAnything(0, conf);
  readStat();

  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(PIN_LIGHT, OUTPUT);

  bmp085.begin();
  resetAltitudeSensor();

  display.begin();
  display.setContrast(conf.contrast_default);
  display.setTextWrap(false);
  //display.setRotation(0);

  menuSetup();
  updateBrightness();
}


void loop()
{
  readButtons();
  updateAltitude();
  
  float tempo = micros();
  
  /* TEST BLOC */
  //Altitude = alt + 0.05;  
  
  // put it in smooth filter and take average
  vario = vario * 0.8 + (200000 * ((Altitude - alt) / (tempo - tim)));

  /* TEST BLOC */
  /*
  vario = vario + 0.005;
  if (vario > 4)
    vario = 0;
  */

  // Update stats if chrono is running
  if (stat.chrono_start != 0) {

    if (vario > 0)
      cumul_alt += Altitude - alt;

    if (Altitude > stat.alti_max)
      stat.alti_max = Altitude;
    if (Altitude < stat.alti_min)
      stat.alti_min = Altitude;

    int decivario = (int)(vario * 10);
    if (decivario < -stat.txchutemax)
      stat.txchutemax = -decivario;
    if (decivario > stat.txchutemin)
      stat.txchutemin = decivario;
  }

  alt = Altitude;
  tim = tempo;

  // make some beeps...
  makeBeeps();

  //every second
  if (millis() >= (get_time1 + 1000))
  {
    resetTimer(get_time1);
    
    if (menu.getCurrent().getShortkey() == MENU_STAT) {
      menu.use();
    }
    
    // proceedings of the dynamic display of vario
    renderVario();

    // diff with previous variometer measurement
    vario_diff = vario - vario_old;
    vario_old = vario;

    // eeprom saves after 5 seconds
    if (push_write_eeprom == 5) {
      push_write_eeprom++;
      EEPROM_writeAnything(0, conf);
    }
    else if (push_write_eeprom < 5)
      push_write_eeprom++;

    // in vario, if button enter is pressed 1 seconds, reset stats
    if (is_vario_button_push == true) {

      is_vario_button_push = false;
      resetStat();

      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(0, 41);
      display.fillRect(0, 41, 84, 7, WHITE);
      resetTimer(get_time1);  //stop the refresh rendering vario
      display.print(F("R.A.Z. stat M"));
      display.print(conf.stat_index + 1);
      display.display();
    }


    // if the altitude out of his "zone", the timer is started
    if (stat.chrono_start == 0) {
      if (altitude_temp + ALTI_TRIGGER < Altitude || altitude_temp - ALTI_TRIGGER > Altitude) {
        resetStat();
        DateTime now = rtc.now();
        stat.chrono_start = now.unixtime();
        chrono_cpt = 0;
        cumul_alt = 0;
      }
      else { // every 15 seconds, the altitude "zone" is updated
        chrono_cpt++;
        if (chrono_cpt >= 15) {
          chrono_cpt = 0;
          altitude_temp = Altitude;
        }
      }
    }
    if (stat.chrono_start != 0 && stat.chrono == 0) {
      // if altitude left in the same "zone" (1.5 meter) during 15 seconds, the timer is stopped
      if (altitude_temp - 0.75 < Altitude && altitude_temp + 0.75 > Altitude) {
        chrono_cpt++;
        if (chrono_cpt >= 15) {
          DateTime now = rtc.now();
          stat.chrono = now.unixtime() - stat.chrono_start;
          stat.cumul_alt = (int)cumul_alt;
          writeStat();
          incrementStatIndex();
        }
      }
      else {
        chrono_cpt = 0;
        altitude_temp = Altitude;
      }
    }
    //correction beep latency
    makeBeeps();
  }
  //Serial.println((tempo - micros()));
}

void readButtons()
{
  long newKnobPosition = knob.read();
  if (newKnobPosition != knobPosition) {
    //Serial.print(newKnobPosition);
    //Serial.print(F(" / "));
    //Serial.println(knobPosition);
    if (abs(newKnobPosition - knobPosition) >= ENCODER_STEP) {
      if (newKnobPosition > knobPosition) { //Right
        if (!menuUsed && varioState == false) {
          if (menu.getCurrent().getShortkey() == MENU_STAT && stat_displayed < NB_STATS) {
            resetTimer(get_time1);
            stat_blink_status = true;
            stat_displayed++;
            renderMenu();
          }
          else
            menu.moveDown();
        }
        else if (varioState == false)
          renderMenu(menu.getCurrent(), MENU_RIGHT);
        else if (varioState == true)
          renderVolume(true);
      }
      else { //Left
        if (!menuUsed && varioState == false) {
          if (menu.getCurrent().getShortkey() == MENU_STAT && stat_displayed > 1) {
            resetTimer(get_time1);
            stat_blink_status = true;
            stat_displayed--;
            renderMenu();
          }
          else
            menu.moveUp();
        }
        else if (varioState == false)
          renderMenu(menu.getCurrent(), MENU_LEFT);
        else if (varioState == true)
          renderVolume(false);
      }
      knobPosition = newKnobPosition;
    }
  }

  //if button enter is pressed
  int reading = digitalRead(Enter);
  if (reading == LOW && lastEnterState == HIGH) {

    // in menu, clic an item
    if (varioState == false) {
      get_time1 = millis() - 1000;
      menu.use();
    }
    // in vario, button enter init timer
    else {
      resetTimer(get_time1);
      is_vario_button_push = true;
    }
  }

  // in vario, stop button enter and go back to menu
  if (reading == HIGH && lastEnterState == LOW && varioState == true && is_vario_button_push == true) {
    is_vario_button_push = false;
    menu.use();
  }

  lastEnterState = reading;

}//end read button
