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
