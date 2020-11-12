#include <MaixAmigo.h>

void setup(void) {

  MA.begin();

  //Draw the jpeg file "p2.jpg" from TF(SD) card
  MA.Lcd.setBrightness(200);
  MA.Lcd.drawJpgFile(SD, "/p2.jpg");
  // MA.Lcd.drawJpgFile(SD, "/p1.jpg", 20, 30, 40, 50);
  // MA.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y, uint16_t maxWidth, uint16_t maxHeight, uint16_t offX, uint16_t offY, jpeg_div_t scale);
}

void loop() {

}
