// Copyright (c) UT2UH. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2020, M5Stack, Hades2001, ropg, UT2UH
 * \class MaixAmigo
 * \brief   MaixAmigo library.
 * @file    MaixAmigo.h
 * @author  M5Stack, Hades2001, ropg, naofum, fukuen, UT2UH
 * @version V0.0.1 Beta
 * @date    2020/11/01
 * @brief   Header for MaixAmigo.cpp module
 *
 * \par Description
 * This file is a drive for MaixAmigo.
 *
 * \par Method List:
 *
 *  System:
        MA.begin();
    LCD:
        MA.lcd.setBrightness(uint8_t brightness);
        MA.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        MA.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        MA.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        MA.Lcd.fillScreen(uint16_t color);
        MA.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        MA.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        MA.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        MA.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        MA.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        MA.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        MA.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        MA.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        MA.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        MA.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        MA.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        MA.Lcd.setCursor(uint16_t x0, uint16_t y0);
        MA.Lcd.setTextColor(uint16_t color);
        MA.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        MA.Lcd.setTextSize(uint8_t size);
        MA.Lcd.setTextWrap(boolean w);
        MA.Lcd.printf();
        MA.Lcd.print();
        MA.Lcd.println();
        MA.Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        MA.Lcd.drawRightString(const char *string, int dX, int poY, int font);
        MA.Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        MA.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        MA.Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
    Touch:
        See MaixTouch.h for documentation
    Buttons:
        See utility/MaixButton.h for documentation
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *    UT2UH           2020/11/01         0.0.1          Rebuild the new.
 * </pre>
 *
 */

#ifndef _MaixAmigo_H_
  #define _MaixAmigo_H_

  #if defined(K210)

    #include <Arduino.h>
    #include <Wire.h>
    #include <SPI.h>
    // #include "FS.h"
    // #include "SD.h"

    #include "utility/Config.h"
    #include "MaixDisplay.h"
    #include "MaixTouch.h"			    // MaixTouch
    #include "utility/MaixButton.h"	// MaixButtons, M5Events, Button, Gesture

    #if defined (ARDUINO_M5STICK_V)
      #include "drivers/AXP1XX/AXP1XX.h"
      #include "drivers/MPU6886/MPU6886.h"
    #elif defined (ARDUINO_MAIX_AMIGO) || defined(ARDUINO_MAIX_CUBE) || defined (ARDUINO_MAIX_DOCK)
      #include "drivers/AXP1XX/AXP1XX.h"
      #include "drivers/MSA30X/MSA30X.h"
    #endif

    class MaixAmigo
    {
      public:
        MaixAmigo();
        void begin(bool LCDEnable = true, bool SDEnable = true, bool SerialEnable = true, bool I2CEnable = true);
        void update();

        // Power
      #if defined (ARDUINO_MAIX_AMIGO) || defined(ARDUINO_MAIX_CUBE) || defined (ARDUINO_M5STICK_V)
        AXP1XX Axp = AXP1XX();
      #endif

        // LCD
        MaixDisplay Lcd;

		    // Touch
      #if defined (ARDUINO_MAIX_AMIGO)
        MaixTouch Touch;
      #elif defined (ARDUINO_MAIX_GO)
        MaixTouchNS2009 Touch;
      #endif
      
        // Buttons (global button and gesture functions)
        MaixButtons Buttons;

      #if defined (ARDUINO_MAIX_AMIGO)
        // Default "button" that gets events where there is no button.
        Button background = Button(0, 0, TOUCH_W, TOUCH_H, true, "background");

        // Touch version of the buttons on Maix Amigo 320x480 TFT, on screen
        Button BtnA = Button(10,440,110,40, true ,"BtnA");
        Button BtnB = Button(130,440,70,40, true, "BtnB");
        Button BtnC = Button(230,440,80,40, true, "BtnC");
      #elif defined (ARDUINO_MAIX_GO)
        // Default "button" that gets events where there is no button.
        Button background = Button(0, 0, TOUCH_W, TOUCH_H, true, "background");

        // Touch version of the buttons on most Maix 240x320 TFT, on screen
        Button BtnA = Button(10,200,110,40, true ,"BtnA");
        Button BtnB = Button(130,200,70,40, true, "BtnB");
        Button BtnC = Button(230,200,80,40, true, "BtnC");
      #else
        // TODO hardware buttons for all other boards
        Button BtnA = Button(10,200,110,20, true ,"BtnA");
        Button BtnB = Button(130,200,70,20, true, "BtnB");
        Button BtnC = Button(230,200,80,20, true, "BtnC");
      #endif

        //IMU
      #if defined (ARDUINO_M5STICK_V)
        MPU6886 Mpu;
      #elif defined (ARDUINO_MAIX_AMIGO) || defined(ARDUINO_MAIX_CUBE) || defined (ARDUINO_MAIX_DOCK)
        //MSA30X Imu = MSA30X();
        MSA30X Msa;
      #endif

      private:
          bool isInited;
    };

    extern MaixAmigo MA;
    #define ma MA
    #define Ma MA
    #define mA MA
    #define lcd Lcd
  #else
    #error “This library only supports boards with K210 processor.”
  #endif
#endif


