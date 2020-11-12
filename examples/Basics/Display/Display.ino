#include <MaixAmigo.h>

// the setup routine runs once when M5Stack starts up
void setup() {

  // initialize the M5Stack object
  MA.begin();

  // Lcd display
  MA.Lcd.fillScreen(TFT_WHITE);
  delay(500);
  MA.Lcd.fillScreen(TFT_RED);
  delay(500);
  MA.Lcd.fillScreen(TFT_GREEN);
  delay(500);
  MA.Lcd.fillScreen(TFT_BLUE);
  delay(500);
  MA.Lcd.fillScreen(TFT_BLACK);
  delay(500);

  // text print
  MA.Lcd.fillScreen(TFT_BLACK);
  MA.Lcd.setCursor(10, 10);
  MA.Lcd.setTextColor(TFT_WHITE);
  MA.Lcd.setTextSize(1);
  MA.Lcd.printf("Display Test!");

  // draw graphic
  delay(1000);
  MA.Lcd.drawRect(100, 100, 50, 50, TFT_BLUE);
  delay(1000);
  MA.Lcd.fillRect(100, 100, 50, 50, TFT_BLUE);
  delay(1000);
  MA.Lcd.drawCircle(100, 100, 50, TFT_RED);
  delay(1000);
  MA.Lcd.fillCircle(100, 100, 50, TFT_RED);
  delay(1000);
  MA.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, TFT_YELLOW);
  delay(1000);
  MA.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, TFT_YELLOW);

}

// the loop routine runs over and over again forever
void loop(){

  //rand draw
  MA.Lcd.fillTriangle(random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(0xfffe));

  MA.update();
}
