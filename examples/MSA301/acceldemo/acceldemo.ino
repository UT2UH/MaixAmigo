// Basic demo for accelerometer readings from Adafruit MSA301
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

void setup(void) {
  MA.begin();
  pinMode(BUTTON_B_PIN, INPUT);
  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.setTextColor(TFT_YELLOW);  
  MA.Lcd.setTextSize(2);

  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.println("MaixAmigo MSA301 test!");

  //MA.Msa.setDataRate(MSA30X_DATARATE_31_25_HZ);
  Serial.print("Data rate set to: ");
  switch (MA.Msa.getDataRate()) {
    case MSA30X_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case MSA30X_DATARATE_1_95_HZ: Serial.println("1.95 Hz"); break;
    case MSA30X_DATARATE_3_9_HZ: Serial.println("3.9 Hz"); break;
    case MSA30X_DATARATE_7_81_HZ: Serial.println("7.81 Hz"); break;
    case MSA30X_DATARATE_15_63_HZ: Serial.println("15.63 Hz"); break;
    case MSA30X_DATARATE_31_25_HZ: Serial.println("31.25 Hz"); break;
    case MSA30X_DATARATE_62_5_HZ: Serial.println("62.5 Hz"); break;
    case MSA30X_DATARATE_125_HZ: Serial.println("125 Hz"); break;
    case MSA30X_DATARATE_250_HZ: Serial.println("250 Hz"); break;
    case MSA30X_DATARATE_500_HZ: Serial.println("500 Hz"); break;
    case MSA30X_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }

  //MA.Msa.setPowerMode(MSA30X_SUSPENDMODE);
  Serial.print("Power mode set to: ");
  switch (MA.Msa.getMode()) {
    case MSA30X_MODE_NORMAL: Serial.println("Normal"); break;
    case MSA30X_MODE_LOW: Serial.println("Low Power"); break;
    case MSA30X_MODE_SUSPEND: Serial.println("Suspend"); break;
  }

//  //MA.Msa.setBandwidth(MSA30X_BANDWIDTH_31_25_HZ);
//  Serial.print("Bandwidth set to: ");
//  switch (MA.Msa.getBandwidth()) {
//    case MSA30X_BANDWIDTH_1_95_HZ: Serial.println("1.95 Hz"); break;
//    case MSA30X_BANDWIDTH_3_9_HZ: Serial.println("3.9 Hz"); break;
//    case MSA30X_BANDWIDTH_7_81_HZ: Serial.println("7.81 Hz"); break;
//    case MSA30X_BANDWIDTH_15_63_HZ: Serial.println("15.63 Hz"); break;
//    case MSA30X_BANDWIDTH_31_25_HZ: Serial.println("31.25 Hz"); break;
//    case MSA30X_BANDWIDTH_62_5_HZ: Serial.println("62.5 Hz"); break;
//    case MSA30X_BANDWIDTH_125_HZ: Serial.println("125 Hz"); break;
//    case MSA30X_BANDWIDTH_250_HZ: Serial.println("250 Hz"); break;
//    case MSA30X_BANDWIDTH_500_HZ: Serial.println("500 Hz"); break;
//  }

  //MA.Msa.setRange(MSA30X_RANGE_2_G);
  Serial.print("Range set to: ");
  switch (MA.Msa.getRange()) {
    case MSA30X_RANGE_2_G:  Serial.println("+-2G"); break;
    case MSA30X_RANGE_4_G:  Serial.println("+-4G"); break;
    case MSA30X_RANGE_8_G:  Serial.println("+-8G"); break;
    case MSA30X_RANGE_16_G: Serial.println("+-16G"); break;
  }

  //MA.Msa.setResolution(MSA30X_RES_14_BIT );
  Serial.print("Resolution set to: ");
  switch (MA.Msa.getResolution()) {
    case MSA30X_RES_14_BIT: Serial.println("14 bits"); break;
    case MSA30X_RES_12_BIT: Serial.println("12 bits"); break;
    case MSA30X_RES_10_BIT: Serial.println("10 bits"); break;
    case MSA30X_RES_8_BIT:  Serial.println("8 bits"); break;
  }

  while (digitalRead(BUTTON_B_PIN) != 0) {
  }
}

void loop() {
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