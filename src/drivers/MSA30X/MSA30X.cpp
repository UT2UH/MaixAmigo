/**************************************************************************/
/*!
    @file     MSA30X.cpp
    
    @mainpage MSA30X 14-bit digital accelometer library

    @section author Author   
    Joel Lavikainen
    
    @section license License 
    BSD
    
    @section intro_sec Introduction 
    Based on Adafruit Accelerometer library code
*/
/**************************************************************************/
// #include "Arduino.h"
// #include <Wire.h>

#include <limits.h>

#include "MSA30X.h"

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register.
    @param  reg
            Register address
    @param  value  
    Byte value to be written
*/
/**************************************************************************/
void MSA30X::writeRegister(uint8_t reg, uint8_t value) 
{
    Wire.beginTransmission(MSA_ADDR);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)(value));
    Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
    @param  reg
            Address of register
    @return Reply byte
*/
/**************************************************************************/
uint8_t MSA30X::readRegister(uint8_t reg) 
{
    Wire.beginTransmission(MSA_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MSA_ADDR, 1);
    uint8_t data =Wire.read();
    return data;
}

/**************************************************************************/
/*!
    @brief  Reads 16-bits from the specified register
    @param  reg
            Register address
    @return 16-bits of data
*/
/**************************************************************************/
int16_t MSA30X::read16(uint8_t reg) 
{
    Wire.beginTransmission(MSA_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MSA_ADDR, 2);
    return (uint16_t)(((Wire.read()&0xff) | (uint16_t)(Wire.read()&0xff) << 8));  
}

