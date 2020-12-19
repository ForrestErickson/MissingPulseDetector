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
	#define LED_BUILTIN 13
#endif

volatile bool status = 0;

void timed_function() {
	status = !status;
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	TimerLib.setInterval_us(timed_function, 1000000);
}

void loop() {
	digitalWrite(LED_BUILTIN, status);
}
