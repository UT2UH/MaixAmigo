#ifndef _MAIXDISPLAY_H_
  #define _MAIXDISPLAY_H_

  #include <Arduino.h>
  // #include <FS.h>
  #include <SPI.h>
  #undef max  //or next include doesn't work
    #include <vector>

  #include "utility/Config.h"  // This is where boards defines would be
  #include "utility/In_eSPI.h"
  #include "utility/Sprite.h"

  typedef enum {
    JPEG_DIV_NONE,
    JPEG_DIV_2,
    JPEG_DIV_4,
    JPEG_DIV_8,
    JPEG_DIV_MAX
  } jpeg_div_t;

  struct DisplayState {
    uint8_t textfont, textsize, datum;
    const GFXfont *gfxFont;
    uint32_t textcolor, textbgcolor;
    int32_t cursor_x, cursor_y, padX;
  };

namespace colors  // Color definitions for backwards compatibility with old sketches
{
  [[deprecated("use TFT_BLACK"      )]] static constexpr int BLACK       = 0x0000;      /*   0,   0,   0 */
  [[deprecated("use TFT_NAVY"       )]] static constexpr int NAVY        = 0x000F;      /*   0,   0, 128 */
  [[deprecated("use TFT_DARKGREEN"  )]] static constexpr int DARKGREEN   = 0x03E0;      /*   0, 128,   0 */
  [[deprecated("use TFT_DARKCYAN"   )]] static constexpr int DARKCYAN    = 0x03EF;      /*   0, 128, 128 */
  [[deprecated("use TFT_MAROON"     )]] static constexpr int MAROON      = 0x7800;      /* 128,   0,   0 */
  [[deprecated("use TFT_PURPLE"     )]] static constexpr int PURPLE      = 0x780F;      /* 128,   0, 128 */
  [[deprecated("use TFT_OLIVE"      )]] static constexpr int OLIVE       = 0x7BE0;      /* 128, 128,   0 */
  [[deprecated("use TFT_LIGHTGREY"  )]] static constexpr int LIGHTGREY   = 0xC618;      /* 192, 192, 192 */
  [[deprecated("use TFT_DARKGREY"   )]] static constexpr int DARKGREY    = 0x7BEF;      /* 128, 128, 128 */
  [[deprecated("use TFT_BLUE"       )]] static constexpr int BLUE        = 0x001F;      /*   0,   0, 255 */
  [[deprecated("use TFT_GREEN"      )]] static constexpr int GREEN       = 0x07E0;      /*   0, 255,   0 */
  [[deprecated("use TFT_CYAN"       )]] static constexpr int CYAN        = 0x07FF;      /*   0, 255, 255 */
  [[deprecated("use TFT_RED"        )]] static constexpr int RED         = 0xF800;      /* 255,   0,   0 */
  [[deprecated("use TFT_MAGENTA"    )]] static constexpr int MAGENTA     = 0xF81F;      /* 255,   0, 255 */
  [[deprecated("use TFT_YELLOW"     )]] static constexpr int YELLOW      = 0xFFE0;      /* 255, 255,   0 */
  [[deprecated("use TFT_WHITE"      )]] static constexpr int WHITE       = 0xFFFF;      /* 255, 255, 255 */
  [[deprecated("use TFT_ORANGE"     )]] static constexpr int ORANGE      = 0xFD20;      /* 255, 165,   0 */
  [[deprecated("use TFT_GREENYELLOW")]] static constexpr int GREENYELLOW = 0xAFE5;      /* 173, 255,  47 */
  [[deprecated("use TFT_PINK"       )]] static constexpr int PINK        = 0xF81F;
}

