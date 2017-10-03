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

#define MOBILE_VIKINGS "0485291098"
#define ORANGE "0485291098"
#define JIM "0485291098"
#define TEXT "TEXT"
#define CALL "CALL"

static const char *PIN = "1111";

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSMVoiceCall vcs;
GSM_SMS sms;

struct action{
  const char *type;
  const char *phone_number;
  int duration;
  int amount;
  const char *content;
};

struct action actions[2] = {
  {.type = CALL, .phone_number = MOBILE_VIKINGS, .duration = 5, .amount = 1, .content = ""},
  {.type = TEXT , .phone_number = MOBILE_VIKINGS, .duration = 0, .amount = 1, .content = "TEST SMS"}
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
  do_actions();
}

void do_actions(){
  struct action* ptr = actions;
  struct action* endPtr = actions + sizeof(actions)/sizeof(actions[0]);
  while ( ptr < endPtr ){
     do_action(ptr);
     ptr++;
  }
}

void do_action(action* action){
  Serial.println(action->type);
  if(action->type == CALL){
    make_call(action->phone_number, action->duration, action->amount);
  } else if (action->type == TEXT){
    send_text(action->phone_number, action->content, action->amount);
  } else {
    ;
  }
}

void make_call(const char *phonenumber, int duration_in_seconds, int amount){
  while(amount > 0){
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
    amount--;
  }
}

void send_text(const char *phonenumber, const char *content, int amount){
  char Log[200];
  sprintf(Log, "Sending \"%s\" to %s.", content, phonenumber);
  Serial.println(Log);
  sms.beginSMS(phonenumber);
  sms.print(content);
  sms.endSMS();
  Serial.println("SMS Send");
}

void loop() {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
  }
}

