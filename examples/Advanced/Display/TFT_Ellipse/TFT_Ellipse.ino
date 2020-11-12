/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

#include <MaixAmigo.h>

void setup(void) {
  MA.begin();

  // MA.Lcd.setRotation(1);
}

void loop() {

  MA.Lcd.fillScreen(TFT_BLACK);

  // Draw some random ellipses
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(320 - rx - rx);
    int y = ry + random(240 - ry - ry);
    MA.Lcd.fillEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
  MA.Lcd.fillScreen(TFT_BLACK);

  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(320 - rx - rx);
    int y = ry + random(240 - ry - ry);
    MA.Lcd.drawEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
}



