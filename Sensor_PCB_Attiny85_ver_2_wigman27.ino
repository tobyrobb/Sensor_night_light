// test of the attiny  for the sensor widget.

#include <avr/wdt.h>                   // include the watchdog timer library
#include <avr/io.h>                    //include standard AVR io library
#include <avr/interrupt.h>             //include standard AVR interrupt library

const int ldrPin = A1;                 // LDR light sensor  
const int timerPin = A2;               // Pin the timer potentiometer is connected to
const int brightnessPin = A3;          // Pin the brightness potentiometer is connected to
const int ledPin = 0;                  // Output for LED
const int irPin = 1;                   // IR movement sensor pin
const int nightTimeThreshold = 900;   // Value used within the checkNighTime function, once determined it wont need changing.
const int ledBrightnessLow = 10;       // This is the minimum PWM value that can be set with the brightness pot to stop the LED turning all the way off when the brightness is turned right down.
const int ledBrightnessHigh = 150;     // This is the maximum PWM value that can be set with the brightness pot to dull in certain installations. Maximum this value can be is 255.
const int ledOff = 0;                  // Used within the setLED function
const int ledOn = 1;                   // Used within the setLED function

//int timer = 512;                     // Don't think this is needed anymore
int offDelay = 0;                      // Initialising a variable to use to determine howlong the LED stays on for
int onTime = 0;                        // Initialising a variable to use to show the value of the on time pot.
int ledBrightness = 0;                 // Initialising a variable to set the LED brightness.

boolean nightTime = 0;                 // a boolean used to set night time or day time according to the LDR

void setup(){
  wdt_enable(WDTO_8S);                 // begin watchdog timer

  pinMode(ledPin, OUTPUT);             //out put for LED
  
  pinMode(irPin, INPUT);               //input sensor
  pinMode(ldrPin, INPUT);              // input for LDR
  pinMode(timerPin, INPUT);            // input for timer pot
  pinMode(brightnessPin, INPUT);       //input for brightness
  
  wdt_reset();                         //make sure we reset watchdog timer to prevent endless resetting
  
  //digitalWrite(irPin, HIGH);         //Not needed
  //digitalWrite(ldrPin, HIGH);        //Not needed
  //digitalWrite(timerPin, HIGH);      //Not needed
  //digitalWrite(brightnessPin, HIGH); //Not needed
  
  //lets flash the led 
  for(int i = 0; i <=255;i++){
    analogWrite(ledPin,i);
    delay(1);        
  }
  for(int i = 255; i >=0;i--){
    analogWrite(ledPin,i);
    delay(1);        
  }
   
  initTimerCounter1();                 // Initialise the timer counter 1 (see below for details)
  initInterrupt();                     // Initialise the interrupt routine (see below for details)
  
  digitalWrite(ledPin, HIGH);
  
}

void loop(){
  wdt_reset();                         // Tell the timer all is well at the western front!
  onTime = checkOnTime();              // Check the how long the LED is to stay on for
  nightTime = checkNightTime();        // Check to see if its dark
  ledBrightness = checkBrightness();   // Check to see how bright to make the LED, this will only when the LED turns on, you will have to wait untill it turns off again for the brightness adjustment to be seen.
}

/*
  This checkOnTime function simply reads the delay pot and returns a digital number between 0 and 1023.
*/
unsigned int checkOnTime(void) {
  int onTime = analogRead(timerPin);
  return onTime;
}

/*
  This checkNightTime function reads the divider value between the LDR and the POT and compares it to the threshold value 
  (which needs to be tweeked for the design then left) and either returns 1 (true) if it is or 0 (false) if it isn't. 
  This is constantly checked and updated.
*/
boolean checkNightTime (void) {
  if (analogRead(ldrPin) < nightTimeThreshold) {
    return 1; 
  } else {
    return 0;
  }
}

