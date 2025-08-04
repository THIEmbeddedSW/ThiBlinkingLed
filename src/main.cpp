/**
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

void setup()
{
    pinMode(13, OUTPUT); // initialize LED digital pin as an output
    pinMode(A0, INPUT);  // initialize pin A0 of LCD Display Shield buttons as input

    Serial.begin(115200); // we need the serial line for debugging
}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(13, HIGH);

    // wait for 1s
    delay(1000);

    // turn the LED off, if switch is not pressed (i.e. if pin is HIGH)
    if (digitalRead(A0) == HIGH) digitalWrite(13, LOW);

    Serial.println("digital input: " + String(digitalRead(A0)));

    // wait for 1s 
    delay(1000);
}