#include <MaixAmigo.h>
#include "utility/MPU9250.h"
#include "WiFi.h"

extern const unsigned char gImage_logoM5[];
extern const unsigned char m5stack_startup_music[];

#ifndef min
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

MPU9250 IMU;

void startupLogo() {
    static uint8_t brightness, pre_brightness;
    uint32_t length = strlen((char*)m5stack_startup_music);
    MA.Lcd.setBrightness(0);
    MA.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
    for(int i=0; i<length; i++) {
        dacWrite(SPEAKER_PIN, m5stack_startup_music[i]>>2);
        delayMicroseconds(40);
        brightness = (i/157);
        if(pre_brightness != brightness) {
            pre_brightness = brightness;
            MA.Lcd.setBrightness(brightness);
        }
    }

    for(int i=255; i>=0; i--) {
        MA.Lcd.setBrightness(i);
        if(i<=32) {
            dacWrite(SPEAKER_PIN, i);
        }
        delay(2);
    }

    MA.Lcd.fillScreen(TFT_BLACK);
    delay(800);
    for(int i=0; i>=100; i++) {
        MA.Lcd.setBrightness(i);
        delay(2);
    }
}

//TF card test
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);
    MA.Lcd.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        MA.Lcd.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        MA.Lcd.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            MA.Lcd.print("  DIR : ");
            Serial.println(file.name());
            MA.Lcd.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            MA.Lcd.print("  FILE: ");
            Serial.print(file.name());
            MA.Lcd.print(file.name());
            Serial.print("  SIZE: ");
            MA.Lcd.print("  SIZE: ");
            Serial.println(file.size());
            MA.Lcd.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\n", path);
    MA.Lcd.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        MA.Lcd.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    MA.Lcd.print("Read from file: ");
    while(file.available()){
        int ch = file.read();
        Serial.write(ch);
        MA.Lcd.write(ch);
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);
    MA.Lcd.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        MA.Lcd.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
        MA.Lcd.println("File written");
    } else {
        Serial.println("Write failed");
        MA.Lcd.println("Write failed");
    }
}

void buttons_test() {
    if(MA.BtnA.wasPressed()) {
        MA.Lcd.printf("A");
        Serial.printf("A");
    }
    if(MA.BtnB.wasPressed()) {
        MA.Lcd.printf("B");
        Serial.printf("B");
    } 
    if(MA.BtnC.wasPressed()) {
        MA.Lcd.printf("C");
        Serial.printf("C");
    } 
}

void wifi_test() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("scan start");
    MA.Lcd.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    MA.Lcd.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        MA.Lcd.println("no networks found");
    } else {
        Serial.print(n);
        MA.Lcd.print(n);
        Serial.println(" networks found");
        MA.Lcd.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            MA.Lcd.print(i + 1);
            Serial.print(": ");
            MA.Lcd.print(": ");
            Serial.print(WiFi.SSID(i));
            MA.Lcd.print(WiFi.SSID(i));
            Serial.print(" (");
            MA.Lcd.print(" (");
            Serial.print(WiFi.RSSI(i));
            MA.Lcd.print(WiFi.RSSI(i));
            Serial.print(")");
            MA.Lcd.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            MA.Lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(5);
        }
    }
    Serial.println("");
    MA.Lcd.println("");
}

