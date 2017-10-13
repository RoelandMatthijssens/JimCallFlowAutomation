bool getJsonBody(char* responseBuffer, int bufferSize, GSMClient client){
  printFreeRam();
  bool isJson = false;
  int responseBufferIndex = 0;
  while(client.available() || client.connected()) {
    char c = client.read();
    if('{' == c){
      isJson = true;
    }
    if(isJson){
      if(bufferSize < responseBufferIndex){
        Serial.print("Command too long. Keep it under ");
        Serial.println(bufferSize);
        return false;
      }
      responseBuffer[responseBufferIndex] = c;
      responseBuffer[responseBufferIndex + 1] = NULL;
      ++responseBufferIndex;
    }
    if('}' == c){
      isJson = false;
      break;
    }
  }
  printFreeRam();
  return true;
}

