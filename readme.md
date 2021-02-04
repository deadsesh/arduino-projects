# ASR Projects
This is a set of projects from a university course called Applications of Robotic Systems (ASR).
This README is created just for the sake of it :D

## Topic I
User interaction.
Exercises are written inside namespaces to understand the structure easier and not having to build three different circuits.
### Exercise 1
ON/OFF button for a green LED:
```c++
namespace first_exercise {
    const int led = 11;         //LED pin
    const int button = 10;      //button pin
    bool state = false;         //LED state flag

    void switch_led() {
        ...
    }
}
```
Contains declarations for the LED and the button with the switch function.
### Exercise 2
ON/OFF commands sent from terminal to USART.
Uses wrappers around Serial I/O for calling stdio.h functions.
```bash
'<led on>' # turn the LED on
'<led off>' # turn the LED off
'<quit>' # stop executing the function
```
### Exercise 3
Printing characters from the keypad to the LCD.
Like the previous exercise, uses stdio.h wrappers around LCD printing and keypad reading functions.
Also uses [LiquidCrystal.h](https://www.arduino.cc/en/Reference/LiquidCrystal) and [Keypad.h](https://www.arduino.cc/reference/en/libraries/keypad/).
Bug: keypad characters are written with auxilliary random symbols to the LCD. Using <i>Serial.print()</i> works just fine.

## Topic II
To be continued..
