/*
  This sketch has been written to test the Processing screenshot client.

  It has been created to work with the TFT_eSPI library here:
  https://github.com/Bodmer/TFT_eSPI

  It sends screenshots to a PC running a Processing client sketch.

  The Processing IDE that will run the client sketch can be downloaded
  here:  https://processing.org/

  The Processing sketch needed is contained within a tab attached to this
  Arduino sketch. Cut and copy that tab into the Processing IDE and run.
  
  This sketch uses the GLCD, 2, 4, 6 fonts only.

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  Maximum recommended SPI clock rate is 27MHz when reading pixels, 40MHz
  seems to be OK with ILI9341 displays but this is above the manufacturers
  specifed maximum clock rate.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

// Created by: Bodmer  5/3/17
// Updated by: Bodmer 10/3/17
// Version: 0.06

// MIT licence applies, all text above must be included in derivative works

#include <MaixAmigo.h> 

unsigned long targetTime = 0;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11; // Colour order is RGB 5+6+5 bits each

void setup(void) {

  MA.begin();
  Serial.begin(921600);

  MA.Lcd.fillScreen(TFT_BLACK);
  
  targetTime = millis() + 1000;
}

void loop() {

  if (targetTime < millis()) {
    targetTime = millis() + 1500; // Wait a minimum of 1.5s
    
    MA.Lcd.setRotation(random(4));
    rainbow_fill(); // Fill the screen with rainbow colours

    MA.Lcd.setTextColor(TFT_BLACK); // Text background is not defined so it is transparent
    MA.Lcd.setTextDatum(TC_DATUM);  // Top Centre datum
    int xpos = MA.Lcd.width()/2;    // Centre of screen
    
    MA.Lcd.setTextFont(0);        // Select font 0 which is the Adafruit font
    MA.Lcd.drawString("Original Adafruit font!", xpos, 5);

    // The new larger fonts do not need to use the .setCursor call, coords are embedded
    MA.Lcd.setTextColor(TFT_BLACK); // Do not plot the background colour
    
    // Overlay the black text on top of the rainbow plot (the advantage of not drawing the backgorund colour!)
    MA.Lcd.drawString("Font size 2", xpos, 14, 2); // Draw text centre at position xpos, 14 using font 2
    MA.Lcd.drawString("Font size 4", xpos, 30, 4); // Draw text centre at position xpos, 30 using font 4
    MA.Lcd.drawString("12.34", xpos, 54, 6);       // Draw text centre at position xpos, 54 using font 6

    MA.Lcd.drawString("12.34 is in font size 6", xpos, 92, 2); // Draw text centre at position xpos, 92 using font 2
    // Note the x position is the top of the font!

    // draw a floating point number
    float pi = 3.1415926; // Value to print
    int precision = 3;    // Number of digits after decimal point

    int ypos = 110;     // y position

    MA.Lcd.setTextDatum(TR_DATUM); // Top Right datum so text butts neatly to xpos (right justified)

    MA.Lcd.drawFloat(pi, precision, xpos, ypos, 2); // Draw rounded number and return new xpos delta for next print position

    MA.Lcd.setTextDatum(TL_DATUM);  // Top Left datum so text butts neatly to xpos (left justified)

    MA.Lcd.drawString(" is pi", xpos, ypos, 2);

    MA.Lcd.setTextSize(1);           // We are using a font size multiplier of 1
    MA.Lcd.setTextDatum(TC_DATUM);   // Top Centre datum
    MA.Lcd.setTextColor(TFT_BLACK);  // Set text colour to black, no background (so transparent)

    MA.Lcd.drawString("Transparent...", xpos, 125, 4);  // Font 4

    MA.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);          // Set text colour to white and background to black
    MA.Lcd.drawString("White on black", xpos, 150, 4);  // Font 4

    MA.Lcd.setTextColor(TFT_GREEN, TFT_BLACK); // This time we will use green text on a black background

    MA.Lcd.setTextFont(2); // Select font 2, now we do not need to specify the font in drawString()

    // An easier way to position text and blank old text is to set the datum and use width padding
    MA.Lcd.setTextDatum(BC_DATUM);          // Bottom centre for text datum
    MA.Lcd.setTextPadding(MA.Lcd.width() + 1); // Pad text to full screen width + 1 spare for +/-1 position rounding
    
    MA.Lcd.drawString("Ode to a Small Lump of Green Putty", xpos, 230 - 32);
    MA.Lcd.drawString("I Found in My Armpit One Midsummer", xpos, 230 - 16);
    MA.Lcd.drawString("Morning", xpos, 230);
    
    MA.Lcd.setTextDatum(TL_DATUM); // Reset to top left for text datum
    MA.Lcd.setTextPadding(0);      // Reset text padding to 0 pixels

    // Now call the screen server to send a copy of the TFT screen to the PC running the Processing client sketch
    screenServer();
  }
}

// Fill screen with a rainbow pattern
void rainbow_fill()
{
  // The colours and state are not initialised so the start colour changes each time the funtion is called
  int rotation = MA.Lcd.getRotation();
  MA.Lcd.setRotation(random(4));
  for (int i = MA.Lcd.height() - 1; i >= 0; i--) {
    // This is a "state machine" that ramps up/down the colour brightnesses in sequence
    switch (state) {
      case 0:
        green ++;
        if (green == 64) {
          green = 63;
          state = 1;
        }
        break;
      case 1:
        red--;
        if (red == 255) {
          red = 0;
          state = 2;
        }
        break;
      case 2:
        blue ++;
        if (blue == 32) {
          blue = 31;
          state = 3;
        }
        break;
      case 3:
        green --;
        if (green == 255) {
          green = 0;
          state = 4;
        }
        break;
      case 4:
        red ++;
        if (red == 32) {
          red = 31;
          state = 5;
        }
        break;
      case 5:
        blue --;
        if (blue == 255) {
          blue = 0;
          state = 0;
        }
        break;
    }
    colour = red << 11 | green << 5 | blue;
    // Draw a line 1 pixel wide in the selected colour
    MA.Lcd.drawFastHLine(0, i, MA.Lcd.width(), colour); // in this example MA.Lcd.width() returns the pixel width of the display
  }
  MA.Lcd.setRotation(rotation);
}



