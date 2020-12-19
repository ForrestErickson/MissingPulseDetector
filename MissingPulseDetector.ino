/** /file: Missing pulse detector
 *  /Author: Forrest Lee Erickson
 *  /Date: 20201218
 * /brief: For use to detect garage safety pulses. Will set a time out greater than about two pulses. An ISR on falling edge will reset timer.
 * /details:  If the timer evertimes out a flag is setto show not safe. If the pulses return the flag is cleared indicating safe.
 * The main loop will light an LED when safe.
 * Tested with a button press at a very slow time out and works.
 */

#include "Arduino.h"
#include "uTimerLib.h"

#define LED_IS_SAFE_DOOR 2  // ESP32 built in LED.
volatile bool status = 0;     // Holds LED status.

// Button with interupt service
const byte interruptPin = 25;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  TimerLib.setInterval_us(timed_function, 3000000);   // Time out in 3 seconds
  status = true;       // Got a pulse so safe.
  portEXIT_CRITICAL_ISR(&mux);
}// end IRAM_ATTR handleInterrupt


void timed_function() {
  status = false;       //Timed out so not safe.
//	status = !status;
}

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);  // Button. Using an extarnal pull up instead of internal
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

	pinMode(LED_IS_SAFE_DOOR, OUTPUT);
	TimerLib.setInterval_us(timed_function, 3000000);
}

void loop() {
	digitalWrite(LED_IS_SAFE_DOOR, status);
  
}
