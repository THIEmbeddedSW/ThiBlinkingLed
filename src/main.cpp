/**
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystalAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>


// define the required LCD objects
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
CharacterDisplayRenderer renderer(new LiquidCrystalAdapter(&lcd, 16, 2), 16, 2);
LcdMenu menu(renderer);

// data for debouncing
volatile u16 lastValidState = LOW;
volatile unsigned long lastInterrupt = 0;
const unsigned long debounceTime = 20;

// counters and bools for interrupt event handling
volatile bool tick = false;
u16 system_counter = 0;
volatile bool button_clicked = false;
u8 button_counter = 0;

// PCINT1_vect: interrupt vector for PCINT[14:8]
ISR (PCINT1_vect)
{
    if((millis() - lastInterrupt) > debounceTime) // debounce time elapsed
    {           
        if( (digitalRead(A0) == LOW) && (lastValidState == HIGH) ) //we come from HIGH
        {
            lastValidState = LOW;
            button_clicked = true;
        }
        else
        {
            lastValidState = HIGH;
        }
        lastInterrupt = millis();
    }
}

// timer1 compare interrupt service routine
ISR(TIMER1_COMPA_vect)
{
    tick = true;
}


void setup()
{
    pinMode(13, OUTPUT); // initialize LED digital pin as an output
	pinMode(A0, INPUT);  // initialize A0 pin, which is connected to the buttons of Display Keypad, as input
    
    // configure pin-change interrupt
    PCICR = (1<<PCIE1);   // enable PCINT[14:8] interrupts
    PCMSK1 = (1<<PCINT8); // A0 = PCINT8
    EICRA = (1<<ISC11);   // falling egde only

	// configure timer1 interrupt to 10ms
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = 625;              // compare match register to trigger every 10ms
	TCCR1B |= (1 << WGM12);   // CTC mode
	TCCR1B |= (1 << CS12) | (0 << CS11) | (0 >> CS10); //Prescale auf 256 -> frequency 16MHz/256 = 62500 Hz
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

    Serial.begin(115200); // we need the serial line for debugging; set the baudrate as parameter

    renderer.begin();
    lcd.clear();

    // Some hello text
    lcd.setCursor(0,0);
	lcd.print("THI Experiment");
}

void loop()
{
    if (tick) // 10ms timer interrupt occured
    {
        system_counter++;

        // check for clicked button
        if (button_clicked)
        {
            digitalWrite(13, HIGH); // switch LED on
            lcd.setCursor(0,1);
	        lcd.print("On ");
            lastValidState = LOW;
            button_counter++;
            Serial.println(String(button_counter) + " Interrupts on pin A0!");

            button_clicked = false;
        }

        if (system_counter % 100 == 0) // 1s elapsed (100x10ms)
        {
            // switch LED, put info on LCD
            if (digitalRead(13) == LOW) // LED is off
            {
                digitalWrite(13, HIGH);
                lcd.setCursor(0,1);
                lcd.print("On ");
            } 
            else // LED is on
            {
                if (digitalRead(A0) == HIGH) // if button is not pressed
                {
                    digitalWrite(13, LOW);
                    lcd.setCursor(0,1);
                    lcd.print("Off");
                }
            }
            Serial.println("digital input: " + String(digitalRead(A0)));

            system_counter = 0;  // reset
        }
        tick = false; //reset for next tick.
    }
}