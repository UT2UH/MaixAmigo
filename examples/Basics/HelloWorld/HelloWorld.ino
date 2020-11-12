#include <MaixAmigo.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  MA.begin();

  // LCD display
  MA.Lcd.print("Hello World");
}

// the loop routine runs over and over again forever
void loop() {

}
