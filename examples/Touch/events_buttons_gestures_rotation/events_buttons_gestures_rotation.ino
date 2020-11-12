#include <MaixAmigo.h>

// Defines gestures
Gesture swipeRight("swipe right", 160, DIR_RIGHT, 30, true);
Gesture swipeDown("swipe down", 120, DIR_DOWN, 30, true);
Gesture swipeLeft("swipe left", 160, DIR_LEFT, 30, true);
Gesture swipeUp("swipe up", 120, DIR_UP, 30, true);

// Defines the buttons. Colors in format {bg, text, outline}
ButtonColors on_clrs = {TFT_RED, TFT_WHITE, TFT_WHITE};
ButtonColors off_clrs = {TFT_BLACK, TFT_WHITE, TFT_WHITE};
Button tl(0, 0, 0, 0, false ,"top-left", off_clrs, on_clrs, TL_DATUM);
Button bl(0, 0, 0, 0, false, "bottom-left", off_clrs, on_clrs, BL_DATUM);
Button tr(0, 0, 0, 0, false, "top-right", off_clrs, on_clrs, TR_DATUM);
Button br(0, 0, 0, 0, false, "bottom-right", off_clrs, on_clrs, BR_DATUM);

// For performance measurement (Single tap on bottom-right button)
uint32_t startTime;
uint32_t times = 0;

void setup() {
  MA.begin();
  MA.Buttons.addHandler(doRotation, E_GESTURE);
  MA.Buttons.addHandler(toggleColor, E_DBLTAP);
  MA.Buttons.addHandler(eventDisplay, E_ALL - E_MOVE);
  br.addHandler(showPerformance, E_TAP);
  br.repeatDelay = 1000;
  doButtons();
  startTime = millis();
}

void loop() {
  MA.update();
  times++;
}

// Positions the buttons and draws them. (Only because height and width
// change. If we were only switching between normal and upside-down we
// would only need the Buttons.draw() here.)
void doButtons() {
  int16_t hw = MA.Lcd.width() / 2;
  int16_t hh = MA.Lcd.height() / 2;
  tl.set(0, 0, hw - 5, hh - 5);
  bl.set(0, hh + 5, hw - 5, hh - 5);
  tr.set(hw + 5, 0, hw - 5, hh - 5);
  br.set(hw + 5, hh + 5, hw - 5, hh - 5);
  MA.Buttons.draw();
}

void doRotation(Event& e) {
  // Gestures and Buttons have an instanceIndex() that starts at zero
  // so by defining the gestures in the right order I can use that as
  // the input for MA.Lcd.setRotation.
  uint8_t new_rotation = e.gesture->instanceIndex();
  if (new_rotation != MA.Lcd.rotation) {
    MA.Lcd.clearDisplay();
    MA.Lcd.setRotation(new_rotation);
    doButtons();
  }
}

void toggleColor(Event& e) {
  // Just so we can type "b." instead of "e.button->"
  Button& b = *e.button;

  if (b != MA.background) {
    // Toggles the button color between black and blue
    b.off.bg = (b.off.bg == TFT_BLACK) ? TFT_BLUE : TFT_BLACK;
    b.draw();
  }
}

void showPerformance(Event& e) {
  Serial.printf("%d in %d ms, average MA.update() took %.2f microseconds\n",
   times, millis() - startTime, (float)((millis() - startTime) * 1000) / times);
  startTime = millis();
  times = 0;
}

void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,
                e.to.x, e.to.y);
  Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),
                e.distance(), e.duration);
}
