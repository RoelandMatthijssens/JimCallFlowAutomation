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
#include <stdio.h>

static const char* PIN = "1111";

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSMVoiceCall vcs;
GSM_SMS sms;
GPRS gprs;
GSMClient client;

int redPin = 10;
int bluePin = 11;
int greenPin = 12;

// APN Settings

enum ActionType {
  CALL = 1,
  TEXT = 2
};

struct Action {
  ActionType type;
  const char *phone_number;
  int duration;
  int amount;
  const char *content;
};

Action actions[2] = {
  {.type = TEXT, .phone_number = "0485291098", .duration = 1, .amount = 1, .content = "TEST"},
  0
};

void setup() {
  char GPRS_APN[] = "web.be";
  char GPRS_LOGIN[] = "web";
  char GPRS_PASSWORD[] = "web";
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Initializing...");

  // Start GSM shield
  while (gsmAccess.begin(PIN) != GSM_READY) {
    Serial.println("Not connected to cell network, Trying again in 1 second");
    delay(1000);
  };
  Serial.println("GSM connected to Cell network.");
  while (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) != GPRS_READY) {
    Serial.println("Not connected to 4g network, Trying again in 1 second");
    delay(1000);
  }
  Serial.println("GSM connected to 4G network.");
  Serial.println("GSM initialized.");
  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, HIGH);
  do_actions();
  digitalWrite(bluePin, LOW);
}

void do_actions() {  
  // ?
}

void do_action(Action& action) {
  if(action.type == CALL) {
    make_call(action.phone_number, action.duration, action.amount);
  } else if (action.type == TEXT) {
    send_text(action.phone_number, action.content, action.amount);
  } else {
    Serial.println("OMG I don4\'t knoz zjat to do");
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

bool request_action(int action_index, char* action_buffer, int buffer_size){
  char server[] = "c468dc2e.ngrok.io";
  int port = 80;
  
  const int path_size = 22; // can use up to 2 digits for action_index
  char path[path_size];
  snprintf(path, path_size, "/nodes/1/action/%d", action_index);
  Serial.println(path);

  Serial.println("connecting...");
  if (!client.connect(server, port)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("connected");
  // Make a HTTP request:
  client.print("GET ");
  client.print(path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  bool is_instruction = false;
  int action_buffer_index = 0;
  while(client.available() || client.connected()) {
    char c = client.read();
    if('{' == c){
      is_instruction = true;
    }
    if(is_instruction){
      if(buffer_size < action_buffer_index){
        Serial.print("Command too long. Keep it under ");
        Serial.println(buffer_size);
        client.stop();
        return false;
      }
      action_buffer[action_buffer_index] = c;
      action_buffer[action_buffer_index + 1] = NULL;
      ++action_buffer_index;
    }
    if('}' == c){
      is_instruction = false;
      break;
    }
  }
  client.stop();
  if(!action_buffer[0]){
    Serial.println("No actions found");
    return false;
  }
  return true;
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
  int action_index = 0;
  digitalWrite(greenPin, HIGH);
  const int action_buffer_size = 200;
  char action_buffer[action_buffer_size] = "";
  if(!request_action(action_index, action_buffer, action_buffer_size)){
    Serial.println("No next action found");
    for(;;) ;
  }
  Serial.println(action_buffer);
  StaticJsonBuffer<200> json_buffer;
  JsonObject& action_json = json_buffer.parseObject(action_buffer);
  const char* phone_number = root["phone_number"];
  Serial.println(phone_number);

  //do_next_action();
  Serial.println("weee");
  for(;;) ;
}

