/** /file Missing pulse detector
 *  /Author Forrest Lee Erickson
 *  /Date: 20201218
 * /brief For use to detect garage safety pulses. Will set a time out greater than about two pulses. An ISR on falling edge will reset timer.
 * /details.  If the timer evertimes out a flag is setto show not safe. If the pulses return the flag is cleared indicating safe.
 * The main loop will light an LED when safe.
 */


/**
 * uTimerLib example
 *
 * @author Naguissa
 * @url https://www.github.com/Naguissa/uTimerLib
 * @url https://www.foroelectro.net
 */

#include "Arduino.h"
#include "uTimerLib.h"

#ifndef LED_BUILTIN
	// change to fit your needs
	// Use 0 or 1 to use DigiSpark AVR internal LED (depending revision, mine is 1)
	//#define LED_BUILTIN 13
  #define LED_BUILTIN 2  // ESP32 built in LED.
#endif

volatile bool status = 0;     // Holds LED status.

// Button with interupt service
const byte interruptPin = 25;
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  
//  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
//    interruptCounter++;
//  }
//  last_micros = micros();

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

	pinMode(LED_BUILTIN, OUTPUT);
	TimerLib.setInterval_us(timed_function, 3000000);
}

void loop() {
	digitalWrite(LED_BUILTIN, status);
  
}
