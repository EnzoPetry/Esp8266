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

#define SERVER_IP "192.168.1.42"
#define URL "https://dispenser.berjooj.com/api/dispenser"
#define REDE "Rede Sankleyy"
#define SENHA  "Olasenhor1"

ESP8266WiFiMulti WiFiMulti;

const char fingerprint[] PROGMEM =  "20 FD 3D 2C 4D E2 CF FE C4 6E 1F BB AA E6 08 18 A4 F1 C6 86";

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(REDE, SENHA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectando...\n");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    https.begin(*client, URL);
    https.addHeader("Content-Type", "application/json");

    Serial.print("Enviando...\n");
    DynamicJsonDocument dados(1024);
    dados["token"] = "b8099e5c-d426-4d4b-89be-9ab082f4018f";
    dados["uses"] = 70;
    dados["entries"] = 100;
    dados["type"] = 1;
    String info;
    serializeJson(dados, info);

    int httpCode = https.POST(info);
    
    Serial.printf(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = https.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
      }else {
      Serial.printf("Erro no Envio - Código: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  }

  delay(10000);
}
