/*
 Make Voice Call

 This sketch, for the Arduino GSM shield, puts a voice call to
 a remote phone number that you enter through the serial monitor.
 To make it work, open the serial monitor, and when you see the
 READY message, type a phone number. Make sure the serial monitor
 is set to send a just newline when you press return.
*/

// libraries
#include <GSM.h>

// PIN Number
static const char *PIN = "1111";
static const char *PHONE_NUMBER = "0485291098";

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSMVoiceCall vcs;

void setup() {

  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Make Voice Call");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected) {
    if (gsmAccess.begin(PIN) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized.");
  Serial.println("Enter phone number to call.");

  call_number(PHONE_NUMBER);
}

void call_number(const char *phonenumber){
  Serial.print("Calling to : ");
  Serial.println(phonenumber);
  if (vcs.voiceCall(phonenumber)) {
    Serial.println("Call Established. Enter line to end");
    while (Serial.read() != '\n' && (vcs.getvoiceCallStatus() == TALKING));
    vcs.hangCall(); //hangup
    Serial.println("Call Finished");
  }
}

void loop() {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
  }
}

