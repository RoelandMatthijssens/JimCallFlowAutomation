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
#include <ArduinoJson.h>

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
const char* nodeId = "1";

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
  digitalWrite(bluePin, LOW);
}

bool request_action(int action_index, char* response_buffer, int buffer_size){
  const int path_size = 22; // can use up to 2 digits for action_index
  char path[path_size];
  snprintf(path, path_size, "/nodes/%s/action/%d", nodeId, action_index);
  Serial.println(path);
  do_request(path, "GET");
  bool result = get_json_body(response_buffer, buffer_size);
  if(!response_buffer[0]){
    Serial.println("No actions found");
    return false;
  }
  return result;
}

bool request_node_details(char* response_buffer, int buffer_size){
  const int path_size = 22;
  char path[path_size];
  snprintf(path, path_size, "/nodes/%s", nodeId); 
  do_request(path, "GET");
  bool result = get_json_body(response_buffer, buffer_size);
  if(!response_buffer[0]){
    Serial.println("Empty node details. Are you sure your node exists?");
    return false;
  }
  return result;
}

void do_request(char* path, char* method){
  char server[] = "c468dc2e.ngrok.io";
  int port = 80;
  Serial.println("connecting...");
  if (!client.connect(server, port)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("connected.");
  Serial.print("requesting ");
  Serial.print(server);
  Serial.println(path);
  // Make a HTTP request:
  client.print(method);
  client.print(" ");
  client.print(path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
}

bool get_json_body(char* response_buffer, int buffer_size){
  bool is_json = false;
  int response_buffer_index = 0;
  while(client.available() || client.connected()) {
    char c = client.read();
    if('{' == c){
      is_json = true;
    }
    if(is_json){
      if(buffer_size < response_buffer_index){
        Serial.print("Command too long. Keep it under ");
        Serial.println(buffer_size);
        client.stop();
        return false;
      }
      response_buffer[response_buffer_index] = c;
      response_buffer[response_buffer_index + 1] = NULL;
      ++response_buffer_index;
    }
    if('}' == c){
      is_json = false;
      break;
    }
  }
  client.stop();
  return true;
}

void do_action(JsonObject& action) {
  int type = action["type"];
  const char* phone_number = action["phone_number"];
  int amount = action["amount"];
  if(type == CALL) {
    make_call(phone_number, action["duration"], amount);
  } else if (type == TEXT) {
    send_text(phone_number, action["content"], amount);
  } else {
    Serial.println("OMG I don\'t know what to do");
  }
}

void make_call(const char *phonenumber, int duration_in_seconds, int amount){
  while(amount > 0){
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
    }
    amount--;
  }
}

void send_text(const char *phonenumber, const char *content, int amount){
  while(amount > 0){
    sms.beginSMS(phonenumber);
    sms.print(content);
    sms.endSMS();
    Serial.println("SMS Send");
    amount--;
  }
}

int action_index = 0;
void loop() {
  digitalWrite(greenPin, HIGH);
  const int response_buffer_size = 100;
  char response_buffer[response_buffer_size] = "";
  StaticJsonBuffer<response_buffer_size> json_buffer;
  if(!request_node_details(response_buffer, response_buffer_size)){
    Serial.println("Node not found");
    for(;;);
  }
  Serial.print("e ");Serial.println(action_index);
  JsonObject& node_details = json_buffer.parseObject(response_buffer);
  if(node_details["state"]!="STARTING"){
    Serial.println("Server says node is not ready to start yet");
    for(;;);
    
  };
  response_buffer[0] = '\0';
  if(!request_action(action_index, response_buffer, response_buffer_size)){
    Serial.println("No next action found");
    for(;;);
  }
  Serial.println(response_buffer);
  JsonObject& action_json = json_buffer.parseObject(response_buffer);
  do_action(action_json);
  
  Serial.println("Action finished, polling next action");
  ++action_index;
}