/*
bool gpio_test_flg = 0;
void GPIO_test() {
    // uint8_t gpio_table[] = {23,19,18,3,16,21,2,12,15,26,1,17,22,5,13,0,34};
    uint8_t gpio_table[] = {12,2,21,16,3,18,19,23,15,0,13,5,22,17,1,26,25};
    
    // while(1) 
    {
        for (int i = 0; i<=sizeof(gpio_table) / sizeof(gpio_table[0]); i++) {
            pinMode(gpio_table[i], OUTPUT);
        }
        for(int i=0; i<=sizeof(gpio_table)/sizeof(gpio_table[0]); i++) {
            digitalWrite(gpio_table[i], 1);
            delay(50);
            digitalWrite(gpio_table[i], 0);
            delay(50);
            digitalWrite(gpio_table[i], 1);
            delay(50);
            digitalWrite(gpio_table[i], 0);
            delay(50);
        }
    }
}

void adc_test() {
    int count = 10;
    pinMode(35, INPUT);
    pinMode(36, INPUT);
    pinMode(34, INPUT);
    MA.Lcd.fillScreen(TFT_BLACK);
    while(count--) {
        MA.Lcd.setCursor(0, 10);
        MA.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
        MA.Lcd.setTextSize(2);
        MA.Lcd.printf("ADC35:%d\r\nADC36:%d\r\nADC34:%d\r\n", analogRead(35), analogRead(36), analogRead(34));
        delay(500);
    }
}
*/
unsigned long testLines(uint16_t color)
{
    unsigned long start, t;
    int x1, y1, x2, y2,
        w = MA.Lcd.width(),
        h = MA.Lcd.height();

    MA.Lcd.fillScreen(TFT_BLACK);

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    t = micros() - start; // fillScreen doesn't count against timing

    MA.Lcd.fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    MA.Lcd.fillScreen(TFT_BLACK);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    MA.Lcd.fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        MA.Lcd.drawLine(x1, y1, x2, y2, color);

    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
    unsigned long start;
    int x, y, w = MA.Lcd.width(), h = MA.Lcd.height();

    MA.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (y = 0; y < h; y += 5)
        MA.Lcd.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5)
        MA.Lcd.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color)
{
    unsigned long start;
    int n, i, i2,
        cx = MA.Lcd.width() / 2,
        cy = MA.Lcd.height() / 2;

    MA.Lcd.fillScreen(TFT_BLACK);
    n = min(MA.Lcd.width(), MA.Lcd.height());
    start = micros();
    for (i = 2; i < n; i += 6)
    {
        i2 = i / 2;
        MA.Lcd.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
    unsigned long start, t = 0;
    int n, i, i2,
        cx = MA.Lcd.width() / 2 - 1,
        cy = MA.Lcd.height() / 2 - 1;

    MA.Lcd.fillScreen(TFT_BLACK);
    n = min(MA.Lcd.width(), MA.Lcd.height());
    for (i = n - 1; i > 0; i -= 6)
    {
        i2 = i / 2;
        start = micros();
        MA.Lcd.fillRect(cx - i2, cy - i2, i, i, color1);
        t += micros() - start;
        // Outlines are not included in timing results
        MA.Lcd.drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, w = MA.Lcd.width(), h = MA.Lcd.height(), r2 = radius * 2;

    MA.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (x = radius; x < w; x += r2)
    {
        for (y = radius; y < h; y += r2)
        {
            MA.Lcd.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, r2 = radius * 2,
              w = MA.Lcd.width() + radius,
              h = MA.Lcd.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2)
    {
        for (y = 0; y < h; y += r2)
        {
            MA.Lcd.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles()
{
    unsigned long start;
    int n, i, cx = MA.Lcd.width() / 2 - 1,
              cy = MA.Lcd.height() / 2 - 1;

    MA.Lcd.fillScreen(TFT_BLACK);
    n = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5)
    {
        MA.Lcd.drawTriangle(
            cx, cy - i,     // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            MA.Lcd.color565(0, 0, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles()
{
    unsigned long start, t = 0;
    int i, cx = MA.Lcd.width() / 2 - 1,
           cy = MA.Lcd.height() / 2 - 1;

    MA.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5)
    {
        start = micros();
        MA.Lcd.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                            MA.Lcd.color565(0, i, i));
        t += micros() - start;
        MA.Lcd.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                            MA.Lcd.color565(i, i, 0));
    }

    return t;
}

unsigned long testRoundRects()
{
    unsigned long start;
    int w, i, i2,
        cx = MA.Lcd.width() / 2 - 1,
        cy = MA.Lcd.height() / 2 - 1;

    MA.Lcd.fillScreen(TFT_BLACK);
    w = min(MA.Lcd.width(), MA.Lcd.height());
    start = micros();
    for (i = 0; i < w; i += 6)
    {
        i2 = i / 2;
        MA.Lcd.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, MA.Lcd.color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects()
{
    unsigned long start;
    int i, i2,
        cx = MA.Lcd.width() / 2 - 1,
        cy = MA.Lcd.height() / 2 - 1;

    MA.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (i = min(MA.Lcd.width(), MA.Lcd.height()); i > 20; i -= 6)
    {
        i2 = i / 2;
        MA.Lcd.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, MA.Lcd.color565(0, i, 0));
    }

    return micros() - start;
}

// the setup routine runs once when M5Stack starts up
void setup() {
    
    //gpio test 
    // pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    // if(digitalRead(BUTTON_A_PIN) == 0) {
    //     gpio_test_flg = 1;
    // }

    // if (gpio_test_flg) {
    //     GPIO_test();
    // }

    // initialize the M5Stack object
    MA.begin();

    // dac test
    // if (gpio_test_flg)
    // {
    //     adc_test();
    // }

    startupLogo();
    Wire.begin();

    // Lcd display
    MA.Lcd.setBrightness(100);
    MA.Lcd.fillScreen(TFT_BLACK);
    MA.Lcd.setCursor(10, 10);
    MA.Lcd.setTextColor(TFT_WHITE);
    MA.Lcd.setTextSize(1);
    MA.Lcd.printf("Display Test!");
    delay(300);

    MA.Lcd.fillScreen(TFT_WHITE);
    delay(150);
    MA.Lcd.fillScreen(TFT_RED);
    delay(150);
    MA.Lcd.fillScreen(TFT_GREEN);
    delay(150);
    MA.Lcd.fillScreen(TFT_BLUE);
    delay(150);
    MA.Lcd.fillScreen(TFT_BLACK);
    delay(150);

    // draw graphic
    yield();
    Serial.print(F("Lines                    "));
    yield();
    Serial.println(testLines(TFT_CYAN));
    //total+=testLines(TFT_CYAN);
    //delay(500);

    yield();
    Serial.print(F("Horiz/Vert Lines         "));
    yield();
    Serial.println(testFastLines(TFT_RED, TFT_BLUE));
    //total+=testFastLines(TFT_RED, TFT_BLUE);
    //delay(500);

    yield();
    Serial.print(F("Rectangles (outline)     "));
    yield();
    Serial.println(testRects(TFT_GREEN));
    //total+=testRects(TFT_GREEN);
    //delay(500);

    yield();
    Serial.print(F("Rectangles (filled)      "));
    yield();
    Serial.println(testFilledRects(TFT_YELLOW, TFT_MAGENTA));
    //total+=testFilledRects(TFT_YELLOW, TFT_MAGENTA);
    //delay(500);

    yield();
    Serial.print(F("Circles (filled)         "));
    yield();
    Serial.println(testFilledCircles(10, TFT_MAGENTA));
    //total+= testFilledCircles(10, TFT_MAGENTA);

    yield();
    Serial.print(F("Circles (outline)        "));
    yield();
    Serial.println(testCircles(10, TFT_WHITE));
    //total+=testCircles(10, TFT_WHITE);
    //delay(500);

    yield();
    Serial.print(F("Triangles (outline)      "));
    yield();
    Serial.println(testTriangles());
    //total+=testTriangles();
    //delay(500);

    yield();
    Serial.print(F("Triangles (filled)       "));
    yield();
    Serial.println(testFilledTriangles());
    //total += testFilledTriangles();
    //delay(500);

    yield();
    Serial.print(F("Rounded rects (outline)  "));
    yield();
    Serial.println(testRoundRects());
    //total+=testRoundRects();
    //delay(500);

    yield();
    Serial.print(F("Rounded rects (filled)   "));
    yield();
    Serial.println(testFilledRoundRects());
    //total+=testFilledRoundRects();
    //delay(500);

    yield();
    Serial.println(F("Done!"));
    yield();

    //rand draw 
    int i = 250;
    while(--i) {
        MA.Lcd.fillTriangle(random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(MA.Lcd.width()-1), random(MA.Lcd.height()-1), random(0xfffe));
    }
    for(int i=255; i>=0; i--) {
        MA.Lcd.setBrightness(i);
        delay(2);
    }

    //wifi test
    MA.Lcd.setCursor(0, 10);
    MA.Lcd.fillScreen(TFT_BLACK);
    for(int i=0; i<200; i++) {
        MA.Lcd.setBrightness(i);
        delay(2);
    }

    byte c = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
    Serial.print(" I should be "); Serial.println(0x71, HEX);
    Serial.println("");
    MA.Lcd.setCursor(20,0); MA.Lcd.print("MPU9250");
    MA.Lcd.setCursor(0,10); MA.Lcd.print("I AM");
    MA.Lcd.setCursor(0,20); MA.Lcd.print(c, HEX);
    MA.Lcd.setCursor(0,30); MA.Lcd.print("I Should Be");
    MA.Lcd.setCursor(0,40); MA.Lcd.println(0x71, HEX);
    MA.Lcd.println();
    delay(100);

    IMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = IMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    Serial.print(" I should be "); Serial.println(0x48, HEX);

    // MA.Lcd.fillScreen(TFT_BLACK);
    MA.Lcd.setCursor(20,100); MA.Lcd.print("AK8963");
    MA.Lcd.setCursor(0,110); MA.Lcd.print("I AM");
    MA.Lcd.setCursor(0,120); MA.Lcd.print(d, HEX);
    MA.Lcd.setCursor(0,130); MA.Lcd.print("I Should Be");
    MA.Lcd.setCursor(0,140); MA.Lcd.print(0x48, HEX);
    delay(1000);

    MA.Lcd.setCursor(0, 0);
    MA.Lcd.println("wifi test:");
    MA.Lcd.fillScreen(TFT_BLACK);
    wifi_test();
    delay(2000);

    // TF card test
    MA.Lcd.fillScreen(TFT_BLACK);
    MA.Lcd.setCursor(0, 10);
    MA.Lcd.printf("TF card test:\r\n");
    // digitalWrite(TFT_CS, 1);
    listDir(SD, "/", 0);
    writeFile(SD, "/hello.txt", "Hello world");
    readFile(SD, "/hello.txt");

    //Button test
    MA.Lcd.println();
    MA.Lcd.println();
    MA.Lcd.print("buttons Test:");
    MA.Lcd.setTextColor(TFT_RED);
}

// the loop routine runs over and over again forever
void loop(){
    buttons_test();
    MA.update();
}
