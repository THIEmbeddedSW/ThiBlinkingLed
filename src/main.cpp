/**
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

void setup()
{
    pinMode(13, OUTPUT); // initialize LED digital pin as an output
}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(13, HIGH);

    // wait for 1s
    delay(1000);

    // turn the LED off
    digitalWrite(13, LOW);

    // wait for 1s 
    delay(1000);
}