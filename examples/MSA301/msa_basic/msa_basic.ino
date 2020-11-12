#include <MaixAmigo.h>

acc_t data;

// This is the length of the string that will be created
// included minus and decimal point
const signed char formattedStringLength = 11;

//  The number of digits after the deimal point to print
const unsigned char numVarsAfterDecimal = 6;

static char outstr[formattedStringLength];

char * formatValue(float value)
{
  dtostrf(value, formattedStringLength, numVarsAfterDecimal, outstr);
  return outstr;
}

void setup()
{
  MA.begin();
  pinMode(BUTTON_B_PIN, INPUT);
  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.setTextColor(TFT_YELLOW);  
  MA.Lcd.setTextSize(2);
  MA.Lcd.println("MaixAmigo MSA301 test!");
}

void loop()
{
  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  data = MA.Msa.getAcceleration();

  MA.Lcd.printf(" Xa:%s\n", formatValue(data.x));
  MA.Lcd.printf(" Ya:%s\n", formatValue(data.y));
  MA.Lcd.printf(" Za:%s\n", formatValue(data.z));
  MA.Lcd.println();

  if (digitalRead(BUTTON_B_PIN) == 0) {
    MA.Lcd.printf("powerOff()\n");
    delay(1000);
    MA.Axp.powerOff();
  }
  delay(100);
}