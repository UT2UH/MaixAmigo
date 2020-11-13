// Copyright (c) UT2UH. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "utility/Config.h"
#include "MaixAmigo.h"


MaixAmigo::MaixAmigo() : isInited(0) {
}

void MaixAmigo::begin(bool LCDEnable, bool SDEnable, bool SerialEnable, bool I2CEnable) {
  // Correct init once
  if (isInited == true) {
    return;
  } else {
    isInited = true;
  }

  // UART
  if (SerialEnable == true) {
    Serial.begin(1000000);
    Serial.flush();
    delay(50);
    Serial.println("MaixAmigo initializing...");
  }

  // I2C init
  if (I2CEnable == true) {
    Wire.begin((uint8_t) SDA, (uint8_t) SCL, 400000);
  }

  // I2C PMIC AXP173/AXP192
  #if defined(MAIX_AMIGO)
    // Minimal setup to get Amigo powered
    // Axp.setLDO4(1500);
    // Axp.setLDO2(1800);
    // Axp.setLDO3(3000);

    AXP1XX_InitDef initAmigo = {
      .DCDC1  = 3300, //26
      .DCDC2  = 0,    //23
      .LDO2   = 1800, //28
      .LDO3   = 3000, //28
      .LDO4   = 1500, //27 =AXP192 DCDC3 0.8V
      .CHGC   = CHGC_100mA, //33
      .EXTEN  = true, //12
      .BACKUP = true, //35
    };
    Axp.begin(initAmigo);

  #elif defined(MAIX_CUBE)
    // Minimal setup to get Cube powered
    Axp.setLDO4(1500);
    Axp.setLDO2(1800);
    Axp.setLDO3(3000);
    //TODO Check full setup
    // AXP1XX_InitDef initCube = {
    //   .DCDC1  = 3300, //26
    //   .DCDC2  = 0,    //23
    //   .LDO2   = 1800, //28
    //   .LDO3   = 3000, //28
    //   .LDO4   = 1500, //27 =AXP192 DCDC3 0.8V
    //   .CHGC   = CHGC_100mA, //33
    //   .EXTEN  = true, //12
    //   .BACKUP = true, //35
    // };
    // Axp.begin(initCube);
  #elif defined(M5STICK_V)
    // writeByte(0x46, 0xFF); //Clear interupts

    // writeByte(0x23, 0x08); //Set DC-DC2 (VCore) to 0V9
    // writeByte(0x33, 0xC0); //Set Bat Charging Voltage to 4V2, Current to 100mA
    // writeByte(0x36, 0x0C); //Set 128ms power on, 4s power off

    // writeByte(0x91, 0xF0); //Set GPIO0 (TFT_BL) (from 0x70 (min)) to 3V3 (max)
    // writeByte(0x90, 0x02); //Set GPIO0 to LDO mode

    // writeByte(0x28, 0xF0); //VDD2.8V net: LDO2 3.3V,  VDD 1.5V net: LDO3 1.8V

    // writeByte(0x27, 0x2C); //VDD1.8V net: DC-DC3 1.8V

    // writeByte(0x12, 0xFF); //Enble all LDO/DC_DC and EXTEN

    // writeByte(0x32, 0x46); //Enable bat detection
    // writeByte(0x39, 0xFC); //Disable Temp Protection (Sensor doesn't exist!)
    // writeByte(0x31, (Read8bit(0x31) & 0xf8) | (1 << 2)); // Set Power off voltage 3V0
    AXP1XX_InitDef initStickV = {
      .DCDC1  = 3300, //26
      .DCDC2  = 900,  //23  Set DC-DC2 (VCore) to 0V9
      .LDO2   = 3300, //28  VDD2.8V net: LDO2 3.3V,  VDD 1.5V net: LDO3 1.8V
      .LDO3   = 1800, //28  VDD2.8V net: LDO2 3.3V,  VDD 1.5V net: LDO3 1.8V
      .LDO4   = 1800, //27  VDD1.8V net: DC-DC3 1.8V
      .CHGC   = CHGC_100mA, //33
      .EXTEN  = true, //12
      .BACKUP = true, //35
    };
    sysctl_set_power_mode(SYSCTL_POWER_BANK3,SYSCTL_POWER_V33);
    Axp.begin(initStickV);
    Axp.setGPIO0(3000); // Set GPIO0 (TFT_BL) (from 0x70 (min)) to 3V3 (max)
    fpioa_set_function(AXP1XX_INT, (fpioa_function_t)(FUNC_GPIOHS0 + 26));
    gpiohs_set_drive_mode(26, GPIO_DM_OUTPUT);
    gpiohs_set_pin(26, GPIO_PV_HIGH); //Disable VBUS As Input, BAT->5V Boost->VBUS->Charing Cycle
    msleep(20);
  #endif

  // LCD INIT
  if (LCDEnable == true) {
    Lcd.begin();
  }

  // Touch init
  #if defined (MAIX_AMIGO) || defined (MAIX_GO)
    Touch.begin();  // Touch begin after AXP begin. (Reset at the start of AXP)
  #endif

  // IMU init
  #if defined (MAIX_AMIGO) || defined (MAIX_GO)
    Msa.begin();  // MSA30x
  #elif defined(M5STICK_V)
    Mpu.begin();  // MPU6886
  #endif

  // TF Card
  // if (SDEnable == true) {
  //   SD.begin(TFCARD_CS, SPI, 40000000);
  // }

  // TONE
  // Speaker.begin();

  if (SerialEnable == true) {
    Serial.println("OK");
  }

  //Rtc.begin();
}

void MaixAmigo::update() {
  Touch.update();
  Buttons.update();
  yield();
}

MaixAmigo MA;
