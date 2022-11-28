//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266WiFiMulti.h>
//#include <WiFiClientSecureBearSSL.h>
//#include <WiFiClient.h>
//#include <ArduinoJson.h>

#define SERVER_IP "192.168.1.42"

#define STASSID1 "Rede Sankleyy"
#define STAPSK1  "Olasenhor1"
#define STASSID2 "TESTE A"
#define STAPSK2  "12345678"
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04



//ESP8266WiFiMulti WiFiMulti;

//const char fingerprint[] PROGMEM =  "49 07 75 57 93 8F D6 CD EE 60 A0 48 6B 59 FB ED 02 E3 D4 A4";
int sensor = 4;
//int res = 5;
//int usos = 0;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int entrada = 0;
int saida = 0;
int pessoas = 0;

void setup() {

  Serial.begin(115200);
  pinMode(sensor, INPUT);
  //pinMode(res, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  //Serial.println();
  //Serial.println();
  // Serial.println();

  //WiFi.begin(STASSID2, STAPSK2);


  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(2000);
  //  Serial.print(".");
  //}
  // Serial.println("");
  //Serial.print("Connected! IP address: ");
  //Serial.println(WiFi.localIP());


}

void loop() {
  // wait for WiFi connection
  //if ((WiFi.status() == WL_CONNECTED)) {

  // std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  // client->setFingerprint(fingerprint);
  // HTTPClient https;

  //Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  // https.begin(*client, "https://dispenser.berjooj.com/api/dispenser"); //HTTP
  // https.addHeader("Content-Type", "application/json");

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  //Serial.println(distance);
  bool tes = digitalRead(sensor);

  if (distance <= 30 && entrada == saida) {
    entrada += 1;
    while (entrada > saida) {
     // Serial.println("Aguardando Entrada");
      if ((digitalRead(sensor) == HIGH)) {
        saida += 1;
        pessoas += 1;
        Serial.println(pessoas);
        delay(1000);
      }
    }
  } else if ((digitalRead(sensor) == HIGH) && entrada == saida) {
    saida += 1;
    while (entrada < saida) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
      //.println("Aguardando Saida");
      if (distance <= 30) {
        entrada += 1;
        Serial.println(pessoas);

        delay(2000);
      }
    }
  }//else{
  //Serial.println("Nada");
  //}

  //Serial.print("[HTTPS] POST...\n");
  // start connection and send HTTP header and body
  //usos++;
  //DynamicJsonDocument doc(2048);
  //doc["token"] = "b8099e5c-d426-4d4b-89be-9ab082f4018f";
  //doc["uses"] = usos;
  // doc["entries"] = usos;
  //doc["type"] = 1;
  // String json;
  // serializeJson(doc, json);
  //int httpCode = https.POST(json);

  // httpCode will be negative on error
  //if (httpCode > 0) {
  // HTTP header has been send and Server response header has been handled
  // Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  // Serial.println(json);

  // file found at server
  // if (httpCode == HTTP_CODE_OK) {
  //    const String& payload = https.getString();
  //   Serial.println("received payload:\n<<");
  //   Serial.println(payload);
  //  Serial.println(">>");
  //  }
  // }

  //}
  //delay(2000);
  //https.end();
}
