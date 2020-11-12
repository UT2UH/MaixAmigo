/*

== MaixTouch - The MaixAmigo Touch Library ==

  This is the library behind the MA.Touch object that you can use to read
  from the touch sensor on the Maix Amigo. It was made to be an input
  source for the MaixButton library that provides higher level buttons,
  gestures and events, but both libraries can be also be used alone.


== About the Touch Sensor in the Maix Amigo ==

  Touchpanel interfacing is done by a FocalTech FT6336 chip, which supports
  two simultaneous touches. However, the Maix Amigo touch display is only
  multi-touch in one dimension. What that means is that it can detect two
  separate touches only if they occur on different vertical positions. This
  has to do with the way the touch screen is wired, it's not something that
  can be changed in software. So you will only ever see two points if they do
  not occur side-by-side. Touches that do happen side-by-side blend into one
  touch that is detected somewhere between the actual touches.

  While this limits multi-touch somewhat, you can still create multiple
  buttons and see two that are not on the same row simultaneously. You could
  also use one of the buttons below the screen as a modifier for something
  touched on the screen.

  The touch sensor extends to below the screen of the Maix Amigo: the sensor maps
  to 320x280 pixels, the screen is 320x240. The missing 40 pixels are placed
  below the screen, where the printed circles are. This is meant to simulate
  the three hardware buttons on the original M5Stack units. Note that on some
  units the touch sensor in this area only operates properly if the USB cable
  is plugged in or if the unit is placed firmly in your hand on a metal
  surface.

  For a quick view of how the sensor sees the world, try this sketch:

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

  (Don't worry if this all seems abracadabra now, we'll get to all of
  this is due time.)


== Point and Zone: Describing Points and Areas on the Screen ==

  The Point and Zone classes allow you to create variables that hold a point
  or an area on the screen. You can

  Point(x, y)

    Holds a point on the screen. Has members x and y that hold the coordinates
    of a touch. Values INVALID_VALUE for x and y indicate an invalid value,
    and that's what a point starts out with if you declare it without
    parameters. The 'valid()' method tests if a point is valid. If you
    explicitly evaluate a Point as a boolean ("if (p) ..."), you also get
    whether the point is valid, so this is equivalent to writing "if
    (p.valid()) ...".

  Zone(x, y, w, h)

    Holds a rectangular area on the screen. Members x, y, w and h are for the
    x and y coordinate of the top-left corner and the width and height of the
    rectangle.

  The 'set' method allows you to change the properties of an existing Point
  or Zone. Using the 'in' or 'contains' method you can test if a point lies
  in a zone.

  The PointAndZone library also provides the low-level support for direction
  from one point to another and for screen rotation translations.

  The documentation in src/utility/PointAndZone.h provides more details and
  examples.


== Basic Touch API ==

  The basic touch API provides a way to read the data from the touch sensor.


  MA.update()
    In the loop() part of your sketch, call "MA.update()". This will in turn
    call MA.Touch.update(), which is the only part that talks to the touch
    interface. It updates the data used by the rest of the API.

  MA.Touch.changed
    Is true if anything changed on the touchpad since the last time
    MA.update() was called.

  MA.Touch.points
    Contains the number of touches detected: 0, 1 or 2.

  MA.Touch.point[0], MA.Touch.point[1]
    MA.Touch.point[0] and MA.Touch.point[1] are Points that hold the detected
    touches.


  A very simple sketch to print the location where the screen is touched:

    #include <MaixAmigo.h>

    void setup() {
      MA.begin();
    }

    void loop() {
      MA.update();
      if ( MA.Touch.changed ) Serial.println( MA.Touch.point[0] );
    }


== Buttons, Gestures, Events ==

  Note that you may not want to use any of the above directly. The MaixButtons
  library provides button, gestures and events that allow you to quickly
  create reactive visual buttons on the screen and react differently based on
  whether a button was clicked, tapped, or double-tapped. Have a look at the
  documentation for that, which is in the MaixButton.h file in the src/utility
  directory of this repository. The examples under "File / Examples
  / MaixAmigo / Touch" in your Arduino environment should give you an
  idea of what's possible.


== Screen Rotation ==

  If you rotate the screen with MA.Lcd.setRotation, the touch coordinates
  will rotate along with it.

  * What that means is that either x or y for the area below the screen may
    go negative. Say you use the screen upside-down with
    MA.Lcd.setRotation(3). In that case the off-screen touch area (Y
    coordinates 240 through 279) that was below the screen now becomes above
    the screen and has Y coordinates -40 through -1.

  * See the MaixButton library for a feature that allows you to keep some Zone
    and Button objects in the same place on the physical screen, regardless
    of rotation.


== TFT_eSPI Resistive Touch API emulation ==

  While technically not part of this library itself, we added an emulation
  for the TFT_eSPI touch API to the MaixDisplay object that merely passes
  informaton on to the MA.Touch object. So MA.Lcd can be addressed as if it's
  a touch screen using that older resistive touch interface. Do note that
  this interface is not nearly as powerful as MA.Touch's native API. But
  together with MaixButton's TFT_eSPI_Button emulation, this should allow you
  to compile lots of ESP32 software written for touch screens.


== Advanced Uses of the Touch Library ==

  You should never need any of the below features in everyday use. But
  they're there just in case...

  MA.Touch.wasRead
    True if the sensor was actually read. The sensor can only provide updates
    every 13 milliseconds or so. MA.update() can loop as quick as once every
    20 MICROseconds, meaning it would continually read the sensor when there
    was nothing to read. So MA.Touch.read() only really reads when it's time
    to do so, and returns with MA.Touch.wasRead false otherwise.

  MA.point0finger
    The FT6336 chip keeps track of fingers, each touch has a finger ID of 0
    or 1. So when there are two touches in point[0] and point[1] and then one
    goes away, point0finger allows you to see which touch is left in
    point[0].

  MA.Touch.interval()
    Without arguments returns the current interval between sensor updates in
    milliseconds. If you supply a number that's the new interval. The default
    of 13 seems to give the most updates per second.

  MA.Touch.ft6336(reg)
  MA.Touch.ft6336(reg, value)
  MA.Touch.ft6336(reg, size, *data)
    Allows you to read and write registers on the ft6336 touch interface chip
    directly. The first form reads one byte, the second form writes one and
    the third form reads a block of 'size' bytes starting at 'reg' into a
    buffer at '*data'.

  MA.Touch.dump()
    MA.Touch.dump() dumps the entire register space on the FT6336 chip as a
    formatted hexdump to the serial port.


== Legacy API ==

  There was a previous version of this library, and it provided a number of
  functions that were single touch only. The older version did not have
  MA.update(). Instead it used ispressed() and getPressedPoint() functions as
  well as HotZones, which provided something that worked a little bit like
  Buttons. This older API is still supported (the MaixAmigo Factory Test sketch
  still works), but you should not use it for new programs. The ispressed()
  function specifically does not mix well with code that uses MA.update().


== Example ==

  It may sound complicated when you read it all in this document, but it's
  all made to be easy to use.

  Under File / Examples / MaixAmigo / Touch in your Arduino environment is an
  example sketch called "events_buttons_gestures_rotation" that shows both
  this library and MaixButton in action. Please have a look at it to understand
  how this all works and run the sketch to see all the events printed to the
  serial port. It shows buttons, gestures and events and should be pretty
  self-explanatory.

*/

