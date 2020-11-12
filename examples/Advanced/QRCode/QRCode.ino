#include <MaixAmigo.h>

void setup() {
  MA.begin();

  // Display QRCode
  MA.Lcd.qrcode("http://www.m5stack.com");
  // MA.Lcd.qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
}

void loop() {
}
