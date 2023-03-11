/**
 @file FaBoBarometer_MPL115.h
 @brief This is a library for the FaBo Barometer I2C Brick.

   http://fabo.io/204.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef FABOBAROMETER_MPL115_H
#define FABOBAROMETER_MPL115_H

#include <Arduino.h>
#include <Wire.h>


#define MPL115A2_SLAVE_ADDRESS 0x60 ///< MPL115A2 I2C slave address

/// @name MPL115A2 Register Address
/// @{
#define MPL115A2_PADC_MSB 0x00
#define MPL115A2_PADC_LSB 0x01
#define MPL115A2_TADC_MSB 0x02
#define MPL115A2_TACD_LSB 0x03
#define MPL115A2_A0_MSB 0x04
#define MPL115A2_A0_LSB 0x05
#define MPL115A2_B1_MSB 0x06
#define MPL115A2_B1_LSB 0x07
#define MPL115A2_B2_MSB 0x08
#define MPL115A2_B2_LSB 0x09
#define MPL115A2_C12_MSB 0x0A
#define MPL115A2_C12_LSB 0x0B
#define MPL115A2_CONVERT 0x12
/// @}


/**
 * @class FaBoBarometer
 * @brief FaBo Barometer I2C Controll class
 */
class FaBoBarometer {
  public:
    FaBoBarometer(uint8_t addr = MPL115A2_SLAVE_ADDRESS);
    void begin(void);
    void readCoef(void);
    void readData(float * hpa, float * temp);
    float readHpa(float altitude = 0.0);
    float readTemp(void);
  private:
    uint8_t _i2caddr;
    float _a0, _b1, _b2, _c12;
    void writeI2c(uint8_t address, uint8_t data);
    void readI2c(uint8_t address, uint8_t num, uint8_t * data);
};

#endif // FABOBAROMETER_MPL115_H
