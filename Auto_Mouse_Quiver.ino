/*
  The mouse movement is always relative. This sketch randomly generates 4 direction values,
  and uses them to set the movement of the mouse. The movement is then inverted and returned to original position.

  WARNING: When you use the Mouse.move() command, the Arduino takes over your
  mouse! Make sure you have control before you use the mouse commands.

  created 30 Sept 2020
  modified 30 Sept 2020
  by Matt Raine

  This code is modified from an example code in the public domain.
  http://www.arduino.cc/en/Tutorial/ButtonMouseControl
*/

#include "Mouse.h"
#include "usb_keyboard.h"

// Bit numbers for each led - used to make it easier later to know what you were actually testing for...
#define USB_LED_NUM_LOCK 0
#define USB_LED_CAPS_LOCK 1
#define USB_LED_SCROLL_LOCK 2
#define USB_LED_COMPOSE 3
#define USB_LED_KANA 4

int range = 3;              // output range of X or Y movement; affects movement speed
int responseDelay = 10;     // response delay of the mouse, in ms


void setup() {
  // initialize mouse control:
  Mouse.begin();
  Serial.begin(115200);
  Serial1.begin(115200);

}

unsigned long lastLoopTime=now();
void loop() {
  byte data =0;
  if(Serial.available()>0)
  {
    data = Serial.read();
    Serial1.write(data);
  }
  if(Serial1.available()>0)
  {
    data = Serial1.read();
    Serial.write(data);
  }

  if((now()-lastLoopTime)>responseDelay){
    
    // calculate the movement distance based on the button states:
    int leftState = random(-1,2);
    int rightState = random(-1,2); 
    int upState = random(-1,2);
    int downState = random(-1,2);
    int  xDistance = (leftState - rightState) * range;
    int xDist_inv = (-leftState + rightState) * range;
    int  yDistance = (upState - downState) * range;
    int yDist_inv = (-upState + downState) * range;
  
    // Only quiver when caps lock is on. 
    if (keyboard_leds & (1<<USB_LED_CAPS_LOCK)){
      // if X or Y is non-zero, move:
      if ((xDistance != 0) || (yDistance != 0)) {
        Mouse.move(xDistance, yDistance, 0);
        delay(responseDelay);
        Mouse.move(xDist_inv, yDist_inv, 0);
      }
    }
    
    //  // a delay so the mouse doesn't move too fast:
    //  delay(responseDelay);
    lastLoopTime = now();
  }
}
