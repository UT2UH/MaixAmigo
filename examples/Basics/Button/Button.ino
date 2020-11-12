/*
    Name:       button.ino
    Created:    2018/9/21 14:06:15
    Author:     sakabin
*/

#include <MaixAmigo.h>
// The setup() function runs once each time the micro-controller starts
void setup() {
  // init lcd, serial, but don't init sd card
  MA.begin(true, false, true);
  MA.Lcd.clear(TFT_BLACK);
  MA.Lcd.setTextColor(TFT_YELLOW);
  MA.Lcd.setTextSize(2);
  MA.Lcd.setCursor(65, 10);
  MA.Lcd.println("Button example");
  MA.Lcd.setCursor(3, 35);
  MA.Lcd.println("Press button B for 700ms");
  MA.Lcd.println("to clear screen.");
  MA.Lcd.setTextColor(TFT_RED);
}

// Add the main program code into the continuous loop() function
void loop() {
  MA.update();
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (MA.BtnA.wasReleased()) {
    MA.Lcd.print('A');
  } else if (MA.BtnB.wasReleased()) {
    MA.Lcd.print('B');
  } else if (MA.BtnC.wasReleased()) {
    MA.Lcd.print('C');
  } else if (MA.BtnB.wasReleasefor(700)) {
    MA.Lcd.clear(TFT_BLACK);
    MA.Lcd.setCursor(0, 0);
  }
}