/* 
  The checkBrightness function serves two purposes, 1. To read the brightness pot and return a 10 bit (0-1023) digital value. The brightness of the LED is actually varied using PWM,
  the processor used can only accept and 8 bit value (0-255) for its brightness value, so we need to change the 10 bit value we have read to a proportional 8 bit value so we aren't
  wasting values from 256 - 1023 that our pot will return. There is a great function for doing this which is called map, it accepts 5 paramaeters,
  map(inputVariable, lowestInputValue, highestInputValue, lowestOutputValue, highestOutputValue). As out input variable can range from 0 to 1023 that is out second and third paramater,
  the fourth and fifth parameter is how low an high we want the output to go. Here I have used two variables that I have set at 10 for the lowest and 150 for the highest, for example if the
  input pot read 0, the lowest number this function would put out is 10, this will stop the LED turning all the way off, if the input was 1023, the maximum this would put out is 150, to stop
  the LED getting too bright. These can be any values you like, just change them at the top, its then proportional for all values in between, and input of 512 would output approximatley 55.
    
  The actual brightness of the LED will only update when the LED turns off, then when it turn on again it will be at the adjsuted brightness value.
  
*/
int checkBrightness () {
  int tenBitBrightness = analogRead(brightnessPin);
  int eightBitBrightness = map(tenBitBrightness, 0, 1023, ledBrightnessLow, ledBrightnessHigh);
  return eightBitBrightness;
}

/*
  This function turns the LED on or off, it accepts two parameters, state = 0 or 1 and brightness, must be between 0 and 255, defaults to ledBrightnessHigh.
*/
void setLED(boolean state, int brightness = ledBrightnessHigh) {
  switch (state){
    case 0: // Turn LED Off
      digitalWrite(ledPin, LOW);
    break;
    case 1: // Turn LED on and set its brightness value.
      analogWrite(ledPin, brightness);
    break;
  }
}

