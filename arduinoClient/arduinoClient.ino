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

// APN Settings
char GPRS_APN[] = "web.be";
char GPRS_LOGIN[] = "web";
char GPRS_PASSWORD[] = "web";

int printFreeRam (){
  extern int __heap_start, *__brkval;
  int v;
  Serial.println((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
}

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

void loop() {
  printFreeRam();
  if(ready_to_start()){
    do_actions();
    set_finished();
  }
  delay(5000);
  printFreeRam();
}

