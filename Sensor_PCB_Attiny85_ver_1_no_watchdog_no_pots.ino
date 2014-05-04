// test of the attiny  for the sensor widget.


#define ldrPin A1    // LDR light sensor
#define irPin 1      // IR movement sensor pin  
#define timerPin A2  // Pin the timer potentiometer is connected to
#define brightnessPin A3  // Pin the timer potentiometer is connected to
#define ledPin 0     // Output for LED 


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

    analogWrite(ledPin,255);
    delay(120000);
  
  }
  
  else {
        analogWrite(ledPin,0);
    }

}
