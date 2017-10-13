// libraries
#include <GSM.h>
#include <time.h>
#include <stdio.h>
#include <ArduinoJson.h>

static const char* PIN = "1111";
static const char* SERVER = "http://5a4ed1f2.ngrok.io";

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSMVoiceCall vcs;
GSM_SMS sms;
GPRS gprs;
GSMClient gsmClient;

const char* nodeId = "alfa";
const char* GET = "GET";
const char* POST = "POST";
static int actionIndex;
const int responseBufferSize = 100;

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
  if(readyToStart(gsmClient)){
    doActions(gsmClient);
    setFinished(gsmClient);
  }
  delay(5000);
  printFreeRam();
}

