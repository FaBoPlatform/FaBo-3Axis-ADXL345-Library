/**
 @file FaBo3Axis_ADXL345.cpp
 @brief This is a library for the FaBo 3AXIS I2C Brick.

   http://fabo.io/201.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "FaBo3Axis_ADXL345.h"

/**
 @brief Constructor
*/
FaBo3Axis::FaBo3Axis(uint8_t addr)
{
  _i2caddr = addr;
}

/**
 * @brief set sda, scl pin
 * @param [in] sda : the number of sda pin
 * @param [in] scl : the number of scl pin
 */
void FaBo3Axis::initPin(int sda, int scl){
  #ifdef __arm__
    Wire.setSDA(sda);
    Wire.setSCL(scl);
  #endif
}

/**
 * @brief Search ADXL345 Device
 * @retval true  : Found
 * @retval false : Not Found
 */
bool FaBo3Axis::searchDevice()
{
  Wire.begin();
  uint8_t device = 0x00;
  readI2c(ADXL345_DEVID_REG, 1, &device);

  if(device == ADXL345_DEVICE){
    return true;
  } else{
    return false;
  }
}

/**
 * @brief Set Config
 */
void FaBo3Axis::configuration()
{

  uint8_t conf = ADXL345_SELF_TEST_OFF;
  conf |= ADXL345_SPI_OFF;
  conf |= ADXL345_INT_INVERT_OFF;
  conf |= ADXL345_FULL_RES_OFF;
  conf |= ADXL345_JUSTIFY_OFF;
  conf |= ADXL345_RANGE_2G;

  writeI2c(ADXL345_DATA_FORMAT_REG, conf);

  writeI2c(ADXL345_BW_RATE_REG, 0b1110);
}

/**
 * @brief Power ON
 */
void FaBo3Axis::powerOn()
{
  uint8_t power = ADXL345_AUTO_SLEEP_OFF;
  power |= ADXL345_MEASURE_ON;
  power |= ADXL345_SLEEP_OFF;
  power |= ADXL345_WAKEUP_8HZ;

  writeI2c(ADXL345_POWER_CTL_REG, power);
}

/**
 * @brief set data rate
 * @param [in] rate: date rate 
 */
void FaBo3Axis::setRate(uint8_t rate)
{
  writeI2c(ADXL345_BW_RATE_REG, rate);
}

/**
 * @brief Read ADXL345
 * @param [out] *x : X-axis Data
 * @param [out] *y : Y-axis Data
 * @param [out] *z : Z-axis Data
 */
void FaBo3Axis::readXYZ(int *x, int *y, int *z)
{
  uint8_t length = 6;
  uint8_t axis_buff[6];
  readI2c(0x32, length, axis_buff);
 
  if(axis_buff[1] == 0) {
    *x = axis_buff[0];
  } else {
    *x = - axis_buff[0];
  }
  
  if(axis_buff[3] == 0) {
    *y = axis_buff[2];
  } else {
    *y = - axis_buff[2];
  }
  
  if(axis_buff[5] == 0) {
    *z = axis_buff[4];
  } else {
    *z = - axis_buff[4];
  }
}

/**
 * @brief Read interrupts Status
 * @return byte : interrupts Status
 */
uint8_t FaBo3Axis::readIntStatus()
{
  uint8_t buff;
  readI2c(ADXL345_INT_SOURCE_REG, 1, &buff);

  return buff;
}

/**
 * @brief Enable Tap
 */
void FaBo3Axis::enableTap()
{
  writeI2c(ADXL345_THRESH_TAP_REG, 0x32); // 62.5mg/LBS
  writeI2c(ADXL345_DUR_REG, 0x0f); // 1.25ms/LSB
  writeI2c(ADXL345_LATENT_REG, 0x78); // 1.25ms/LSB
  writeI2c(ADXL345_WINDOW_REG, 0xcb); // 1.25ms/LSB

  uint8_t int_tap = ADXL345_INT_SINGLE_TAP | ADXL345_INT_DOUBLE_TAP;
  writeI2c(ADXL345_INT_ENABLE_REG, int_tap); // Interrupts Tap Enable

  writeI2c(ADXL345_TAP_AXES_REG, ADXL345_TAP_AXES_Z_ON); // Tap Enable z axis
}

/**
 * @brief Single Tap
 * @param [in] value : interrupts Status
 * @retval true  : Tap
 * @retval false : Not Tap
 */
bool FaBo3Axis::isSingleTap(uint8_t value)
{
  if((value & 0b01000000) == 0b01000000){
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Double Tap
 * @param [in] value : interrupts Status
 * @retval true  : Double Tap
 * @retval false : Not Double Tap
 */
bool FaBo3Axis::isDoubleTap(uint8_t value)
{
  if((value & 0b00100000) == 0b00100000){
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Write I2C Data
 * @param [in] register_addr : Write Register Address
 * @param [in] value  : Write Data
 */
void FaBo3Axis::writeI2c(uint8_t register_addr, uint8_t value)
{
  Wire.beginTransmission(_i2caddr);
  Wire.write(register_addr);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 * @brief Read I2C Data
 * @param [in] register_addr : register address
 * @param [in] num   : Data Length
 * @param [out] *buf : Read Data
 */
void FaBo3Axis::readI2c(uint8_t register_addr, uint8_t num, uint8_t buffer[])
{
  Wire.beginTransmission(_i2caddr);
  Wire.write(register_addr);
  Wire.endTransmission();

  Wire.requestFrom(_i2caddr, num);

  int i = 0;
  while(Wire.available())
  {
    buffer[i] = Wire.read();
    i++;
  }
}
