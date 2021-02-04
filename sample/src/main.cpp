#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "LiquidCrystal.h"
#include "Keypad.h"

#define SOP '<' 
#define EOP '>'
#define INTERRUPT_BUTTON 13

namespace first_exercise {
    /* 
    * sa se proiecteze o apliatie in baza de MCU 
    * care ar schimba starea unui LED la detectarea apasarii unui buton
    */
    const int led = 11;
    const int button = 10;
    bool state = false;

    void switch_led() {
        if(digitalRead(button) == LOW) {
            state = !state;
        }
        if(state) {
            digitalWrite(led, HIGH);
        } else {
            digitalWrite(led, LOW);
        }
        delay(1000);
    }
}

namespace second_exercise {
    /* 
    * sa se proiecteze o aplicate in baza de MCU
    * care ar receptiona comenzi de la terminal prin interfata seriala pentru a seta starea unui LED.
    * - led on pentru aprindere si led off pentru stingere., sistemul trebuie sa raspunda cu mesaje text despre confirmarea comenzii
    * - pentru schimbul de text prin terminal a se utiliza libraia STDIO
    */
    const int led = 12;
    bool run = true;

    bool switch_led() {
        char * command = new char[7], tmp;
        scanf("%c", &tmp);
        printf(&tmp);
        if(tmp == SOP) {
            int i = 0;
            scanf("%c", &tmp);
            printf(&tmp);
            while(tmp != EOP) {
                command[i++] = tmp;
                scanf("%c", &tmp);
                printf(&tmp);
            }
        }
        if(strcmp(command, "led on") == 0) {
            printf("\rTurning led on..\r");
            digitalWrite(led, HIGH);
        } else if(strcmp(command, "led off") == 0) {
            printf("\rTurning led off..\r");
            digitalWrite(led, LOW);
        } else if(strcmp(command, "quit") == 0) {
            printf("\rStopping function..\r");
            return false;
        } else if(strcmp(command, "\r") != 0) {
            printf("\r:)\r");
        } else {
            printf("\rInvalid command\r");
        }
        return true;    
    }
}

namespace third_exercise {
    /*
    * sa se proiecteze o aplicatie in baza de MCU
    * pentru detectarea unui cod de la o tastarura 4x4, sa verifice codul si sa afisese mesaj la un LCD. 
    * - pentru cod valid sa se aprinda un led de culoare verde, pentru cod invalid, un led de culoare rosie.
    * - a se utiliza STDIO pentru scanarea tastaturii si afisare la LCD.
    */
    LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
    const byte rows = 4;
    const byte cols = 4;
    const char keys[rows][cols] = {
        {'7', '8', '9', '/'},
        {'4', '5', '6', '*'},
        {'1', '2', '3', '-'},
        {'C', '0', '=', '+'}
    };
    const char invalidKeys[] = {
        keys[0][3], keys[1][3], keys[2][3], 
        keys[3][3], keys[3][2], keys[3][0]
    };
    byte rowPins[rows] = {9, 8, 7, 6};
    byte colPins[cols] = {5, 4, 3, 2};

    Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
    char key = 0;

    void validate_input() {
        scanf("%c", &key);
        if(key != 0) {
            lcd.clear();
            for(unsigned int i = 0; i < sizeof(invalidKeys); i++) {
                if(key == invalidKeys[i]) {
                    digitalWrite(second_exercise::led, LOW);
                    delay(100);
                    digitalWrite(second_exercise::led, HIGH);
                    delay(100);
                    digitalWrite(second_exercise::led, LOW);
                    printf("WRONG CHAR");
                    break;
                } else if(i == sizeof(invalidKeys) - 1) {
                    digitalWrite(first_exercise::led, LOW);
                    delay(100);
                    digitalWrite(first_exercise::led, HIGH);
                    delay(100);
                    digitalWrite(first_exercise::led, LOW);
                    printf(&key);
                }
            }
            key = 0;
        }
    }
}

namespace serial {
    int my_putChar(char c, FILE * f) {
        return Serial.write(c);
    }

    int my_getChar(FILE * f) {
        while (!Serial.available());
        return Serial.read();
    }

    FILE * stream = fdevopen(my_putChar, my_getChar);
}

namespace liquid_crystal {
    int my_putChar(char c, FILE * f) {
        return third_exercise::lcd.write(c);
    }

    int my_getChar(FILE * f) {
        return third_exercise::keypad.getKey();
    }

    FILE * stream = fdevopen(my_putChar, my_getChar);
}


void setup() {
    Serial.begin(9600);
    stdin = stdout = serial::stream;

    pinMode(INTERRUPT_BUTTON, INPUT);
    pinMode(first_exercise::button, INPUT);
    pinMode(first_exercise::led, OUTPUT); 
    pinMode(second_exercise::led, OUTPUT); 
    
    digitalWrite(first_exercise::led, LOW);
    third_exercise::lcd.begin(16, 2);

    printf("\r!!! PRESS THE INTERRUPT BUTTON TO START !!! \r\r");
    while(true) {
        if(digitalRead(INTERRUPT_BUTTON) == HIGH) break;
    }
}

void loop() {
    stdin = stdout = serial::stream;
    int option;
    printf("\rMENU\r");
    printf("1. Button LED switching;\r");
    printf("2. Terminal LED switching;\r");
    printf("3. Keyboard input validation;\r");
    printf("IMPORTANT: The interrupt button should be used to quit 1 & 3, use <quit> to stop 2.\r");

    scanf("%d", &option);

    if(option == 1) {
        printf("Option 1 chosen\r");
        while(digitalRead(INTERRUPT_BUTTON) != LOW) {
            first_exercise::switch_led();
        }
        printf("Interrupt button pressed!\r");
    } else if(option == 2) {
        printf("Option 2 chosen\r");
        while(true) {
            bool running = second_exercise::switch_led();
            if(!running) break;
        }
        printf("Function stopped!\r");
    } else if(option == 3) {
        printf("Option 3 chosen\rBug: lcd prints random symbols\r");
        stdin = stdout = liquid_crystal::stream;
        printf("HELLO");
        while(digitalRead(INTERRUPT_BUTTON) != LOW) {
            third_exercise::validate_input();
        }
        stdin = stdout = serial::stream;
        printf("Interrupt button pressed!\r");
    }
}