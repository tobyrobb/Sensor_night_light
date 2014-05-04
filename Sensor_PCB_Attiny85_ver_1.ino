// test of the attiny  for the sensor widget.

#define mosfetPin 0
#define sensorPin 1
#define ldrPin A1
#define timerPin A2
#define brightnessPin A3



void setup(){
  
  pinMode(mosfetPin, OUTPUT);  //out put for LED
  
  pinMode(sensorPin, INPUT);  //input sensor
  pinMode(ldrPin, INPUT);  // input for LDR
  pinMode(timerPin, INPUT);  // input for timer pot
  pinMode(brightnessPin, INPUT);  //input for brightness

  //lets flash the led 
  
 
  
  for(int i = 0; i <=255;i++){
    analogWrite(mosfetPin,i);
    delay(1);
        
    }
      
  for(int i = 255; i >=0;i--){
    analogWrite(mosfetPin,i);
   delay(1);
        
   }
 

}

void loop(){
  while(analogRead(ldrPin)<255){

  if(digitalRead(0)){
    
  analogWrite(mosfetPin,(analogRead(brightnessPin)/4));
  }
  else {analogWrite(mosfetPin,0);
    
}
    int timer = analogRead(timerPin);
    delay(timer*100);

}
analogWrite(mosfetPin,0);
}

