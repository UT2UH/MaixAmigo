#include <MaixAmigo.h>

void setup() {
  MA.begin();
  MA.Power.begin();
  MA.Lcd.setBrightness(200);

  MA.Power.setWakeupButton(BUTTON_A_PIN);
  if(!MA.Power.canControl()) {
    MA.Lcd.printf("IP5306 is not i2c version\n");
  }
}

void loop() {

  MA.update();
  bool c=MA.Power.isResetbyPowerSW();
  bool d=MA.Power.isResetbyDeepsleep();
 
  MA.Lcd.printf("<<Sleep test>>\n");
  MA.Lcd.printf("power-on triggered at:%s%s\n\n",c?"POWER-SW":"",d?"DeepSleep-end":"");
  
  MA.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call MA.Power.setPowerBoostKeepOn(false)
  MA.Power.lightSleep(SLEEP_SEC(5));
  
  MA.Lcd.printf("Go lightSleep (press buttonA wake up)\n");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call MA.Power.setPowerBoostKeepOn(false)
  MA.Power.lightSleep(0);
  
  MA.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call MA.Power.setPowerBoostKeepOn(false)
  MA.Power.deepSleep(0);
  
}