#ifndef _MAIXTOUCH_H_
#define _MAIXTOUCH_H_

#include <MaixDisplay.h>

#include "utility/Config.h"  // Defines 'TFT', a pointer to the display
#include "utility/PointAndZone.h"

// Strangely, the value 13 leads to slightly more frequent updates than 10
// (Still not every 13 ms, often more like 15 to 20)
#define DEFAULT_INTERVAL 13

class MaixTouch {
 public:
  static MaixTouch* instance;
  MaixTouch();
  void begin();
  uint8_t ft6336(uint8_t reg);
  void ft6336(uint8_t reg, uint8_t value);
  void ft6336(uint8_t reg, uint8_t size, uint8_t* data);
  uint8_t interval(uint8_t ivl);
  uint8_t interval();
  void update();
  bool read();
  bool ispressed();
  void dump();
  Point getPressPoint();
  uint8_t points;
  bool changed, wasRead;
  Point point[2];
  uint8_t point0finger;

 protected:
  uint8_t _interval;
  uint32_t _lastRead;
};

#define NS2009_LOW_POWER_READ_X   0xC0
#define NS2009_LOW_POWER_READ_Y   0xD0
#define NS2009_LOW_POWER_READ_Z1  0xE0

class MaixTouchNS2009 {
 public:
  static MaixTouchNS2009* instance;
  MaixTouchNS2009();
  void begin();
  uint8_t ns2009(uint8_t reg);
  void ns2009(uint8_t reg, uint8_t value);
  void ns2009(uint8_t reg, uint8_t size, uint8_t* data);
  uint8_t interval(uint8_t ivl);
  uint8_t interval();
  void update();
  bool read();
  bool ispressed();
  void dump();
  Point getPressPoint();
  uint8_t points;
  bool changed, wasRead;
  Point point[2];
  uint8_t point0finger;

 protected:
  uint8_t _interval;
  uint32_t _lastRead;
};

// For compatibility with M5Core2 code
class HotZone : public Zone {
 public:
  HotZone(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_,
          void (*fun_)() = nullptr);
  void setZone(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_,
               void (*fun_)() = nullptr);
  bool inHotZone(Point& p);
  bool inHotZoneDoFun(Point& p);
  void (*fun)();
};

#define HotZone_t HotZone
#define TouchPoint Point
#define TouchPoint_t Point

#endif /* _MAIXTOUCH_H_ */
