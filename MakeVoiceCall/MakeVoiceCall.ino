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
#include <time.h>
static const char *PIN = "1111";

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSMVoiceCall vcs;

struct call{
  const char *phone_number;
  int duration;
};

struct call calls[3] = {
  {.phone_number = "0485291098", .duration = 1 },
  {.phone_number = "0485291098", .duration = 1 },
  {.phone_number = "0485291098", .duration = 1 }
};

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Make Voice Call");

  // connection state
  // Start GSM shield
  while (gsmAccess.begin(PIN) != GSM_READY) {
    Serial.println("Not connected, Trying again in 1 second");
    delay(1000);
  }
  Serial.println("GSM initialized.");
  make_calls();
}

void make_calls(){
  struct call* ptr = calls;
  struct call* endPtr = calls + sizeof(calls)/sizeof(calls[0]);
  while ( ptr < endPtr ){
     make_call(ptr->phone_number, ptr->duration);
     ptr++;
  }
}

void make_call(const char *phonenumber, int duration_in_seconds){
  char Log[200];
  sprintf(Log, "Calling to %s for %i seconds.", phonenumber, duration_in_seconds);
  Serial.println(Log);
  if (vcs.voiceCall(phonenumber)) {
    unsigned long start_time = millis();
    Serial.println("Call Established.");
    while (Serial.read() != '\n' && (vcs.getvoiceCallStatus() == TALKING)){
      unsigned long time = millis();
      if(time - start_time >= duration_in_seconds*1000){
        break;
      }
    };
    vcs.hangCall(); //hangup
    Serial.println("Call Finished");
    Serial.println();
  }
}

void loop() {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
  }
}