using namespace colors;

  class MaixDisplay : public TFT_eSPI {
    public:
      static MaixDisplay* instance;
      MaixDisplay();
      void begin();
      void sleep();
      void wakeup();
      void setBrightness(uint8_t brightness); //TODO
      void clearDisplay(uint32_t color=TFT_BLACK) { fillScreen(color); }
      void clear(uint32_t color=TFT_BLACK) { fillScreen(color); }
      void display() {}

      // inline void startWrite(void){
      //   #if defined (SPI_HAS_TRANSACTION) && defined (SUPPORT_TRANSACTIONS) && !defined(ESP32_PARALLEL)
      //     if (locked) {
      //       locked = false; SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
      //     }
      //   #endif
      //   CS_L;
      // }
      // inline void endWrite(void){
      //   #if defined (SPI_HAS_TRANSACTION) && defined (SUPPORT_TRANSACTIONS) && !defined(ESP32_PARALLEL)
      //     if(!inTransaction) {
      //       if (!locked) {
      //         locked = true;
      //         SPI.endTransaction();
      //       }
      //     }
      //   #endif
      //   CS_H;
      // }
      // inline void writePixel(uint16_t color) {
      //   SPI.write16(color);
      // }
      // inline void writePixels(uint16_t * colors, uint32_t len){
      //   SPI.writePixels((uint8_t*)colors , len * 2);
      // }
      void progressBar(int x, int y, int w, int h, uint8_t val);

      #define setFont setFreeFont

      void qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
      void qrcode(const String &string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);

      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint8_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, uint8_t *data);
      void drawBitmap(int16_t x0, int16_t y0, int16_t w, int16_t h, const uint16_t *data, uint16_t transparent);

      // void drawBmp(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
      // void drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

      // void drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x = 0,
      //             uint16_t y = 0, uint16_t maxWidth = 0, uint16_t maxHeight = 0,
      //             uint16_t offX = 0, uint16_t offY = 0,
      //             jpeg_div_t scale = JPEG_DIV_NONE);

      // void drawJpg(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
      //               uint16_t maxWidth = 0, uint16_t maxHeight = 0,
      //               uint16_t offX = 0, uint16_t offY = 0,
      //               jpeg_div_t scale = JPEG_DIV_NONE);

      // void drawJpgFile(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
      //               uint16_t maxWidth = 0, uint16_t maxHeight = 0,
      //               uint16_t offX = 0, uint16_t offY = 0,
      //               jpeg_div_t scale = JPEG_DIV_NONE);

      // void drawPngFile(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
      //               uint16_t maxWidth = 0, uint16_t maxHeight = 0,
      //               uint16_t offX = 0, uint16_t offY = 0,
      //               double scale = 1.0, uint8_t alphaThreshold = 127);

      // void drawPngUrl(const char *url, uint16_t x = 0, uint16_t y = 0,
      //               uint16_t maxWidth = 0, uint16_t maxHeight = 0,
      //               uint16_t offX = 0, uint16_t offY = 0,
      //               double scale = 1.0, uint8_t alphaThreshold = 127);

    // Saves and restores font properties, datum, cursor and colors so
    // code can be non-invasive. Just make sure that every push is also
    // popped when you're done to prevent stack from growing.
    //
    // (User code can never do this completely because the gfxFont
    // class variable of TFT_eSPI is protected.)
    #define DISPLAY_HAS_PUSH_POP
     public:
      void pushState();
      void popState();

     private:
      std::vector<DisplayState> _displayStateStack;

    #ifdef ARDUINO_MAIX_AMIGO

      #ifdef TFT_eSPI_TOUCH_EMULATION
        // Emulates the TFT_eSPI touch interface using MA.Touch
       public:
        uint8_t getTouchRaw(uint16_t *x, uint16_t *y);
        uint16_t getTouchRawZ(void);
        void convertRawXY(uint16_t *x, uint16_t *y);
        uint8_t getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);
        void calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg,
                            uint8_t size);
        void setTouch(uint16_t *data);
      #endif /* TFT_eSPI_TOUCH_EMULATION */

    #endif /* ARDUINO_MAIX_AMIGO */
};
#endif /* _MAIXDISPLAY_H_ */