/**************************************************************************/
/*! 
    @brief  Read the part ID (can be used to check connection)
    @return Part ID
*/
/**************************************************************************/
uint8_t MSA30X::getPartID(void) 
{
  // Check device ID register
  return readRegister(MSA30X_REG_PARTID);
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent X axis value
    @return X-axis acceleration data
*/
/**************************************************************************/
int16_t MSA30X::getX(void) 
{
  int16_t temp = (readRegister(MSA30X_REG_ACC_X_LSB) + (int8_t)(readRegister(MSA30X_REG_ACC_X_MSB))*256)/4;
  return temp;
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent Y axis value
    @return Y-axis acceleration data
*/
/**************************************************************************/
int16_t MSA30X::getY(void) 
{
  int16_t temp = (readRegister(MSA30X_REG_ACC_Y_LSB) + (int8_t)(readRegister(MSA30X_REG_ACC_Y_MSB))*256)/4;
  return temp;
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent Z axis value
    @return Z-axis acceleration data
*/
/**************************************************************************/
int16_t MSA30X::getZ(void) 
{
  int16_t temp = (readRegister(MSA30X_REG_ACC_Z_LSB) + (int8_t)(readRegister(MSA30X_REG_ACC_Z_MSB))*256)/4;
  return temp;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new MSA30X class
    @param  sensorID
            ID for identifying different sensors
*/
/**************************************************************************/
MSA30X::MSA30X(int32_t sensorID)
{
  _sensorID = sensorID;
  _range = MSA30X_RANGE_2_G;
}


/**************************************************************************/
/*!
    @brief  Setups the HW (reads coefficients values, etc.)
    @return True if connection was established, False if no MSA30X was
            detected
*/
/**************************************************************************/
bool MSA30X::begin() 
{
  //Wire.begin(); //Can be made in MaixAmigo

  /* Check connection */
  uint8_t partid = getPartID();
  if (partid != 0x13)
  {
    /* No MSA30X detected ... return false */
    //Serial.println(partid, HEX);
    return false;
  }
  _multiplier = MSA30X_MG2G_MULTIPLIER_4_G;
  // Enable measurements
  writeRegister(MSA30X_REG_PWR_MODE_BW, 0x10);  // Normal mode & 500 Hz Bandwidth
  writeRegister(MSA30X_REG_ODR, MSA30X_DATARATE_1000_HZ); // Set Output Data Rate to 1000 Hz
  writeRegister(MSA30X_REG_RES_RANGE, 0x01);
  writeRegister(MSA30X_REG_INT_SET_0,0x00);
  writeRegister(MSA30X_REG_INT_SET_1,0x00);
  writeRegister(MSA30X_REG_INT_LATCH,0x81);
  return true;
}

/**************************************************************************/
/*!
    @brief  Sets the g range for the accelerometer
    @param  range
            Measurement range
*/
/**************************************************************************/
void MSA30X::setRange(range_t range)
{
  /* Read the data format register to preserve bits */
  uint8_t format = readRegister(MSA30X_REG_RES_RANGE);

  /* Update the range */
  format &= ~0x3; // clear range bits
  format |= range;

  
  /* Write the register back to the IC */
  writeRegister(MSA30X_REG_RES_RANGE, format);
  
  /* Keep track of the current range (to avoid readbacks) */
  _range = range;

  /* Map correct conversion multiplier */
  switch(range) {
    case MSA30X_RANGE_16_G:
      _multiplier = MSA30X_MG2G_MULTIPLIER_16_G;
      break;
    case MSA30X_RANGE_8_G:
      _multiplier = MSA30X_MG2G_MULTIPLIER_8_G;
      break;
    case MSA30X_RANGE_4_G:
      _multiplier = MSA30X_MG2G_MULTIPLIER_4_G;
      break;
    case MSA30X_RANGE_2_G:
      _multiplier = MSA30X_MG2G_MULTIPLIER_2_G;
  }
}

/**************************************************************************/
/*!
    @brief   Get the g range for the accelerometer
    @return  Measurement range
*/
/**************************************************************************/
range_t MSA30X::getRange(void)
{
  return (range_t)(readRegister(MSA30X_REG_RES_RANGE) & 0x3);
}

/**************************************************************************/
/*!
    @brief  Sets the resolution for the accelerometer
    @param  resolution
            Measurement resolution
*/
/**************************************************************************/
void MSA30X::setResolution(res_t resolution)
{
  /* Read the data format register to preserve bits */
  uint8_t format = readRegister(MSA30X_REG_RES_RANGE);

  /* Update the resolution */
  format &= ~0xC; // clear resolution bits
  format |= resolution;

  
  /* Write the register back to the IC */
  writeRegister(MSA30X_REG_RES_RANGE, format);
  
  /* Keep track of the current resolution (to avoid readbacks) */
  _res = resolution;
}

/**************************************************************************/
/*!
    @brief  Get resolution for the accelerometer
    @return Measurement resolution
*/
/**************************************************************************/
res_t MSA30X::getResolution(void)
{
  return (res_t)(readRegister(MSA30X_REG_RES_RANGE) & 0xC);
}


/**************************************************************************/
/*!
    @brief  Sets the data rate for the MSA30X (controls power consumption)
    @param  dataRate
            Output data rate
*/
/**************************************************************************/
void MSA30X::setDataRate(dataRate_t dataRate)
{
  /* Note: The LOW_POWER bits are currently ignored and we always keep
     the device in 'normal' mode */
  writeRegister(MSA30X_REG_ODR, dataRate);
}

/**************************************************************************/
/*!
    @brief Get the data rate for the MSA30X
    @return Output data rate
*/
/**************************************************************************/
dataRate_t MSA30X::getDataRate(void)
{
  return (dataRate_t)(readRegister(MSA30X_REG_ODR) & 0x0F);
}

/**************************************************************************/
/*!
    @brief  Sets the operating mode for MSA30X
    @param  mode
            Power mode
*/
/**************************************************************************/
void MSA30X::setMode(pwrMode_t mode) 
{
  /* Read the data format register to preserve bits */
  uint8_t format = readRegister(MSA30X_REG_PWR_MODE_BW);

  /* Update the mode */
  format &= ~0xC0; // clear the mode bits
  format |= mode;

  
  /* Write the register back to the IC */
  writeRegister(MSA30X_REG_PWR_MODE_BW, format);
  
  /* Keep track of the current mode (to avoid readbacks) */
  _mode = mode;
}

/**************************************************************************/
/*!
    @brief  Gets the data rate for the MSA30X
    @return Power mode
*/
/**************************************************************************/
pwrMode_t MSA30X::getMode(void)
{
  return (pwrMode_t)(readRegister(MSA30X_REG_PWR_MODE_BW) & 0xC0);
}

/**************************************************************************/
/*!
    @brief  Reset all latched interrupts
*/
/**************************************************************************/
void MSA30X::resetInterrupt(void)
{
  /* Read register to preserve bits */
  uint8_t reg = readRegister(MSA30X_REG_INT_LATCH);

  /* Turn RESET_INT bit on */
  reg |= (1 << 7);

  /* Write the register back to the IC */
  writeRegister(MSA30X_REG_INT_LATCH, reg);
}

/**************************************************************************/
/*!
    @brief  Clear all interrupt registers by setting them to default state.
*/
/**************************************************************************/
void MSA30X::clearInterrupts(void)
{
  writeRegister(MSA30X_REG_INT_SET_0, 0x00);
  writeRegister(MSA30X_REG_INT_SET_1, 0x00);
  writeRegister(MSA30X_REG_INT_MAP_0, 0x00);
#if !defined (MSA30X_MSA301)  
  writeRegister(MSA30X_REG_INT_MAP_2_1, 0x00);
  writeRegister(MSA30X_REG_INT_MAP_2_2, 0x00);
#endif
}

/**************************************************************************/
/*!
    @brief  Check interrupt registers for all occured interrupts. Return
            struct with booleans of all triggered interrupts.
    @return Struct containing boolean status of interrupts.
*/
/**************************************************************************/
interrupt_t MSA30X::checkInterrupts(void)
{
  interrupt_t interrupts;
  uint8_t motionReg = readRegister(MSA30X_REG_MOTION_INT);
  uint8_t dataReg = readRegister(MSA30X_REG_DATA_INT);
  uint8_t tapReg = readRegister(MSA30X_REG_TAP_ACTIVE_STATUS);

  interrupts.orientInt = (motionReg >> 6) & 1;
  interrupts.sTapInt = (motionReg >> 5) & 1;
  interrupts.dTapInt = (motionReg >> 4) & 1;
  interrupts.activeInt = (motionReg >> 2) & 1;
  interrupts.freefallInt = (motionReg >> 0) & 1;
  interrupts.newDataInt = (dataReg >> 0) & 1;

  /* If there was active or tap interrupts, populate intStatus struct */
  if(interrupts.activeInt || interrupts.sTapInt || interrupts.dTapInt) {
    interrupts.intStatus.tapSign = (tapReg >> 7) & 1;
    interrupts.intStatus.tapFirstX = (tapReg >> 6) & 1;
    interrupts.intStatus.tapFirstY = (tapReg >> 5) & 1;
    interrupts.intStatus.tapFirstZ = (tapReg >> 4) & 1;
    interrupts.intStatus.activeSign = (tapReg >> 3) & 1;
    interrupts.intStatus.activeFirstX = (tapReg >> 2) & 1;
    interrupts.intStatus.activeFirstY = (tapReg >> 1) & 1;
    interrupts.intStatus.activeFirstZ = (tapReg >> 0) & 1;
  }

  return interrupts;
}
/**************************************************************************/
/*!
    @brief  Set interrupt latching mode
    @param  mode
            Interrupt mode
*/
/**************************************************************************/
void MSA30X::setInterruptLatch(intMode_t mode)
{
  /* Read register to preserve bits */
  uint8_t reg = readRegister(MSA30X_REG_INT_LATCH);

  /* Update latching mode */
  reg &= ~0xF0;
  reg |= mode;

  /* Write the register back to the IC */
  writeRegister(MSA30X_REG_INT_LATCH, reg);
}

/**************************************************************************/
/*!
    @brief  Turn on active interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  axis
            Axis to set active interrupt on
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableActiveInterrupt(axis_t axis, uint8_t interrupt) 
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_0);
      reg |= (1 << 2);
      writeRegister(MSA30X_REG_INT_MAP_0, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)  
      reg = readRegister(MSA30X_REG_INT_MAP_2_1);
      reg |= (1 << 2);
      writeRegister(MSA30X_REG_INT_MAP_2_1, reg);
#endif      
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_0);

  switch(axis) {
    case MSA30X_AXIS_X:
      reg |= (1 << 0);
      break;
    case MSA30X_AXIS_Y:
      reg |= (1 << 1);
      break;
    case MSA30X_AXIS_Z:
      reg |= (1 << 2);
      break;
  }

  writeRegister(MSA30X_REG_INT_SET_0, reg);
}

/**************************************************************************/
/*!
    @brief  Toggle freefall interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableFreefallInterrupt(uint8_t interrupt)
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_0);
      reg |= (1 << 0);
      writeRegister(MSA30X_REG_INT_MAP_0, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)  
      reg = readRegister(MSA30X_REG_INT_MAP_2_1);
      reg |= (1 << 0);
      writeRegister(MSA30X_REG_INT_MAP_2_1, reg);
#endif
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_1);
  
  reg |= (1 << 3);

  writeRegister(MSA30X_REG_INT_SET_1, reg);
}

/**************************************************************************/
/*!
    @brief  Enable freefall interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableOrientationInterrupt(uint8_t interrupt)
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_0);
      reg |= (1 << 6);
      writeRegister(MSA30X_REG_INT_MAP_0, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)      
      reg = readRegister(MSA30X_REG_INT_MAP_2_1);
      reg |= (1 << 6);
      writeRegister(MSA30X_REG_INT_MAP_2_1, reg);
#endif
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_0);
  
  reg |= (1 << 6);

  writeRegister(MSA30X_REG_INT_SET_0, reg);
}

/**************************************************************************/
/*!
    @brief  Enable single tap interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableSingleTapInterrupt(uint8_t interrupt)
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_0);
      reg |= (1 << 5);
      writeRegister(MSA30X_REG_INT_MAP_0, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)      
      reg = readRegister(MSA30X_REG_INT_MAP_2_1);
      reg |= (1 << 5);
      writeRegister(MSA30X_REG_INT_MAP_2_1, reg);
#endif
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_0);
  
  reg |= (1 << 5);

  writeRegister(MSA30X_REG_INT_SET_0, reg);
}

/**************************************************************************/
/*!
    @brief  Enable double tap interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableDoubleTapInterrupt(uint8_t interrupt)
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_0);
      reg |= (1 << 4);
      writeRegister(MSA30X_REG_INT_MAP_0, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)  
      reg = readRegister(MSA30X_REG_INT_MAP_2_1);
      reg |= (1 << 4);
      writeRegister(MSA30X_REG_INT_MAP_2_1, reg);
#endif
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_0);
  
  reg |= (1 << 4);

  writeRegister(MSA30X_REG_INT_SET_0, reg);
}

/**************************************************************************/
/*!
    @brief  Enable new data interrupt. Interrupt parameter corresponds to
            interrupt pins.
    @param  interrupt
            Index of interrupt (1 or 2)
*/
/**************************************************************************/
void MSA30X::enableNewDataInterrupt(uint8_t interrupt)
{
  uint8_t reg;
  switch(interrupt) {
    case 1:
      reg = readRegister(MSA30X_REG_INT_MAP_1);
      reg |= (1 << 0);
      writeRegister(MSA30X_REG_INT_MAP_1, reg);
      break;
    case 2:
#if !defined (MSA30X_MSA301)    
      reg = readRegister(MSA30X_REG_INT_MAP_1);
      reg |= (1 << 7);
      writeRegister(MSA30X_REG_INT_MAP_1, reg);
#endif      
      break;
  }

  reg = readRegister(MSA30X_REG_INT_SET_1);
  
  reg |= (1 << 4);

  writeRegister(MSA30X_REG_INT_SET_1, reg);
}

/**************************************************************************/
/*!
    @brief Check orientation of the chip.
    @return Orientation struct containing z and xy-orientations
*/
/**************************************************************************/
orient_t MSA30X::checkOrientation(void)
{
  orient_t orientation;
  uint8_t reg = readRegister(MSA30X_REG_ORIENT_STATUS);

  orientation.z = (zOrient_t)((reg >> 6) & 1);
  orientation.xy = (xyOrient_t)((reg >> 4) & 0x3);

  return orientation;
}

/**************************************************************************/
/*!
    @brief  Set offset compensation value for specific axis. Value can vary
            from 0 to 998.4 mg. 
            Values outside the range will be clamped.
    @param  axis
            Axis to set offset on
    @param  value
            Offset value (0 to 998.4 mg)
*/
/**************************************************************************/
void MSA30X::setOffset(axis_t axis, float value)
{
  uint8_t offset = value / 3.9f;
  uint8_t reg;

  offset = clamp<float>(offset, 0, 998.4f);

  switch(axis) {
    case MSA30X_AXIS_X:
    reg = readRegister(MSA30X_REG_OFFSET_COMP_X);

    reg &= ~0xFF;
    reg |= offset;

    writeRegister(MSA30X_REG_OFFSET_COMP_X, reg);
    break;

    case MSA30X_AXIS_Y:
    reg = readRegister(MSA30X_REG_OFFSET_COMP_Y);

    reg &= ~0xFF;
    reg |= offset;

    writeRegister(MSA30X_REG_OFFSET_COMP_Y, reg);
    break;

    case MSA30X_AXIS_Z:
    reg = readRegister(MSA30X_REG_OFFSET_COMP_Z);

    reg &= ~0xFF;
    reg |= offset;

    writeRegister(MSA30X_REG_OFFSET_COMP_Z, reg);
    break;
  }

}

/**************************************************************************/
/*!
    @brief  Set threshold of tap interrupt. Value can vary from 0 to full
            scale of each range. Values outside the range will be clamped.
    @param  value
            Tap threshold value (0 to full scale)
*/
/**************************************************************************/
void MSA30X::setTapThreshold(float value)
{ 
  float offset = 0;
  switch(_range) {
    case MSA30X_RANGE_16_G:
    offset = clamp<float>(value / MSA30X_MG2G_TAP_TH_16_G, 0, 16000.0);
    
    break;
    case MSA30X_RANGE_8_G:
    offset = clamp<float>(value / MSA30X_MG2G_TAP_TH_8_G, 0, 8000.0);

    break;
    case MSA30X_RANGE_4_G:
    offset = clamp<float>(value / MSA30X_MG2G_TAP_TH_4_G, 0, 4000.0);

    break;
    case MSA30X_RANGE_2_G:
    offset = clamp<float>(value / MSA30X_MG2G_TAP_TH_2_G, 0, 2000.0);

    break;
  }

  writeRegister(MSA30X_REG_TAP_TH, offset);
}

/**************************************************************************/
/*!
    @brief  Set duration of tap interrupt.
    @param  duration
            Second shock duration: According to tapDuration_t enum.
    @param  quiet
            Quiet duration: 0 -> 30 ms
                            1 -> 20 ms
    @param  shock
            Shock duration: 0 -> 50 ms
                            1 -> 70 ms
*/
/**************************************************************************/
void MSA30X::setTapDuration(tapDuration_t duration, uint8_t quiet, uint8_t shock)
{
  uint8_t reg = 0;

  reg |= (quiet << 7);
  reg |= (shock << 6);
  reg |= duration;

  writeRegister(MSA30X_REG_TAP_DUR, reg);
}

/**************************************************************************/
/*!
    @brief  Set threshold of active interrupt. Value can vary from 0 to full
            scale of each range. Values outside the range will be clamped.
    @param  value
            Active threshold value (0 to full scale)
*/
/**************************************************************************/
void MSA30X::setActiveThreshold(float value)
{ 
  float offset = 0;
  switch(_range) {
    case MSA30X_RANGE_16_G:
    offset = clamp<float>(value / MSA30X_MG2G_ACTIVE_TH_16_G, 0, 16000.0);
    break;

    case MSA30X_RANGE_8_G:
    offset = clamp<float>(value / MSA30X_MG2G_ACTIVE_TH_8_G, 0, 8000.0);
    break;

    case MSA30X_RANGE_4_G:
    offset = clamp<float>(value / MSA30X_MG2G_ACTIVE_TH_4_G, 0, 4000.0);
    break;

    case MSA30X_RANGE_2_G:
    offset = clamp<float>(value / MSA30X_MG2G_ACTIVE_TH_2_G, 0, 2000.0);
    break;
  }

  writeRegister(MSA30X_REG_ACTIVE_TH, offset);
}

/**************************************************************************/
/*!
    @brief  Set duration of active interrupt.
            Value can vary from 1 ms to 5 ms.
    @param  duration
            Active interrupt duration (1 to 5 ms)
*/
/**************************************************************************/
void MSA30X::setActiveDuration(uint8_t duration)
{
  uint8_t reg = 0;
  uint8_t value = clamp<uint8_t>(duration - 1, 0, 4);
  reg |= value;

  writeRegister(MSA30X_REG_ACTIVE_DUR, reg);
}

/**************************************************************************/
/*!
    @brief  Set duration of freefall interrupt.
            Value can vary from 2 ms to 512 ms.
    @param  duration
            Freefall interrupt duration (2 to 512 ms)
*/
/**************************************************************************/
void MSA30X::setFreefallDuration(uint16_t duration)
{
  uint8_t reg = 0;
  float dur_f;
  duration = clamp<uint16_t>(duration, 2, 512);
  dur_f = clamp<float>((float)(duration)/2.0f - 1, 0, 256); // avoid rounding the result in between 
  reg |= (uint8_t)dur_f;

  writeRegister(MSA30X_REG_FREEFALL_DUR, reg);
}

/**************************************************************************/
/*!
    @brief  Set threshold of freefall interrupt. Value can vary from 0 to full
            scale of each range. Values outside the range will be clamped.
    @param  value
            Freefall interrupt threshold (0 to full scale)
*/
/**************************************************************************/
void MSA30X::setFreefallThreshold(float value)
{ 
  float threshold = value / 7.81f;
  switch(_range) {
    case MSA30X_RANGE_16_G:
    threshold = clamp<float>(threshold, 0, 16000.0);
    break;

    case MSA30X_RANGE_8_G:
    threshold = clamp<float>(threshold, 0, 8000.0);
    break;

    case MSA30X_RANGE_4_G:
    threshold = clamp<float>(threshold, 0, 4000.0);
    break;

    case MSA30X_RANGE_2_G:
    threshold = clamp<float>(threshold, 0, 2000.0);
    break;
  }

  writeRegister(MSA30X_REG_FREEFALL_TH, threshold);
}

/**************************************************************************/
/*!
    @brief  Set hysteresis value and mode of freefall interrupt. 
            Value can vary from 0 to 500 mg in integers of 125mg.
            Values outside the range will be clamped.
    @param  mode
            Mode: 1 -> sum mode |acc_x| + |acc_y| + |acc_z|
                  0 -> single mode
    @param  value
            Freefall hysteresis value (0 to 500 mg in steps of 125 mg)
*/  
/**************************************************************************/
void MSA30X::setFreefallHysteresis(uint8_t mode, uint8_t value)
{
  uint8_t reg = 0;
  uint8_t hysteresis = (uint8_t)clamp<uint16_t>(value / 125, 0, 500);

  reg |= (mode << 3);
  reg &= ~0x3;
  reg |= hysteresis;

  writeRegister(MSA30X_REG_FREEFALL_HY, reg);
}

/**************************************************************************/
/*! 
    @brief  Swap polarity.
    @param  polarity
            Polarity to be changed
*/
/**************************************************************************/
void MSA30X::swapPolarity(pol_t polarity)
{
  uint8_t reg = readRegister(MSA30X_REG_SWAP_POLARITY);

  reg ^= (1 << polarity);

  writeRegister(MSA30X_REG_SWAP_POLARITY, reg);
}

/**************************************************************************/
/*! 
    @brief  Set orientation mode
    @param  mode
            Orientation mode
*/
/**************************************************************************/
void MSA30X::setOrientMode(orientMode_t mode)
{
  uint8_t reg = readRegister(MSA30X_REG_ORIENT_HY);

  reg &= ~0x3;
  reg |= mode;

  writeRegister(MSA30X_REG_ORIENT_HY, reg);
}

/**************************************************************************/
/*! 
    @brief  Set orientation hysteresis. Value can vary from 0 to 500 mg.
    @param  value
            Orientation hysteresis value (0 to 500 mg)
*/
/**************************************************************************/
void MSA30X::setOrientHysteresis(float value)
{
  uint8_t reg = readRegister(MSA30X_REG_ORIENT_HY);

  uint8_t hysteresis = (uint8_t)clamp<float>(value / 62.5f, 0, 8);
  
  reg &= ~0x70;
  reg |= hysteresis;

  writeRegister(MSA30X_REG_ORIENT_HY, reg);
}

/**************************************************************************/
/*! 
    @brief  Set z blocking.
    @param  mode
            Orientation blocking mode
    @param  zBlockValue
            Limit value of z-blocking
*/
/**************************************************************************/
void MSA30X::setBlocking(orientBlockMode_t mode, float zBlockValue)
{
  uint8_t reg = readRegister(MSA30X_REG_ORIENT_HY);

  reg &= ~0xC;
  reg |= mode;

  writeRegister(MSA30X_REG_ORIENT_HY, reg);

  uint8_t value = (uint8_t)clamp<float>(zBlockValue / 62.5f, 0, 15);

  writeRegister(MSA30X_REG_Z_BLOCK, value);
}

/**************************************************************************/
/*! 
    @brief  Get the acceleration.
    @return Acceleration struct containing accelarations of each axis in 
            m/s^2
*/
/**************************************************************************/
acc_t MSA30X::getAcceleration(void) 
{
  acc_t acceleration;
  
  acceleration.x = getX() * _multiplier * GRAVITY;
  acceleration.y = getY() * _multiplier * GRAVITY;
  acceleration.z = getZ() * _multiplier * GRAVITY;
  
  return acceleration;
}
