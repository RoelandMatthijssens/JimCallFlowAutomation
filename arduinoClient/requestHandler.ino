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

