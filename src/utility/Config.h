#ifndef _CONFIG_H_
  #define _CONFIG_H_

  #define TFT     MaixDisplay::instance
  //#define TFT_eSPI_TOUCH_EMULATION
  #define TOUCH		MaixTouch::instance
  #define BUTTONS	MaixButtons::instance

  // UART
  #define USE_SERIAL Serial

  // Buttons
  #define BTN_A 0
  #define BTN_B 1
  #define BTN_C 2
  #define BUTTON_A 0
  #define BUTTON_B 1
  #define BUTTON_C 2

  #define ARDUINO_MAIX_AMIGO

  #if defined(ARDUINO_MAIX_AMIGO)

    #define DEFAULT_ROTATION   0
    //#define TFT_eSPI_TOUCH_EMULATION
    #define TOUCH_W          320
    #define TOUCH_H          480
    #define CST_DEVICE_ADDR 0x38  //FT6236U
    #define CST_INT         33UL
    // Can't trust TFT_WIDTH, driver is portrait
    #define HIGHEST_X        319
    #define HIGHEST_Y        479

    #define SD_ENABLE          0
    #define TFCARD_MISO        6
    #define TFCARD_MOSI       10
    #define TFCARD_SCLK       11
    #define TFCARD_CS         26

    #define SPEAKER_PIN       -1
    #define I2S_MCLK          13
    #define I2S_LRCK          18
    #define I2S_SCLK          21
    #define I2S_DOUT          34
    #define I2S_DIN           35
    #define HAS_CODEC

    #define BUTTON_A_PIN      31
    #define BUTTON_B_PIN      23
    #define BUTTON_C_PIN      20

  #elif defined (ARDUINO_MAIX_CUBE)

    #define DEFAULT_ROTATION   0
    #define TFT_LED           17
    #define HAS_BACKLIGHT

    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define SPEAKER_PIN       -1
    #define I2S_MCLK          19
    #define I2S_LRCK          33  //WS
    #define I2S_SCLK          35
    #define I2S_DOUT          34
    #define I2S_DIN           18
    #define HAS_CODEC

    #define UART_TX           33
    #define UART_RX           34

    #define HAS_BUTTON
    #define BUTTON_A_PIN      10
    #define BUTTON_B_PIN      11
    #define BUTTON_C_PIN      -1

  #elif defined( ARDUINO_MAIX_GO )

    #define DEFAULT_ROTATION   0
    #define TFT_eSPI_TOUCH_EMULATION
    #define TOUCH_W          320
    #define TOUCH_H          240
    #define CST_DEVICE_ADDR 0x48  //NS2009
    #define CST_INT         23UL
    // Can't trust TFT_WIDTH, driver is landscape
    #define HIGHEST_X        319
    #define HIGHEST_Y        239

    #define WIFI_TX            6
    #define WIFI_RX            7
    #define WIFI_EN            8
    #define HAS_ESP8255

    #define MIC_BCK           18
    #define MIC_WS            19
    #define MIC_DAT3          20
    #define HAS_MIC
    
    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define SPEAKER_PIN       -1
    #define I2S_WS            33 
    #define I2S_DA            34
    #define I2S_BCK           35
    #define HAS_DAC

    #define HAS_BUTTON
    #define BUTTON_A_PIN      15
    #define BUTTON_B_PIN      16
    #define BUTTON_C_PIN      17

  #elif defined(ARDUINO_MAIX_DUINO)

    #define DEFAULT_ROTATION   0

    #define WIFI_TX            6
    #define WIFI_RX            7
    #define WIFI_EN            8
    #define WIFI_READY         9
    #define WIFI_SPI_CS       25
    #define HAS_ESP32

    #define MIC_BCK           18
    #define MIC_WS            19
    #define MIC_DAT3          20
    #define HAS_MIC

    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define SPEAKER_PIN       -1
    #define I2S_WS            33 
    #define I2S_DA            34
    #define I2S_BCK           35
    #define HAS_DAC

    #define BUTTON_A_PIN      -1
    #define BUTTON_B_PIN      -1
    #define BUTTON_C_PIN      -1

  #elif defined(ARDUINO_M1_DOCK) || defined( ARDUINO_M1W_DOCK )

    #define DEFAULT_ROTATION   0

    #define WIFI_TX            6
    #define WIFI_RX            7
    #define WIFI_EN            8
    #define HAS_ESP8255

    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define MIC_WS            30
    #define MIC_DATA          31
    #define MIC_BCK           32
    #define HAS_MIC

    #define SPEAKER_PIN       -1
    #define I2S_WS            33 
    #define I2S_DA            34
    #define I2S_BCK           35
    #define HAS_DAC

    #define HAS_BUTTON
    #define BUTTON_A_PIN      -1  //11
    #define BUTTON_B_PIN      -1  //15
    #define BUTTON_C_PIN      -1  //17

  #elif defined(ARDUINO_MAIX_BIT) || defined(ARDUINO_MAIX_BIT_MIC)

    #define DEFAULT_ROTATION   0

    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define MIC_WS            30
    #define MIC_DATA          31
    #define MIC_BCK           32
    #define HAS_MIC

    #define SPEAKER_PIN       -1

    #define BUTTON_A_PIN      -1  //11
    #define BUTTON_B_PIN      -1  //15
    #define BUTTON_C_PIN      -1  //17

  #elif defined(ARDUINO_AIOT_DAAN)

    #define DEFAULT_ROTATION   0

    #define SD_ENABLE          0
    #define TFCARD_MISO       26
    #define TFCARD_SCLK       27
    #define TFCARD_MOSI       28
    #define TFCARD_CS         29

    #define MIC_WS            30
    #define MIC_DATA          31
    #define MIC_BCK           32
    #define HAS_MIC

    #define SPEAKER_PIN       -1

    #define BUTTON_A_PIN      -1  //11
    #define BUTTON_B_PIN      -1  //15
    #define BUTTON_C_PIN      -1  //17

  #elif defined(ARDUINO_M5STICK_V)

    #define AXP1XX_INT        23
    
    #define DEFAULT_ROTATION   0

    #define SPEAKER_PIN       -1
    #define MIC_WS            10
    #define MIC_DAT3          12
    #define MIC_BCK           13
    #define HAS_MIC

    #define I2S_SD            11
    #define I2S_LRCLK         14
    #define I2S_BCLK          15
    #define I2S_DIN           17
    #define HAS_DAC
    
    #define SD_ENABLE          0
    #define TFCARD_SCLK       30
    #define TFCARD_MISO       31
    #define TFCARD_CS         32
    #define TFCARD_MOSI       33

    #define HAS_BUTTON
    #define BUTTON_A_PIN      36
    #define BUTTON_B_PIN      37
    #define BUTTON_C_PIN      -1

  #elif defined( ARDUINO_M5UNIT_V  )

    #define DEFAULT_ROTATION   0
    
    #define LED_RGB            8

    #define SPEAKER_PIN       -1
    #define MIC_WS            10
    #define MIC_DAT3          12
    #define MIC_BCK           13
    #define HAS_MIC

    #define I2S_SD            11 //25?
    #define I2S_LRCLK         14
    #define I2S_BCLK          15
    #define I2S_DIN           17
    #define HAS_DAC
    
    #define SD_ENABLE          0
    #define TFCARD_SCLK       30
    #define TFCARD_MISO       31
    #define TFCARD_CS         32
    #define TFCARD_MOSI       33

    #define HAS_BUTTON
    #define BUTTON_A_PIN      36
    #define BUTTON_B_PIN      37
    #define BUTTON_C_PIN      -1

  #endif

#endif // _CONFIG_H_

