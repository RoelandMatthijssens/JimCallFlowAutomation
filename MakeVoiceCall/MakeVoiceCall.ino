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

const char* nodeId = "1";
const char* GET = "GET";
const char* POST = "POST";
static int action_index;
const int response_buffer_size = 100;
char GPRS_APN[] = "web.be";
char GPRS_LOGIN[] = "web";
char GPRS_PASSWORD[] = "web";

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
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  printFreeRam();
  Serial.println("Init");

  // Start GSM shield
  while (gsmAccess.begin(PIN) != GSM_READY) {
    Serial.println("Not connected");
    delay(1000);
  };
  Serial.println("GSM connected");
  while (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) != GPRS_READY) {
    Serial.println("4G not connected");
    delay(1000);
  }
  Serial.println("4G connected");
  printFreeRam();
}

bool request_action(int action_index, char* response_buffer, int buffer_size){
  printFreeRam();
  const int path_size = 22;
  char path[path_size];
  snprintf(path, path_size, "/nodes/%s/action/%d", nodeId, action_index);
  Serial.println(path);
  do_request(path, GET);
  bool result = get_json_body(response_buffer, buffer_size);
  client.stop();
  if(!response_buffer[0]){
    return false;
  }
  printFreeRam();
  return result;
}

bool request_node_details(char* response_buffer, int buffer_size){
  const int path_size = 22;
  char path[path_size];
  snprintf(path, path_size, "/nodes/%s", nodeId); 
  do_request(path, GET);
  bool result = get_json_body(response_buffer, buffer_size);
  client.stop();
  if(!response_buffer[0]){
    Serial.println("Empty node details.");
    return false;
  }
  return result;
}

void do_request(char* path, char* method){
  printFreeRam();
  char server[] = "c468dc2e.ngrok.io";
  int port = 80;
  Serial.print("connecting: ");
  Serial.print(server);
  Serial.print(":");
  Serial.println(port);
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
  printFreeRam();
}

bool get_json_body(char* response_buffer, int buffer_size){
  printFreeRam();
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
  printFreeRam();
  return true;
}

void do_actions(){
  printFreeRam();
  char response_buffer[response_buffer_size] = "";
  int action_index = 0;
  while(request_action(action_index, response_buffer, response_buffer_size)){
    StaticJsonBuffer<response_buffer_size> json_buffer;
    Serial.println(response_buffer);
    JsonObject& action_json = json_buffer.parseObject(response_buffer);
    do_action(action_json);
    ++action_index;
    response_buffer[0] = '\0';
  }
  Serial.println("No next action found");
  printFreeRam();
}

void do_action(JsonObject& action) {
  printFreeRam();
  int type = action["type"];
  const char* phone_number = action["phone_number"];
  int amount = action["amount"];
  Serial.println(type);
  if(type == CALL) {
    make_call(phone_number, action["duration"], amount);
  } else if (type == TEXT) {
    send_text(phone_number, action["content"], amount);
  } else {
    Serial.print("Wrong action type: ");
    Serial.println(type);
  }
  printFreeRam();
}

void make_call(const char *phonenumber, int duration_in_seconds, int amount){
  printFreeRam();
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
      vcs.hangCall();
      Serial.println("Call Finished");
    }
    amount--;
  }
  printFreeRam();
}

void send_text(const char *phonenumber, const char *content, int amount){
  printFreeRam();
  while(amount > 0){
    sms.beginSMS(phonenumber);
    sms.print(content);
    sms.endSMS();
    Serial.println("SMS Send");
    amount--;
  }
  printFreeRam();
}

void set_finished(){
  printFreeRam();
  const int path_size = 25;
  char path[path_size];
  snprintf(path, path_size, "/nodes/%s/finish", nodeId);
  Serial.println(path);
  do_request(path, POST);
  client.stop();
  printFreeRam();
}

bool ready_to_start(){
  printFreeRam();
  char* response_buffer = new char[response_buffer_size];
  StaticJsonBuffer<response_buffer_size> json_buffer;
  if(!request_node_details(response_buffer, response_buffer_size)){
    Serial.println("Node not found");
    delete [] response_buffer;
    return false;
  }
  JsonObject& node_details = json_buffer.parseObject(response_buffer);
  if(node_details["state"]!="STARTING"){
    Serial.println("Node waiting");
    delete [] response_buffer;
    return false;
  };
  printFreeRam();
  delete [] response_buffer;
  return true;
}

int printFreeRam (){
  extern int __heap_start, *__brkval;
  int v;
  Serial.println((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
}

void loop() {
  printFreeRam();
  if(ready_to_start()){
    do_actions();
    set_finished();
  }
  delay(5000);
  printFreeRam();
}

