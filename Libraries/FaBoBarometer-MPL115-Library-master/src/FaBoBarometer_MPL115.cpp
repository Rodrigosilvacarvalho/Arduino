/**
 @file FaBoBarometer_MPL115.cpp
 @brief This is a library for the FaBo Barometer I2C Brick.

   http://fabo.io/204.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "FaBoBarometer_MPL115.h"

/**
 @brief Constructor
*/
FaBoBarometer::FaBoBarometer(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

/**
 @brief Begin Device
*/
void FaBoBarometer::begin() {
  readCoef();
}

/**
 @brief Read Coefficient Data
*/
void FaBoBarometer::readCoef() {
  uint8_t data[8];
  readI2c(MPL115A2_A0_MSB, 8, data);
  _a0  = ((float) ((data[0] << 8) + data[1]) / ((long)1 << 3));
  _b1  = ((float) ((data[2] << 8) + data[3]) / ((long)1 << 13));
  _b2  = ((float) ((data[4] << 8) + data[5]) / ((long)1 << 14));
  _c12 = ((float) ((data[6] << 8) + data[7]) / ((long)1 << 24));

//   Serial.println(_a0,32);
//   Serial.println(_b1,32);
//   Serial.println(_b2,32);
//   Serial.println(_c12,32);
}

/**
 @brief Read hpa
 @param [in] altitude Altitude
*/
float FaBoBarometer::readHpa(float altitude) {
  float hpa,temp;
  readData(&hpa,&temp);
  return hpa / pow(1.0 - (altitude/44330.0), 5.255);
}

/**
 @brief Read temperature
*/
float FaBoBarometer::readTemp() {
  float hpa,temp;
  readData(&hpa,&temp);
  return temp;
}

/**
 @brief Read Measurement data
 @param [out] hpa hpa
 @param [out] temp temp
*/
void FaBoBarometer::readData(float * hpa, float * temp) {
  uint8_t data[4];
  uint16_t padc,tadc;
  float pcomp;
  writeI2c(MPL115A2_CONVERT, 0x01);
  delay(3);
  readI2c(MPL115A2_PADC_MSB, 4, data);
  padc = (((uint16_t)data[0] << 8) | data[1]) >> 6;
  tadc = (((uint16_t)data[2] << 8) | data[3]) >> 6;

  pcomp = _a0 + ( _b1 + _c12 * tadc ) * padc + _b2 * tadc;
  *hpa  = pcomp * ( (1150.0-500.0)/1023.0 ) + 500.0;
  *temp = 25.0 - ((float)tadc - 512.0 ) / 5.35;
}


/**
 @brief Write I2C
 @param [in] address register address
 @param [in] data write data
*/
void FaBoBarometer::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 @brief Read I2C
 @param [in] address register address
 @param [in] num read length
 @param [out] data read data
*/
void FaBoBarometer::readI2c(uint8_t address, uint8_t num, uint8_t * data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  uint8_t i = 0;
  Wire.requestFrom(_i2caddr, num);
  while( Wire.available() ) {
    data[i++] = Wire.read();
  }
}
