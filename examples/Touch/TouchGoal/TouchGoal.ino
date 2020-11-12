#include <MaixAmigo.h>
#include "Goals.h"

// This program provides goal-oriented tested for MaixButtons

#define SCORE_TOP       200
#define SCORE_HEIGHT    32
#define SCORE_FONT      4
#define LONG_PRESS_TIME 500

// Defines gestures
Gesture swipeRight("swipe right", 80, DIR_RIGHT, 30, true);
Gesture swipeDown ("swipe down",  60, DIR_DOWN,  30, true);
Gesture swipeLeft ("swipe left",  80, DIR_LEFT,  30, true);
Gesture swipeUp   ("swipe up",    60, DIR_UP,    30, true);

ButtonColors on_clrs  = {TFT_BLACK, TFT_WHITE, TFT_WHITE};
ButtonColors off_clrs = {TFT_BLACK, TFT_WHITE, TFT_WHITE};

Button A(40,  80, 80, 80, false ,"A", off_clrs, on_clrs, MC_DATUM);
Button B(200, 80, 80, 80, false ,"B", off_clrs, on_clrs, MC_DATUM);


Goal* current_goal = nullptr;
Goal* goals[]      = { new TapAGoal(), new TapBGoal(), new DoubleTapAGoal(), new DoubleTapBGoal(),
                       new LongPressAGoal(), new LongPressBackgroundGoal(), new LongPressBGoal(),
                       new TapBackgroundGoal(), new DoubleTapBackgroundGoal(), new DragFromAtoBGoal(),
                       new DragFromBtoAGoal(), new DragFromAtoBackgroundGoal(), new DragFromBtoBackgroundGoal(),
                       new DragFromBackgroundtoAGoal(), new DragFromBackgroundtoBGoal(), new SwipeUpGoal(),
                       new SwipeDownGoal(), new SwipeLeftGoal(), new SwipeRightGoal()
                     };

void eventHandler(Event& e) {
  if(current_goal) current_goal->event_handler(e);
}

void show_score(int successes, int failures) {
  MA.Lcd.fillRect(0, SCORE_TOP, 320, SCORE_HEIGHT, TFT_NAVY);
  uint8_t datum = MA.Lcd.getTextDatum();
  MA.Lcd.setTextDatum(TL_DATUM);
  String str = "Pass: ";
  str += String(successes);
  MA.Lcd.drawString(str, 20, SCORE_TOP, SCORE_FONT);
  str = "Fail: ";
  str += String(failures);
  MA.Lcd.setTextDatum(TR_DATUM);
  MA.Lcd.drawString(str, 300, SCORE_TOP, SCORE_FONT);
  MA.Lcd.setTextDatum(datum);
}

void setup() {
  MA.begin();
  A.longPressTime = B.longPressTime = MA.background.longPressTime = LONG_PRESS_TIME;
  MA.Lcd.fillScreen(TFT_NAVY);
  MA.Lcd.setTextSize(1);
  MA.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
  MA.Lcd.drawCentreString("Goal Oriented Testing", TEXT_CENTER, TEXT_TOP, TEXT_FONT);
  MA.Buttons.addHandler(eventHandler, E_ALL);
  MA.Buttons.draw();
}

void loop() {
  int successes = 0;
  int failures  = 0;
  int len = sizeof(goals) / sizeof(Goal*);
  // Shuffle the goals
  for (int i=0; i < len; i++) {
    int n = random(0, len);  // Integer from 0 to len-1
    Goal* temp = goals[n];
    goals[n]   = goals[i];
    goals[i]   = temp;
  }
  
  for(int i = 0; i < len; i++) {
    delay(500);
    current_goal = goals[i];
    if(current_goal->test()) {
      successes++;
    }
    else {
      failures++;
    }
    current_goal = nullptr;
    MA.Lcd.fillRect(0, TEXT_TOP, 320, TEXT_HEIGHT, TFT_NAVY);
    show_score(successes, failures);
  }
  MA.Lcd.drawCentreString("Test Complete", TEXT_CENTER, TEXT_TOP, TEXT_FONT);
  if(failures) {
    MA.Lcd.fillRect(0, TEXT_TOP+TEXT_HEIGHT, 320, 240, TFT_NAVY);
    MA.Lcd.setCursor(20, TEXT_TOP + 50, TEXT_FONT);
    MA.Lcd.println("Failures:");
    for(uint8_t i = 0; i < len; i++) {
      if(!goals[i]->passed()) {
        MA.Lcd.print("      ");
        MA.Lcd.println(goals[i]->getName());
      }
    }
  }
  while(true) { delay(1000); }
}
