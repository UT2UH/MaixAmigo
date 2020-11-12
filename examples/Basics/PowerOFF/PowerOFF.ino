#include <MaixAmigo.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // initialize the M5Stack object
  MA.begin();

  // Lcd display
  MA.Lcd.println("This is software power off demo");
  MA.Lcd.println("Press the button A to power off.");
  
  // Set the wakeup button
  MA.setWakeupButton(BUTTON_A_PIN);
}

// the loop routine runs over and over again forever
void loop() {

  if(MA.BtnA.wasPressed()) {
    MA.powerOFF();
  }

  MA.update();
}
