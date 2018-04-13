// ------------
// MailNotification
// ------------

/*-------------

fillout
This assumes a switch is being used that completes the circuit when the mailbox is open, so a normally open switch. If you need to adapt to a Normally closed, switch the "<" for ">" in the if (analogRead(A7) < 2600) statement below. 

-------------*/
#include "cellular_hal.h"
STARTUP(cellular_credentials_set("fp.com.attz", "", "", NULL));



int led2 = D7;
// This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

bool notified = false;
int count = 0;
void setup() {
  pinMode(led2, OUTPUT);
  pinMode(A7, INPUT);

}


void loop() {
  // To blink the LED, first we'll turn it on...
  digitalWrite(led2, HIGH);

  // We'll leave it on...
  // This checks to see if we have sent an open notification  
  if (notified == false)
  {
      Particle.publish("MailStatus","opened",60,PRIVATE);
      notified = true;
      
  }
  //wait 30 sec for the mailman to shut the door
  delay(30000);
  
  //if the door is left open too long, send a notification and shut down
  count = count +1;
  if (count > 10)
  {
    Particle.publish("MailStatus","Left Open",60,PRIVATE);
    delay(500);
    System.sleep(SLEEP_MODE_SOFTPOWEROFF);
    
  }
  // Then we'll turn the led off...
  digitalWrite(led2, LOW);
  
  //if the door is closed, publish that it is closed, and shutdown.
  if (analogRead(A7) < 2600)
  {
    Particle.publish("MailStatus","closed",60,PRIVATE);
    FuelGauge fuel;
    float stateOfCharge = fuel.getSoC();
    Particle.publish("Battery",String(stateOfCharge) ,60,PRIVATE);
    delay(500);
    System.sleep(SLEEP_MODE_SOFTPOWEROFF);
  };
  // And repeat!
}

