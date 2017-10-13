bool requestAction(int actionIndex, char* responseBuffer, int bufferSize, GSMClient client){
  printFreeRam();
  const int pathSize = 22;
  char path[pathSize];
  snprintf(path, pathSize, "/nodes/%s/action/%d", nodeId, actionIndex);
  Serial.println(path);
  doRequest(path, GET, client);
  bool result = getJsonBody(responseBuffer, bufferSize, client);
  client.stop();
  if(!responseBuffer[0]){
    return false;
  }
  printFreeRam();
  return result;
}

bool requestNodeDetails(char* responseBuffer, int bufferSize, GSMClient client){
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

void doRequest(char* path, char* method, GSMClient client){
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

