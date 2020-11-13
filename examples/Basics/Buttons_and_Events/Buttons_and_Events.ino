#include <MaixAmigo.h>

void setup() {
  MA.begin();
  MA.BtnA.setLabel("Test");
  MA.BtnB.setLabel("Wow !");
  MA.BtnC.setLabel("Amazing !");
  MA.BtnA.off = MA.BtnB.off = MA.BtnC.off = {BLUE, WHITE, NODRAW};
  MA.BtnA.on = MA.BtnB.on = MA.BtnC.on = {RED, WHITE, NODRAW};
  MA.Buttons.addHandler(eventDisplay);
  MA.Buttons.draw();
}

void loop() {
	MA.update();
}

void eventDisplay(Event& e) {
  Serial.printf("\n%-12s %-18s", e.typeName(), e.objName());
  if (e.type == E_RELEASE || e.type == E_PRESSED) Serial.printf("%5d ms", e.duration);
}