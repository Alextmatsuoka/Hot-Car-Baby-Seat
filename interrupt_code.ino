 #include <avr/sleep.h>
 
int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter
int interruptPin = 2;
volatile int isr_flag = 0;
int isr_count = 0;
int loop_status = 1;
int timer_pin = 12;

void wakeUpNow()        // here the interrupt is handled after wakeup
{
  isr_flag=1;
}

void setup()
{
  pinMode(wakePin, INPUT);
  Serial.begin(9600);
  sleepNow();
}

void sleepNow()         // here we put the arduino to sleep
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 

    attachInterrupt(digitalPinToInterrupt(interruptPin),wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW 
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(digitalPinToInterrupt(interruptPin));      // disables interrupt 0 on pin 2 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.
}

void loop()
{
  if(isr_flag){
    isr_flag = 0;
    // Periodic Timer Code
    if(!digitalRead(timer_pin)){
      Serial.print(millis());
      Serial.print(" : Hey Listen!!!"); // classic dummy message
    }
    // Serial Interrupt Code
    while (Serial.available()) {
      int val = Serial.read();
      Serial.print(char(val));
    }
    // Re sync
    Serial.println();
    Serial.flush(); 
    while(!digitalRead(timer_pin));
    // Put back to sleep
    sleepNow();
  }
}

