/*
  Web client

 This sketch connects to a website through a GSM shield. Specifically,
 this example downloads the URL "http://www.arduino.cc/asciilogo.txt" and
 prints it to the Serial monitor.

 Circuit:
 * GSM shield attached to an Arduino
 * SIM card with a data plan

 created 8 Mar 2012
 by Tom Igoe

 http://www.arduino.cc/en/Tutorial/GSMExamplesWebClient

 */

// libraries
#include <GSM.h>
#include <ArduinoJson.h>

// APN data
char GPRS_APN[] = "web.be";
char GPRS_LOGIN[] = "web";
char GPRS_PASSWORD[] = "web";

char PINNUMBER[] = "1111";

// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// URL, path & port (for example: arduino.cc)
char server[] = "c468dc2e.ngrok.io";
char path[] = "/nodes/1/actions";
int port = 80;

char input[200] = "";
int input_index = 0;

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Starting Arduino web client.");
  // connection state
  boolean notConnected = true;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (notConnected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void clear_input(){
  input[0] = '\0';
  input_index = 0;
}

void input_append(char c){
  input[input_index] = c;
  input[input_index+1] = '\0';
  input_index++;
}

void do_instruction(){
  StaticJsonBuffer<200> jsonBuffer;
  Serial.println("aaaaaaaaaaaaaaaaaaa");
  JsonObject& root = jsonBuffer.parseObject(input);
  const char* phone_number = root["phone_number"];
  Serial.println(phone_number);
};

void loop() {
  int is_instruction = 0;
  while(client.available() || client.connected()) {
    char c = client.read();
    if('{' == c){
      is_instruction = 1;
    }
    if(is_instruction){
      input_append(c);
    }
    if('}' == c){
      is_instruction = 0;
      do_instruction();
      clear_input();
    }
  }
  Serial.println();
  Serial.println("disconnecting.");
  client.stop();
  Serial.println("FINISHED.");
  for (;;)
    ;
}
