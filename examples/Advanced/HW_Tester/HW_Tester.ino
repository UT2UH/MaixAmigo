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

int textColor=TFT_YELLOW;

void setup() 
{
  MA.begin();
  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.setTextColor(TFT_YELLOW);  
  MA.Lcd.setTextSize(2);

  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.println("Maix Amigo Tester");
  
  int address;
  int error;

  MA.Lcd.println("Scanning I2C addresses -");
  MA.Lcd.println("Internal:");
  for(address = 1; address < 127; address++ ){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if(error==0){
      MA.Lcd.print("0x");
      MA.Lcd.print(address,HEX); 
      MA.Lcd.print(" ");
    }
    delay(10);
  }

  pinMode(BUTTON_B_PIN, INPUT);

  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.println("MaixAmigo MSA301 test!");

  //MA.Msa.setDataRate(MSA30X_DATARATE_31_25_HZ);
  MA.Lcd.print("Data rate set to: ");
  switch (MA.Msa.getDataRate()) {
    case MSA30X_DATARATE_1_HZ: MA.Lcd.println("1 Hz"); break;
    case MSA30X_DATARATE_1_95_HZ: MA.Lcd.println("1.95 Hz"); break;
    case MSA30X_DATARATE_3_9_HZ: MA.Lcd.println("3.9 Hz"); break;
    case MSA30X_DATARATE_7_81_HZ: MA.Lcd.println("7.81 Hz"); break;
    case MSA30X_DATARATE_15_63_HZ: MA.Lcd.println("15.63 Hz"); break;
    case MSA30X_DATARATE_31_25_HZ: MA.Lcd.println("31.25 Hz"); break;
    case MSA30X_DATARATE_62_5_HZ: MA.Lcd.println("62.5 Hz"); break;
    case MSA30X_DATARATE_125_HZ: MA.Lcd.println("125 Hz"); break;
    case MSA30X_DATARATE_250_HZ: MA.Lcd.println("250 Hz"); break;
    case MSA30X_DATARATE_500_HZ: MA.Lcd.println("500 Hz"); break;
    case MSA30X_DATARATE_1000_HZ: MA.Lcd.println("1000 Hz"); break;
  }

  //MA.Msa.setPowerMode(MSA30X_SUSPENDMODE);
  MA.Lcd.print("Power mode set to: ");
  switch (MA.Msa.getMode()) {
    case MSA30X_MODE_NORMAL: MA.Lcd.println("Normal"); break;
    case MSA30X_MODE_LOW: MA.Lcd.println("Low Power"); break;
    case MSA30X_MODE_SUSPEND: MA.Lcd.println("Suspend"); break;
  }

//  //MA.Msa.setBandwidth(MSA30X_BANDWIDTH_31_25_HZ);
//  MA.Lcd.print("Bandwidth set to: ");
//  switch (MA.Msa.getBandwidth()) {
//    case MSA30X_BANDWIDTH_1_95_HZ: MA.Lcd.println("1.95 Hz"); break;
//    case MSA30X_BANDWIDTH_3_9_HZ: MA.Lcd.println("3.9 Hz"); break;
//    case MSA30X_BANDWIDTH_7_81_HZ: MA.Lcd.println("7.81 Hz"); break;
//    case MSA30X_BANDWIDTH_15_63_HZ: MA.Lcd.println("15.63 Hz"); break;
//    case MSA30X_BANDWIDTH_31_25_HZ: MA.Lcd.println("31.25 Hz"); break;
//    case MSA30X_BANDWIDTH_62_5_HZ: MA.Lcd.println("62.5 Hz"); break;
//    case MSA30X_BANDWIDTH_125_HZ: MA.Lcd.println("125 Hz"); break;
//    case MSA30X_BANDWIDTH_250_HZ: MA.Lcd.println("250 Hz"); break;
//    case MSA30X_BANDWIDTH_500_HZ: MA.Lcd.println("500 Hz"); break;
//  }

  //MA.Msa.setRange(MSA30X_RANGE_2_G);
  MA.Lcd.print("Range set to: ");
  switch (MA.Msa.getRange()) {
    case MSA30X_RANGE_2_G:  MA.Lcd.println("+-2G"); break;
    case MSA30X_RANGE_4_G:  MA.Lcd.println("+-4G"); break;
    case MSA30X_RANGE_8_G:  MA.Lcd.println("+-8G"); break;
    case MSA30X_RANGE_16_G: MA.Lcd.println("+-16G"); break;
  }

  //MA.Msa.setResolution(MSA30X_RES_14_BIT );
  MA.Lcd.print("Resolution set to: ");
  switch (MA.Msa.getResolution()) {
    case MSA30X_RES_14_BIT: MA.Lcd.println("14 bits"); break;
    case MSA30X_RES_12_BIT: MA.Lcd.println("12 bits"); break;
    case MSA30X_RES_10_BIT: MA.Lcd.println("10 bits"); break;
    case MSA30X_RES_8_BIT:  MA.Lcd.println("8 bits"); break;
  }

  while (digitalRead(BUTTON_B_PIN) != 0) {
  }
  
//  if(textColor==TFT_YELLOW) textColor=TFT_GREEN;
//  else textColor=TFT_YELLOW;
//  MA.Lcd.setTextColor(textColor,TFT_BLACK);
}

//void loop() {
//  MA.update();
//  if ( MA.Touch.changed ) MA.Lcd.println( MA.Touch.point[0] );
//}

void loop() {
  MA.Lcd.fillScreen( TFT_BLACK );
  MA.Lcd.setCursor(0, 0);
  MA.Lcd.printf(" BatVoltage : %7.2f\n", MA.Axp.getBatteryVoltage());
  MA.Lcd.printf(" BatChrgCurr: %7.2f\n", MA.Axp.getBatteryChargeCurrent());
  MA.Lcd.printf(" BatDischrgC: %7.2f\n", MA.Axp.getBatteryDischargeCurrent());
  MA.Lcd.printf(" AcinVolatge: %7.2f\n", MA.Axp.getAcinVolatge());
  MA.Lcd.printf(" AcinCurrent: %7.2f\n", MA.Axp.getAcinCurrent());
  MA.Lcd.printf(" VbusVoltage: %7.2f\n", MA.Axp.getVbusVoltage());
  MA.Lcd.printf(" VbusCurrent: %7.2f\n", MA.Axp.getVbusCurrent());
  MA.Lcd.printf(" InternlTemp: %7.2f\n", MA.Axp.getInternalTemperature());
  MA.Lcd.printf(" ApsVoltage : %7.2f\n", MA.Axp.getApsVoltage());
  MA.Lcd.printf(" PekPress   : %4d\n"  , MA.Axp.getPekPress());

  data = MA.Msa.getAcceleration();
  MA.Lcd.printf(" Xa:%s\n", formatValue(data.x));
  MA.Lcd.printf(" Ya:%s\n", formatValue(data.y));
  MA.Lcd.printf(" Za:%s\n", formatValue(data.z));
  MA.Lcd.println();

  if (digitalRead(BUTTON_B_PIN) == 0) {
    MA.Lcd.printf("powerOff()\n");
    //MA.Lcd.flush();
    delay(1000);
    MA.Axp.powerOff();
  }

  delay(3000);
}