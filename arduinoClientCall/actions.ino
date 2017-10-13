enum ActionType {
  CALL = 1,
  TEXT = 2
};

struct Action {
  ActionType type;
  const char *phoneNumber;
  int duration;
  int amount;
  const char *content;
};

void doActions(GSMClient& client){
  printFreeRam();
  char responseBuffer[responseBufferSize] = "";
  int actionIndex = 0;
  while(requestAction(actionIndex, responseBuffer, responseBufferSize, client)){
    StaticJsonBuffer<responseBufferSize> jsonBuffer;
    Serial.println(responseBuffer);
    JsonObject& actionJson = jsonBuffer.parseObject(responseBuffer);
    doAction(actionJson);
    ++actionIndex;
    responseBuffer[0] = '\0';
  }
  Serial.println("No next action found");
  printFreeRam();
}

void doAction(JsonObject& action) {
  printFreeRam();
  int type = action["type"];
  const char* phoneNumber = action["phoneNumber"];
  int amount = action["amount"];
  if(type == CALL) {
    makeCall(phoneNumber, action["duration"], amount);
  } else if (type == TEXT) {
    sendText(phoneNumber, action["content"], amount);
  } else {
    Serial.print("Wrong action type: ");
    Serial.println(type);
  }
  printFreeRam();
}

void sendText(const char *phonenumber, const char *content, int amount){
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

void makeCall(const char *phonenumber, int durationInSeconds, int amount){
  printFreeRam();
  while(amount > 0){
      if (voiceCallService.voiceCall(phonenumber)) {
          unsigned long startTime = millis();
          Serial.println("Call Established.");
          while (Serial.read() != '\n' && (voiceCallService.getvoiceCallStatus() == TALKING)){
              unsigned long time = millis();
              if(time - startTime >= durationInSeconds*1000){
                  break;
              }
          };
          voiceCallService.hangCall();
          Serial.println("Call Finished");
      }
      amount--;
  }
  printFreeRam();
}

void setFinished(GSMClient& client){
  printFreeRam();
  const int pathSize = 25;
  char path[pathSize];
  snprintf(path, pathSize, "/nodes/%s/finish", nodeId);
  doRequest(path, POST, client);
  client.stop();
  printFreeRam();
}

bool readyToStart(GSMClient& client){
  printFreeRam();
  char* responseBuffer = new char[responseBufferSize];
  StaticJsonBuffer<responseBufferSize> jsonBuffer;
  if(!requestNodeDetails(responseBuffer, responseBufferSize, client)){
  }
  JsonObject& nodeDetails = jsonBuffer.parseObject(responseBuffer);
  if(nodeDetails["state"]!="STARTING"){
    Serial.println("Node waiting");
    delete [] responseBuffer;
    return false;
  };
  printFreeRam();
  delete [] responseBuffer;
  return true;
}
