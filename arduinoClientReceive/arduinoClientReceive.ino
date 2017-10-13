/*
 Receive Voice Call

 This sketch, for the Arduino GSM shield, receives voice calls,
 displays the calling number, waits a few seconds then hangs up.

 Circuit:
 * GSM shield
 * Voice circuit. Refer to to the GSM shield getting started guide
   at http://www.arduino.cc/en/Guide/ArduinoGSMShield#toc11
 * SIM card that can accept voice calls

 With no voice circuit the call will connect, but will not send or receive sound

 created Mar 2012
 by Javier Zorzano

 This example is in the public domain.

 http://www.arduino.cc/en/Tutorial/GSMExamplesReceiveVoiceCall

 */

// Include the GSM library
#include <GSM.h>

// PIN Number
static const char* PIN = "1111";

int redPin = 10;
int bluePin = 11;
int greenPin = 12;

// initialize the library instance
GSM gsmAccess;
GSMVoiceCall voiceCallService;

// Array to hold the number for the incoming call

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Start GSM shield
  while (gsmAccess.begin(PIN) != GSM_READY) {
    Serial.println("Not connected, Trying again in 1 second");
    delay(1000);
  }
  Serial.println("GSM initialized.");
  
  // This makes sure the modem correctly reports incoming events
  voiceCallService.hangCall();
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  Serial.println("Waiting for a call");
}

void loop() {
  char incomingTelephoneNr[20];
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  // Check the status of the voice call
  switch (voiceCallService.getvoiceCallStatus()) {
    case IDLE_CALL: // Nothing is happening
      digitalWrite(greenPin, HIGH);
      break;
    case RECEIVINGCALL: // Yes! Someone is calling us
      digitalWrite(bluePin, HIGH);
      // Retrieve the calling number
      voiceCallService.retrieveCallingNumber(incomingTelephoneNr, 20);
      Serial.print("Receiving call from: ");
      Serial.println(incomingTelephoneNr);
      // Answer the call, establish the call
      voiceCallService.answerCall();
      break;
    case TALKING:
      digitalWrite(bluePin, HIGH);
      break;
  }
  delay(100);
}


