#ifndef __AXP1XX_H__
#define __AXP1XX_H__

#include <Arduino.h>
#include <Wire.h>

#define AXP_ADDR 0X34

#define SLEEP_MSEC(us) (((uint64_t)us) * 1000L)
#define SLEEP_SEC(us)  (((uint64_t)us) * 1000000L)
#define SLEEP_MIN(us)  (((uint64_t)us) * 60L * 1000000L)
#define SLEEP_HR(us)   (((uint64_t)us) * 60L * 60L * 1000000L)

enum CHGCurrent{
  CHGC_100mA = 0,
  CHGC_190mA,
  CHGC_280mA,
  CHGC_360mA,
  CHGC_450mA,
  CHGC_550mA,
  CHGC_630mA,
  CHGC_700mA,
  CHGC_780mA,
  CHGC_880mA,
  CHGC_960mA,
  CHGC_1000mA,
  CHGC_1080mA,
  CHGC_1160mA,
  CHGC_1240mA,
  CHGC_1320mA,
};

typedef struct {
  int     DCDC1;    //DCDC1: 3V3 Main rail. When not set the stick shuts down
  int     DCDC2;    //0V9 K210 VCore
  int     LDO2;
  int     LDO3;
  int     LDO4;     //AXP173 LDO4  = AXP192 DCDC3
  CHGCurrent CHGC;
  bool    EXTEN;
  bool    BACKUP;
} AXP1XX_InitDef;

class AXP1XX {
  public:   
    AXP1XX();
    void begin(AXP1XX_InitDef initDef);

    void setDCDC1(int voltage);   //0x26
    void setDCDC2(int voltage);   //0x23
    void setLDO1(int voltage);    //0x28
    void setLDO2(int voltage);    //0x28
    void setLDO3(int voltage);    //0x28
    void setLDO4(int voltage);    //0x27
    void setEXTEN(bool enable);   //0x12
    void setBACKUP(bool enable);  //0x35
    void setChgCurrent(CHGCurrent current);  //0x33

    void setGPIO0(int voltage);   //0x91

    float getBatteryVoltage();
    float getBatteryDischargeCurrent();
    float getBatteryChargeCurrent();
    float getBatteryPower();
    float getAcinVolatge();
    float getAcinCurrent();
    float getVbusVoltage();
    float getVbusCurrent();
    float getInternalTemperature();
    float getApsVoltage();

    void powerOff();
    uint8_t getPekPress();

  private:
    uint8_t readByte(uint8_t address);
    void writeByte(uint8_t address, uint8_t data);
    void bitOn(uint8_t address, uint8_t bit);
    void bitOff(uint8_t address, uint8_t bit);

};

#endif
