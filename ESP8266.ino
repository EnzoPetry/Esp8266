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
#include <EEPROM.h>
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
#define STASSID1 "Rede Sankleyy"
#define STAPSK1  "Olasenhor1"
#define STASSID2 "TESTE A"
#define STAPSK2  "12345678"
#endif

ESP8266WiFiMulti WiFiMulti;

const char fingerprint[] PROGMEM =  "49 07 75 57 93 8F D6 CD EE 60 A0 48 6B 59 FB ED 02 E3 D4 A4";
int sensor = 4;
int res = 5;
int usos = 0;

void setup() {

  Serial.begin(115200);
  pinMode(sensor, INPUT);
  pinMode(res, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID2, STAPSK2);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
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

    bool tes = digitalRead(sensor);
    delay(500);

    if (tes == LOW) {
      Serial.println("Nada");
    }
    else if ((digitalRead(sensor) == HIGH)) {
      while ((digitalRead(sensor) == HIGH)) {
        digitalWrite(res,HIGH);
        Serial.println("LIGA");
        delay(2000);
        digitalWrite(res,LOW);
        Serial.println("DESLIGA");
        delay(1000);
      }
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header and body
      usos++;
      DynamicJsonDocument doc(2048);
      doc["token"] = "b8099e5c-d426-4d4b-89be-9ab082f4018f";
      doc["uses"] = usos;
      doc["entries"] = usos;
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
      }

    }
    delay(2000);
    EEPROM.end();
    https.end();
  }
}