/*
  This is where the fun begins, there are two main 8 bit timers in the ATTiny85, Arduino uses timer 0 for its PWM function so we don't need to worry about effecting that, however delay() and mills()
  will be effected by this so should not be used. We are using this timer to turn off the LED. The function can be described below.
  The timer uses the internal system clock, in this case, it is set to 1MHz, then we can have more control as to how fast this frequency is by using a prescaler, this can divide our clock frequency by
  1 = 1,000,000 Hz
  2 = 500,000 Hz
  4 = 250,000 Hz
  8 = 125,000 Hz
  And so on for values up to
  8192 = 122 Hz
  16384 = 61 Hz
  So for a prescaler or 1 it will count to 1,000,000 in 1 second for a prescaller of 16384 it will count to 61 in 1 second.
 
  Now that we know that, there are a number of ways you can use a timer, for the project we will use the clear timer on compare (CTC) function, which counts upto an 8 bit value (0-255), then interrupts
  the processor to complete a function, resets itself and starts again. There are two values that you can set for it to count up to, Output compare register A for timer 1 (OCR1A) and output compare register B 
  for timer 1 (OCR1B). It will reset itself when it reaches the value in A.

  We can set these values by writing to internal registers (chunks of memory) in the AT Tiny, luckily the Arduino software makes it easier for us. These steps are shown below.  
 
*/
void initTimerCounter1(void)
{
    TCCR1 = 0;                                 //reset the timer register just to make sure everything is set as we want it.
    TCNT1 = 0;                                 //zero the timer out, just incase there is something random in there. This TCNT1 is the actual register that holds what number its up to.
    GTCCR = _BV(PSR1);                         //reset the prescaler
    OCR1A = 244;                               //set the compare value that we want the timer to count upto and reset.
    OCR1B = 243;                               //set the compare value that we also want something to happen.
    TIMSK = _BV(OCIE1A) | _BV(OCIE1B);         //Tell it to interrupt when the timer matches the values in A and also B to do something.
    TCCR1 = _BV(CTC1) | _BV(CS13) | _BV(CS12); //Start timer, set to CTC mode, set prescaler to 2048 at 1MHZ, this will count to 488 in one second, so it will reach the value shown in OCR1A twice in one second and then clear itself (see below for more info).   
    sei();                                     //Enable global interrupts.
}
/*
  This is the function that's called when the timer 1 reaches the OCR1A compare value, so, if we only had this compare value and didn't also include the OCR1B value this would turn the LED off twice a second.
*/
ISR(TIMER1_COMPA_vect)
{
   setLED(ledOff);                            //Turn LED off.       
}
/*
  This is the function that is called when the timer reaches the value in the OCR1B register, as shown above, this is 243, that is one tick before the value of 244 in OCR1A, that is for very good reason.
  This is what controls the time our LED is on for, what it does is pretty simple. It uses the offDelay variable which is initialised at 0, it then compares it to the number in the onTime variable, this is set directly
  by the digital 10 bit value read from the delay pot, this will range from 0 to 1023, so, if when the timer reaches 243, it enteres this function, compares the offDelay to see if it is less than the onTime, if it is,
  it enters the first if function which will it resets the timer back to 0 to start counting again and increments the offDelay variable and exits the function and continues on its merry way. Because the timer will not reach
  244 this time it won't turn off the LED just yet. The next time the timer reaches 243 it will enter this function again to test the offDelay against the onTime and continue as shown.
  
  For example, if our onTime is set to the maximum of 1023, the timer will need to count to 243, 1024 (1 more than onTime) times before it turns off the LED, so lets work out how long the is.
  1,000,000 divided by the prescaller of 2048 = 488 so it will count to 243 twice in one second so it will count to 2 in one second, to reach 1024 will take 512 seconds or approximatley 8.5 minutes.
  
*/
ISR(TIMER1_COMPB_vect)
{
  if(offDelay <= onTime) {
    TCNT1 = 0; 
    offDelay++;}
  else if(digitalRead(irPin)) {             //This else if function is really only used for really short "ontime" settings, because the actual sensor takes a little while to go low again, if the on time is not high the,
    TCNT1 = 0;                              //LED may not activate again until you have been still long enough for the motion sensor to go low. It resets the time again and also resets the off delay so it wont start counting down to turn off until you stop moving.
    offDelay = 0;
  } else {                                  //This else section will be true if the offDelay is greater than the onTime and is now time to turn off the LED, this just resets the offDelay but does NOT reset the timer and allows it to count to 244  
    offDelay = 0;                           //activating the above function to turn off the LED.
  }
}
/*
  This function initialises the interrupt function so that we can detect a change in digital pin 1 (ie when the pin goes high or l). As this pin wil only support a pin change interrupt its not smart enough to know if it is a rising edge or falling edge so we need to
  consider that in the actual function.
  This particular initialise function is called in the setup loop, it puts a 1 in the PCIE bit of the GIMSK register, this enables Pin Change Interrupts. It also puts a 1 in the PCINT1 but of the PCMSK register, this enables interrupts on digital pin 1 (PB1).
*/
void initInterrupt(void) {
  GIMSK = _BV(PCIE);
  PCMSK = _BV(PCINT1);
}

/*
This is the actual function called everytime the digital pin 1 changes, as mentioned, these types of interrupts aren't smart enough to know the difference between a rising and a falling edge we need to factor that into the function as shown below.
As there is only 1 vector function for all 6 of the PCINT pins thats why the function is PCINT0_vect NOT PCINT1_vect even though we are listening to PCINT1.
*/

ISR(PCINT0_vect) {  
  if(digitalRead(irPin) && nightTime) {    // Check to make sure it has gone high, not low.
    TCNT1 = 0;                             // Reset the timer back to 0
    offDelay = 0;                          // Reset the offDelay incase the LED is already on, this will reset the time the LED is on every time you move.
    setLED(ledOn, ledBrightness);          // Turn the LED on and set its brightness as per the checkBrightness function.
  }  
}
