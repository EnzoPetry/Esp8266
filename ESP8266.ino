#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define SERVER_IP "192.168.1.42"

#define STASSID1 "Rede Sankleyy"
#define STAPSK1  "Olasenhor1"
#define STASSID2 "TESTE A"
#define STAPSK2  "12345678"
#define echoPinEnt 13 // attach pin D4 Arduino to pin Echo of HC-SR04
#define trigPinEnt 15 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPinSai 12 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPinSai 14 //attach pin D1 Arduino to pin Trig of HC-SR04



ESP8266WiFiMulti WiFiMulti;

long durationEnt; // variable for the duration of sound wave travel
int distanceEnt; // variable for the distance measurement
long durationSai; // variable for the duration of sound wave travel
int distanceSai; // variable for the distance measurement
int entrada = 0;
int saida = 0;
int pessoas = 0;

void setup() {

  Serial.begin(115200);

  pinMode(trigPinEnt, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinEnt, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPinSai, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinSai, INPUT); // Sets the echoPin as an INPUT

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
  //wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient https;

    Serial.print("[HTTP] begin...\n");
    //configure traged server and url
    https.begin(client, "http://dispenser.berjooj.com/api/flow"); //HTTP
    https.addHeader("Content-Type", "application/json");

    digitalWrite(trigPinEnt, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinEnt, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinEnt, LOW);
    durationEnt = pulseIn(echoPinEnt, HIGH);
    distanceEnt = durationEnt * 0.034 / 2;
    Serial.println(distanceEnt);
    Serial.println("A");
    delay(250);

    digitalWrite(trigPinSai, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinSai, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinSai, LOW);
    durationSai = pulseIn(echoPinSai, HIGH);
    distanceSai = durationSai * 0.034 / 2;
    Serial.println(distanceSai);
    Serial.println("B");
    delay(250);

    if (distanceEnt <= 30 && entrada == saida) {
      entrada += 1;
      while (entrada > saida) {
        Serial.println("Aguardando Entrada");
        digitalWrite(trigPinSai, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPinSai, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPinSai, LOW);
        durationSai = pulseIn(echoPinSai, HIGH);
        distanceSai = durationSai * 0.034 / 2;
        Serial.println("Aguardando Saida");
        delay(250);
        if (distanceSai <= 30) {
          saida += 1;
          pessoas += 1;
          Serial.println(pessoas);
          delay(1200);
        }
      }
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header and body
      DynamicJsonDocument doc(2048);
      doc["token"] = "b8099e5c-d426-4d4b-89be-9ab082f4018f";
      doc["entries"] = pessoas;
      String json;
      serializeJson(doc, json);
      int httpCode = https.POST(json);
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);


      //httpCode will be negative on error
      if (httpCode <= 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
        Serial.println(json);
        const String& payload = https.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = https.getString();
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      }
    } else if (distanceSai <= 30 && entrada == saida) {
      saida += 1;
      while (entrada < saida) {
        digitalWrite(trigPinEnt, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPinEnt, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPinEnt, LOW);
        durationEnt = pulseIn(echoPinEnt, HIGH);
        distanceEnt = durationEnt * 0.034 / 2;
        Serial.println("Aguardando Saida");
        delay(250);
        if (distanceEnt <= 30) {
          entrada += 1;
          Serial.println(pessoas);

          delay(1200);
        }
      }
    }
  }
  delay(250);
}
