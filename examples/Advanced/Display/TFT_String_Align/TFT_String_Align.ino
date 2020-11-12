/*
Tests string alignment

Normally strings are printed relative to the top left corner but this can be
changed with the setTextDatum() function. The library has #defines for:

TL_DATUM = Top left
TC_DATUM = Top centre
TR_DATUM = Top right
ML_DATUM = Middle left
MC_DATUM = Middle centre
MR_DATUM = Middle right
BL_DATUM = Bottom left
BC_DATUM = Bottom centre
BR_DATUM = Bottom right

 
 Needs fonts 2, 4, 6, 7 and 8

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */

#include <MaixAmigo.h>

unsigned long drawTime = 0;

void setup(void) {
  MA.begin();
}

void loop() {

  MA.Lcd.fillScreen(TFT_BLACK);
  
  for(byte datum = 0; datum < 9; datum++) {
    MA.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    
    MA.Lcd.setTextDatum(datum);
    
    MA.Lcd.drawNumber(88,160,120,8);
    MA.Lcd.fillCircle(160,120,5,TFT_RED);
    
    MA.Lcd.setTextDatum(MC_DATUM);
    
    MA.Lcd.setTextColor(TFT_BLACK);
    MA.Lcd.drawString("X",160,120,2);
    delay(1000);
    MA.Lcd.fillScreen(TFT_BLACK);
  }

  MA.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
  MA.Lcd.drawCentreString("69",160,120,8);
  MA.Lcd.fillCircle(160,120,5,TFT_YELLOW);
  
  MA.Lcd.setTextDatum(MC_DATUM);
  
  MA.Lcd.setTextColor(TFT_BLACK);
  MA.Lcd.drawString("X",160,120,2);
  delay(1000);
  MA.Lcd.fillScreen(TFT_BLACK);
  
  MA.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  MA.Lcd.drawRightString("88",160,120,8);
  MA.Lcd.fillCircle(160,120,5,TFT_YELLOW);
  
  MA.Lcd.setTextDatum(MC_DATUM);
  
  MA.Lcd.setTextColor(TFT_BLACK);
  MA.Lcd.drawString("X",160,120,2);
  delay(1000);
  MA.Lcd.fillScreen(TFT_BLACK);

  MA.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);

  MA.Lcd.setTextDatum(MC_DATUM);

  //Test floating point drawing function
  float test = 67.125;
  MA.Lcd.drawFloat(test, 4, 160, 180, 4);
  delay(1000);
  MA.Lcd.fillScreen(TFT_BLACK);
  test = -0.555555;
  MA.Lcd.drawFloat(test, 3, 160, 180, 4);
  delay(1000);
  MA.Lcd.fillScreen(TFT_BLACK);
  test = 0.1;
  MA.Lcd.drawFloat(test, 4, 160, 180, 4);
  delay(1000);
  MA.Lcd.fillScreen(TFT_BLACK);
  test = 9999999;
  MA.Lcd.drawFloat(test, 1, 160, 180, 4);
  delay(1000);
  
  MA.Lcd.fillCircle(160,180,5,TFT_YELLOW);
  
  MA.Lcd.setTextDatum(MC_DATUM);
  
  MA.Lcd.setTextColor(TFT_BLACK);
  MA.Lcd.drawString("X",160,180,2);
 
  delay(4000);
}








