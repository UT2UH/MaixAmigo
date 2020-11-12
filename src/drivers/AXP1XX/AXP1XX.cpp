#include "drivers/AXP1XX/AXP1XX.h"

AXP1XX::AXP1XX() {
}

void AXP1XX::begin(AXP1XX_InitDef initDef) {
  setDCDC1(initDef.DCDC1);
  setDCDC2(initDef.DCDC2);
  setLDO2(initDef.LDO2);
  setLDO3(initDef.LDO3);
  setLDO4(initDef.LDO4);
  setChgCurrent(initDef.CHGC);
  setEXTEN(initDef.EXTEN);
  setBACKUP(initDef.BACKUP);

  writeByte(0x46, 0b11111111);  // Clear interupts
  writeByte(0x84, 0b11110010);  // ADC 200Hz
  writeByte(0x82, 0b11111111);  // ADC All Enable
  writeByte(0x30, 0b10000000);  // VBUS Open
  writeByte(0x32, 0b01000110);  // Enable bat detection
  // writeByte(0x12, 0b01011111);  // Enble all LDO/DC_DC and EXTEN
  writeByte(0x36, 0b00001100);  // PEK 128ms, PW OFF 4S
  writeByte(0x39, 0b11111100);  // Disable Temp Protection (Sensor doesn't exist!)
  writeByte(0x31, (readByte(0x31) & 0xf8) | (1 << 2)); //Power off 3V0
}

uint8_t AXP1XX::readByte(uint8_t axpReg) {
  Wire.beginTransmission(AXP_ADDR);
  Wire.write(axpReg);
  Wire.endTransmission();
  Wire.requestFrom(AXP_ADDR, 1);
  return Wire.read();;
}

void AXP1XX::writeByte(uint8_t axpReg, uint8_t data) {
  Wire.beginTransmission(AXP_ADDR);
  Wire.write(axpReg);
  Wire.write(data);
  Wire.endTransmission();
}

void AXP1XX::bitOn(uint8_t axpReg, uint8_t bit) {
  uint8_t val = readByte(axpReg) | bit;
  writeByte(axpReg, val);
}

void AXP1XX::bitOff(uint8_t axpReg, uint8_t bit) {
  uint8_t val = readByte(axpReg) & ~bit;
  writeByte(axpReg, val);
}

void AXP1XX::setDCDC1(int voltage) {
  // if (voltage < 700 || 3500 < voltage) {
  //   // Disable
  //   bitOff(0x12, (1 << 0));
  //   return;
  // } else {
  //   // Enable
  //   bitOn(0x12, (1 << 0));
  // }
  // Set
  uint8_t val = ((voltage - 700) / 25) & 0x7f;
  writeByte(0x26, val);
}

void AXP1XX::setDCDC2(int voltage) {
  // if (voltage < 700 || 2275 < voltage) {
  //   // Disable
  //   bitOff(0x12, (1 << 4));
  //   return;
  // } else {
  //   // Enable
  //   bitOn(0x12, (1 << 4));
  // }
  // Set
  uint8_t val = ((voltage - 700) / 25) & 0x3f;
  writeByte(0x23, val);  
}


void AXP1XX::setLDO2(int voltage) {
  if (voltage < 1800 || 3300 < voltage) {
    // Disable
    bitOff(0x12, (1 << 2));
    return;
  } else {
    // Set
    uint8_t val = readByte(0x28) & 0x0f;
    val |= (((voltage - 1800) / 100) & 0x0f ) << 4;
    writeByte(0x28, val);
    // Enable
    bitOn(0x12, (1 << 2));
  }
}

void AXP1XX::setLDO3(int voltage) {
  if (voltage < 1800 || 3300 < voltage) {
    // Disable
    bitOff(0x12, (1 << 3));
    return;
  } else {
    // Set
    uint8_t val = readByte(0x28) & 0xf0;
    val |= (((voltage - 1800) / 100) & 0x0f );
    writeByte(0x28, val);
    // Enable
    bitOn(0x12, (1 << 3));
  }
}

void AXP1XX::setLDO4(int voltage) {
  if (voltage < 700 || 3500 < voltage) {
    // Disable
    bitOff(0x12, (1 << 1));
    return;
  } else {
    // Set
    uint8_t val = ((voltage - 700) / 25) & 0x7f;
    writeByte(0x27, val);
    // Enable
    bitOn(0x12, (1 << 1));
  }
}

void AXP1XX::setGPIO0(int voltage) {
  if (voltage < 1800 || 3300 < voltage) {
    // Set Mode
    writeByte(0x90, voltage);
  } else {
    // LDO
    writeByte(0x90, 0b00000010);

    // Set voltage
    uint8_t val = (((voltage - 1800) / 100) & 0x0f ) << 4;
    writeByte(0x91, val);
  }
}

void AXP1XX::setEXTEN(bool enable) {
  if (enable) {
    // Enable
    bitOn(0x12, (1 << 6));
  } else {
    // Disable
    bitOff(0x12, (1 << 6));
  }
}

void AXP1XX::setBACKUP(bool enable) {
  if (enable) {
    // Enable
    bitOn(0x35, (1 << 7));
  } else {
    // Disable
    bitOff(0x35, (1 << 7));
  }
}

float AXP1XX::getBatteryVoltage() {
  uint16_t val = readByte(0x78) << 4;
  val |= readByte(0x79);
  return val * 1.1;
}

float AXP1XX::getBatteryDischargeCurrent() {
  uint16_t val = readByte(0x7c) << 5;
  val |= readByte(0x7d);
  return val * 0.5;
}

float AXP1XX::getBatteryChargeCurrent() {
  uint16_t val = readByte(0x7a) << 5;
  val |= readByte(0x7b);
  return val * 0.5;
}

float AXP1XX::getBatteryPower() {
  uint32_t val = (readByte(0x70) << 16) | (readByte(0x71) << 8) | readByte(0x72);
  return  1.1 * 0.5 * val / 1000.0;
}

float AXP1XX::getAcinVolatge() {
  uint16_t val = readByte(0x56) << 4;
  val |= readByte(0x57);
  return val * 1.7;
}

float AXP1XX::getAcinCurrent() {
  uint16_t val = readByte(0x58) << 4;
  val |= readByte(0x59);
  return val * 0.625;
}

float AXP1XX::getVbusVoltage() {
  uint16_t val = readByte(0x5a) << 4;
  val |= readByte(0x5b);
  return val * 1.7;
}

float AXP1XX::getVbusCurrent() {
  uint16_t val = readByte(0x5c) << 4;
  val |= readByte(0x5d);
  return val * 0.375;
}

float AXP1XX::getInternalTemperature() {
  uint16_t val = readByte(0x5e) << 4;
  val |= readByte(0x5f);
  return -144.7 + val * 0.1;
}

float AXP1XX::getApsVoltage() {
  uint16_t val = readByte(0x7e) << 4;
  val |= readByte(0x7f);
  return val * 1.4;
}

void AXP1XX::powerOff() {
  bitOn(0x32, (1 << 7));
}

uint8_t AXP1XX::getPekPress() {
  uint8_t val = readByte(0x46);
  writeByte(0x46, 0x03);
  return val;
}

void AXP1XX::setChgCurrent(CHGCurrent current){
    uint8_t val = readByte(0x33);
    val &= 0xf0;
    val = val | ( current & 0x0f );
    writeByte(0x33, val);
}