/*

 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <math.h>

const char *ssid = "xxxxxxxxxxx";
const char *password = "xxxxxxxxxxx";

const String host = "192.xxx.xxx.xxx";
const String stub = "/tfl/busprex/stop/";
const String bus_stop = "xxxxxxxxxxxxx";


const int button = 16; // push button is connected
const int maxAttempts = 10;
int retry = 0;
const int default_apiDelay = 60000;
int apiDelay = default_apiDelay;

const int maxWiFiAttempts = 5;
const int maxWiFiTimeOut = 5000;



void setup() {
  hardwareSetUp();
  wifiSetUp();
  executeCommand();
}


void loop() {
//  checkTheButton();

  apiRequest();
}

void hardwareSetUp() {
  pinMode(button, INPUT); // declare push button as input
}

void checkTheButton() {
  int fetch = digitalRead(button);
     
  if (fetch == HIGH) {
    executeCommand();
   } else {
    //apiDelay = default_apiDelay * 60 * 12;
    //delay(apiDelay);
   }
}

void executeCommand() {
  for (int i=0; i <= maxAttempts; i++){
    apiDelay = default_apiDelay;
    apiRequest();
  }
}

void wifiSetUp() {

  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
 
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  int timeout = 0;
  for (int wifiAttempts=0; wifiAttempts <= maxWiFiAttempts; wifiAttempts++){
    while ((WiFi.status() != WL_CONNECTED) && (wifiAttempts < 5) && (timeout < maxWiFiTimeOut)) {
      delay(500);
      Serial.print(".");
      timeout += 500;
    }
    timeout = 0;
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void apiRequest() {

  String completeURI = "http://" + host + stub + bus_stop;

  HTTPClient http;
  int beginResult = http.begin(completeURI);
  int requestResult = http.GET();
  String payload = http.getString();
  
  Serial.println(requestResult);

  int requestCodeResult = floor(requestResult / 100);
  String responseResult = "Successfull Request";

Serial.println("requestCodeResult " +  String(requestCodeResult));

  if (requestCodeResult == 2) {
    responseResult = "Successfull Request";
  } else if (requestCodeResult == 4) {
    responseResult = "Client Error Response: " + String(requestResult);
    } else if (requestCodeResult == 5) {
      responseResult = "Server Error Response: " + String(requestResult);
      } else {
        responseResult = "Other Error Response: " + String(requestResult);
      }


  Serial.println(responseResult);      
  Serial.println(payload);
  http.end();

  delay(apiDelay);
}


