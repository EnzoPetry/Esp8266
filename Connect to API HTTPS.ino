/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "192.168.1.42"

#ifndef STASSID
#define STASSID "Rede Sankleyy"
#define STAPSK  "Olasenhor1"
#endif

ESP8266WiFiMulti WiFiMulti;

const char fingerprint[] PROGMEM =  "20 FD 3D 2C 4D E2 CF FE C4 6E 1F BB AA E6 08 18 A4 F1 C6 86";

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    https.begin(*client, "https://dispenser.berjooj.com/api/dispenser"); //HTTP
    https.addHeader("Content-Type", "application/json");

    Serial.print("[HTTPS] POST...\n");
    // start connection and send HTTP header and body
    DynamicJsonDocument doc(2048);
    doc["token"] = "b8099e5c-d426-4d4b-89be-9ab082f4018f";
    doc["uses"] = 70;
    doc["entries"] = 100;
    doc["type"] = 1;
    String json;
    serializeJson(doc, json);
    int httpCode = https.POST(json);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      Serial.println(json); 

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = https.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  }

  delay(10000);
}
