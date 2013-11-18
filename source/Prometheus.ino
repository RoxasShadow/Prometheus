/**
*            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
*                    Version 2, December 2004
* 
* Copyright (C) 2013 Giovanni Capuano <webmaster@giovannicapuano.net>
* 
* Everyone is permitted to copy and distribute verbatim or modified
* copies of this license document, and changing it is allowed as long
* as the name is changed.
* 
*            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
*   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
*  0. You just DO WHAT THE FUCK YOU WANT TO.
**/

#include <Wire.h>
#include <Servo.h>
#include "VCNL4000.h"

#define LIMIT     180   // servo's maximum angle degree
#define AMBIENT   600   // light sensor limit
#define ERR_TIME  10    // seconds in which auth is valid
#define SUCC_TIME 20000 // milliseconds in which auth is valid (20 seconds)

/* Password */
#define A 5
#define B 5
#define C 5
#define D 6

/* Components */
VCNL4000 chip; // proximity sensor
Servo servo;  // servo

/* Global vars */
int touches[4] = {0};
int i          = 0;
int auth       = 0;

/* PIN */
const int pinServo      = 9;
const int potentiometer = A0;
const int button        = 0;
const int grnLed        = 7;
const int redLed        = 4;
const int stsLed        = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  servo.attach(pinServo);
  servo.write(0);
  pinMode(redLed, OUTPUT);
  pinMode(grnLed, OUTPUT);
  pinMode(stsLed, OUTPUT);
  digitalWrite(stsLed, HIGH);
  chip.init();
  digitalWrite(stsLed, LOW); // if remains on, something is broken
  Serial.println("Ready.");
}

int pressingButton() {
  return digitalRead(button) == LOW;
}

int getSecondDigit(int number) {
  return number / 100 % 10;
}

void resetTouches() {
  i = 0;
  memset(touches, 0, sizeof(touches)); // reinitialize array
}

void error(int proximity) {  
  Serial.print("Proximity: ");
  Serial.println(proximity);
  resetTouches();
  
  int counter = 0;
  while(counter++ < ERR_TIME) {
    servo.write(90);
    digitalWrite(redLed, HIGH);
    delay(analogRead(potentiometer));
    digitalWrite(redLed, LOW);
    servo.write(0);
    delay(analogRead(potentiometer));
  }
}

void loop() {
  int ambient = chip.ambient();
  int proximity = chip.proximity();
  
  digitalWrite(grnLed, HIGH); // hasn't a ground, so it's inverted
  digitalWrite(redLed, HIGH);
  
  if(pressingButton()) {
    touches[i++] = proximity;
    if(i > 4)
      i = 0;
      
    Serial.println(getSecondDigit(proximity));
    
    if(getSecondDigit(touches[0]) == A && getSecondDigit(touches[1]) == B && getSecondDigit(touches[2]) == C && getSecondDigit(touches[3]) == D) {
      Serial.println("GOOOOOOOOOOOOT");
      auth = 1;
      digitalWrite(redLed, HIGH);
      digitalWrite(grnLed, LOW);
      resetTouches();
      delay(SUCC_TIME);
      digitalWrite(grnLed, HIGH);
    }
    else {
      if(touches[0] != 0 && touches[3] != 0 && (!(proximity >= 2500 && proximity <= 2590))) {
        Serial.println("ERROOOOR");
        error(proximity);
      }
    }
  }
  else {
    if(!(proximity >= 2500 && proximity <= 2590)) {
      Serial.println("GET OUTTA HERE");
      error(proximity);
    }
  }
  
  digitalWrite(stsLed, ambient < AMBIENT);
  
  delay(440);
}
