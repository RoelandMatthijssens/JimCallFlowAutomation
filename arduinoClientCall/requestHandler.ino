bool requestAction(int actionIndex, char* responseBuffer, int bufferSize, GSMClient& client){
  printFreeRam();
  const int pathSize = 22;
  char path[pathSize];
  snprintf(path, pathSize, "/nodes/%s/action/%d", nodeId, actionIndex);
  doRequest(path, GET, client);
  bool result = getJsonBody(responseBuffer, bufferSize, client);
  client.stop();
  if(!responseBuffer[0]){
    return false;
  }
  printFreeRam();
  return result;
}

bool requestNodeDetails(char* responseBuffer, int bufferSize, GSMClient& client){
  const int pathSize = 22;
  char path[pathSize];
  snprintf(path, pathSize, "/nodes/%s", nodeId);
  doRequest(path, GET, client);
  bool result = getJsonBody(responseBuffer, bufferSize, client);
  client.stop();
  if(!responseBuffer[0]){
    Serial.println("Empty node details.");
    return false;
  }
  return result;
}

void doRequest(char* path, char* method, GSMClient& client){
  printFreeRam();
  int port = 80;
  if (!client.connect(SERVER, port)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("connected.");
  Serial.print("requesting ");
  Serial.print(SERVER);
  Serial.println(path);
  // Make a HTTP request:
  client.print(method);
  client.print(" ");
  client.print(path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(SERVER);
  client.println("Connection: close");
  client.println();
  printFreeRam();
}

