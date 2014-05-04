// test of the attiny  for the sensor widget.

#include <avr/wdt.h>  // include the watchdog timer library

#define ldrPin A1    // LDR light sensor
#define irPin 1      // IR movement sensor pin  
#define timerPin A2  // Pin the timer potentiometer is connected to
#define brightnessPin A3  // Pin the timer potentiometer is connected to
#define ledPin 0     // Output for LED 


void setup(){
  wdt_enable(WDTO_8S);      // begin watchdog timer

  pinMode(ledPin, OUTPUT);  //out put for LED
  
  pinMode(irPin, INPUT);  //input sensor
  pinMode(ldrPin, INPUT);  // input for LDR
  pinMode(timerPin, INPUT);  // input for timer pot
  pinMode(brightnessPin, INPUT);  //input for brightness
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

 //lets flash the led 
 
  for(int i = 0; i <=255;i++){
    analogWrite(ledPin,i);
    delay(1);
        
    }
      
  for(int i = 255; i >=0;i--){
    analogWrite(ledPin,i);
   delay(1);
        
   }
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

}

void loop(){
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

  while(analogRead(ldrPin)<255){

  if(digitalRead(irPin)){
    
  analogWrite(ledPin,(analogRead(brightnessPin)/4));
  int timer = analogRead(timerPin);
  timer = timer * 100;
  for(int i = 0; i <=timer; i++){
      wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
      delay(1); 
  }
  }
  else {
        analogWrite(ledPin,0);
   wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting
  
  }
analogWrite(ledPin,0);
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

}
  wdt_reset();    //make sure we reset watchdog timer to prevent endless resetting

}
