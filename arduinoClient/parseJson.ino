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

