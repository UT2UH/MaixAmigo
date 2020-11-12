#include <MaixAmigo.h>

void setup() {
  MA.begin();
  MA.Lcd.fillScreen(TFT_WHITE);
}

void loop() {
  MA.update();
  Event& e = MA.Buttons.event;
  if (e & (E_MOVE | E_RELEASE)) circle(e & E_MOVE ? e.from : e.to, TFT_WHITE);
  if (e & (E_TOUCH | E_MOVE)) circle(e.to, e.finger ? TFT_BLUE : TFT_RED);
}

void circle(Point p, uint16_t c) {
  MA.Lcd.drawCircle(p.x, p.y, 50, c);
  MA.Lcd.drawCircle(p.x, p.y, 52, c);
}
