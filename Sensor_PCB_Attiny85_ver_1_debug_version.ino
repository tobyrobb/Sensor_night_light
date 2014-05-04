


/*
Day night IR sensor shield 5/04/13 Trobb
 
 Please enjoy my dodgy code!
 
 I am not a programmer by any means!
 
 Feel free to improve on it!
 
 Check out http://www.tobyrobb.com 
 
 Written with Arduino IDE V.1.0.1
 
 $VER1.0b
 
 Version 1.0b for the Attiny 85 mini 3 LED Sensor Widget Board

 DEBUG VERSION!!!! 
 
this version does a double flash when in the check LDR routine.



TO DO


  */



#include <avr/wdt.h>  // include the watchdog timer library

#define ldrPin A1    // LDR light sensor
#define irPin 1      // IR movement sensor pin  
#define timerPin A2  // Pin the timer potentiometer is connected to
#define brightnessPin A3  // Pin the timer potentiometer is connected to

#define led 0     // Output for LED 

#define timerGain 1  // the amount of timer delay gain to add to the timerPot. 10 is a normal value

boolean running = false;  // BOOLEAN value to decide wether we are dark and running
boolean movement = false;  // BOOLEAN value to decide wether we are moving

int DUSK = 250;    // Value to trigger the low light condition
int DAWN = 270;    // Value to trigger the bright light condition

int brightnessPot;  // a variable to hold the brightness pot value
int brightness;    // A variable to hold the brightness value itself for the LED 

float timerValue;   // A variable to hold the value of the delay on time
float timerDelay;    // This is the exact value of the timer delay

long previousMillis = 0;        // will store last time we checked the LDR
long interval = 3000;           // interval at which to check the LDR (milliseconds)

void setup() {
  wdt_enable(WDTO_8S);      // begin watchdog timer
 
// initialize the IO.

  pinMode(led, OUTPUT);     
  pinMode(irPin, INPUT);
  pinMode(ldrPin, INPUT);     
  pinMode(timerPin, INPUT);     
  pinMode(brightnessPin, INPUT);     
  digitalWrite(led, LOW);   // start with LED off
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

 //lets flash the led 
 
  for(int i = 0; i <=255;i++){
    analogWrite(led,i);
    delay(1);
        
    }
      
  for(int i = 255; i >=0;i--){
    analogWrite(led,i);
   delay(1);
        
   }
   

// Setup is complete!!

  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
}

void loop() {

  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting


// Now check for darkness or daylight and movement

   checkLdr();  // This will set the RUNNING flag if its now dark
   
   checkIr();
   
   
}



//  The function below is used to check the LDR at set intervals

void checkLdr(){
 
  // check to see if it's time to read the LDR;
  
  
  //DEBUG flash led twice to show we are in this routine
  
  
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    delay(1000);
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    digitalWrite(led, HIGH);       // turn led off 
    delay(10);
    digitalWrite(led, LOW);       // turn led off 
    delay(10);
    digitalWrite(led, HIGH);       // turn led off 
    delay(10);
    digitalWrite(led, LOW);       // turn led off 
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    delay(1000);
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
      
      
  unsigned long currentMillis = millis();    // difference between the current time and last time 

  if(currentMillis - previousMillis > interval) {  // if THIS time is an INTERVAL longer than LAST time
     previousMillis = currentMillis;         // save the last time you checked the LDR 

 // go ahead and check the LDR  
 
  if(analogRead(ldrPin)>=DAWN){
     running = false;      // set the RUNNING flag to OFF
     }
  if(analogRead(ldrPin)<=DUSK){
     running = true;       // set the RUNNING flag to ON
     }
}      // end of if statement

  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

}

void checkIr(){
 
  
  //DEBUG flash led twice to show we are in this routine
  
  
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    delay(1000);
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    digitalWrite(led, HIGH);       // turn led off 
    delay(500);
    digitalWrite(led, LOW);       // turn led off 
    delay(500);
    digitalWrite(led, HIGH);       // turn led off 
    delay(500);
    digitalWrite(led, LOW);       // turn led off 
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
    delay(1000);
    wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
      
  if(digitalRead(irPin)){
     movement = true;      // set the RUNNING flag to ON 
     }
  else{
     movement=false;      // set the RUNNING flag to OFF
        
     }
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

}
