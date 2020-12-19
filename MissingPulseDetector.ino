/** /file: Missing pulse detector
 *  /Author: Forrest Lee Erickson
 *  /Date: 20201218
 * /brief: For use to detect garage safety pulses. Will set a time out greater than about two pulses. An ISR on falling edge will reset timer.
 * /details:  If the timer ever times out a flag is set to show not safe (LED goes off). If the pulses return the flag is cleared indicating safe.
 * The main loop will light an LED when safe.
 * Tested with a button press at a very slow time out and works.
 */

#include "Arduino.h"
#include "uTimerLib.h"

const char* PROGRAM_NAME = "MissingPulseDetector";         // This program
const char* PROGRAM_VERSION = "V 0.0.0";                // Starting to rename stuff and wire up interface.
const int BAUDRATE = 115200;            //Use a fast baud.

#define LED_IS_SAFE_DOOR 2  // ESP32 built in LED.
volatile bool status = 0;     // Holds LED status.

// Button with interupt service
const byte interruptPin = 25;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

/* Pulse detector for Safety signal
 *  When pulse happens reset timer to a time somewhat greater thane the pulse rate.
 *  If another pulse occourse before the timer times out it will be restarted and never time out.
 */
void IRAM_ATTR handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  TimerLib.setInterval_us(timed_function, 3000000);   // Time out in 3 seconds
  status = true;       // Got a pulse so safe.
  portEXIT_CRITICAL_ISR(&mux);
}// end IRAM_ATTR handleInterrupt

/*Missing pulse timer
 * 
 */
void timed_function() {
  status = false;       //Timed out so not safe.
  Serial.print("Safety input: ");
  if (digitalRead(interruptPin) == 1){
    Serial.println("Stuck high. Beam blocked.");  
  } else {
    Serial.println("Stuck low. Bad sensor or wire.");  
  }
}//End timed function

void setup() {
  Serial.begin(BAUDRATE);
  delay(500);
  Serial.println(PROGRAM_NAME);
  Serial.println(PROGRAM_VERSION);
  Serial.println("Caution! Amused Scientist");
  Serial.println();
  
  pinMode(interruptPin, INPUT_PULLUP);  // Button. Using an extarnal pull up instead of internal
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

	pinMode(LED_IS_SAFE_DOOR, OUTPUT);
	TimerLib.setInterval_us(timed_function, 3000000);
}

void loop() {
	digitalWrite(LED_IS_SAFE_DOOR, status);
  
}
