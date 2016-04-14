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
FaBo3Axis::FaBo3Axis()
{
  Wire.begin();
}

/**
 * @brief Search ADXL345 Device
 * @retval true  : Found
 * @retval false : Not Found
 */
bool FaBo3Axis::searchDevice()
{

  byte device = 0x00;
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

  byte conf = ADXL345_SELF_TEST_OFF;
  conf |= ADXL345_SPI_OFF;
  conf |= ADXL345_INT_INVERT_OFF;
  conf |= ADXL345_FULL_RES_OFF;
  conf |= ADXL345_JUSTIFY_OFF;
  conf |= ADXL345_RANGE_16G;

  writeI2c(ADXL345_DATA_FORMAT_REG, conf);
}

/**
 * @brief Power ON
 */
void FaBo3Axis::powerOn()
{
  byte power = ADXL345_AUTO_SLEEP_OFF;
  power |= ADXL345_MEASURE_ON;
  power |= ADXL345_SLEEP_OFF;
  power |= ADXL345_WAKEUP_8HZ;

  writeI2c(ADXL345_POWER_CTL_REG, power);
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
  byte axis_buff[6];
  readI2c(0x32, length, axis_buff);
  *x = (((int)axis_buff[1]) << 8) | axis_buff[0];
  *y = (((int)axis_buff[3]) << 8) | axis_buff[2];
  *z = (((int)axis_buff[5]) << 8) | axis_buff[4];
}

/**
 * @brief Read interrupts Status
 * @return byte : interrupts Status
 */
byte FaBo3Axis::readIntStatus()
{

  byte buff;
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

  byte int_tap = ADXL345_INT_SINGLE_TAP | ADXL345_INT_DOUBLE_TAP;
  writeI2c(ADXL345_INT_ENABLE_REG, int_tap); // Interrupts Tap Enable

  writeI2c(ADXL345_TAP_AXES_REG, ADXL345_TAP_AXES_Z_ON); // Tap Enable z axis


}

/**
 * @brief Single Tap
 * @param [in] value : interrupts Status
 * @retval true  : Tap
 * @retval false : Not Tap
 */
bool FaBo3Axis::isSingleTap(byte value)
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
bool FaBo3Axis::isDoubleTap(byte value)
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
void FaBo3Axis::writeI2c(byte register_addr, byte value)
{
  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS);
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
void FaBo3Axis::readI2c(byte register_addr, int num, byte buffer[])
{
  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS);
  Wire.write(register_addr);
  Wire.endTransmission();

  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS);
  Wire.requestFrom(ADXL345_SLAVE_ADDRESS, num);

  int i = 0;
  while(Wire.available())
  {
    buffer[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
}
