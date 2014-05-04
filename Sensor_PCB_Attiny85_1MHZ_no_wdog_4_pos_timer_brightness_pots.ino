// test of the attiny  for the sensor widget.


#define ldrPin A1    // LDR light sensor
#define irPin 1      // IR movement sensor pin  
#define timerPin A2  // Pin the timer potentiometer is connected to
#define brightnessPin A3  // Pin the brightness potentiometer is connected to
#define ledPin 0     // Output for LED 
#define k 58 // Set this yourself konstant for pot

int val = 1;

void setup(){

  pinMode(ledPin, OUTPUT);  //out put for LED
  
  pinMode(irPin, INPUT);  //input sensor
  pinMode(ldrPin, INPUT);  // input for LDR
  pinMode(timerPin, INPUT);  // input for timer pot
  pinMode(brightnessPin, INPUT);  //input for brightness

 //lets flash the led 
 
  for(int i = 0; i <=255;i++){
    analogWrite(ledPin,i);
    delay(1);
        
    }
      
  for(int i = 255; i >=0;i--){
    analogWrite(ledPin,i);
   delay(1);
        
   }

}

void loop(){

  if((analogRead(ldrPin)<300) && digitalRead(irPin)){
 
    analogWrite(ledPin,(analogRead(brightnessPin)/4));
    val=analogRead(timerPin);
  if(val>=1 && val<=250){
    delay(30000); //30 seconds
  }
  if(val>=251 && val<=700){
    delay(180000); // About 3 minutes
  }
  if(val >=701 && val<=950){
    delay(480000); // About 8 minutes
  }
  if(val>=951 && val <=1023){
    delay(900000); //About 15 minutes
  }
  }
  else {
        analogWrite(ledPin,0);
    }
  }

