#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define SERVER_IP "http://comp590-a2.herokuapp.com/post/env"
#define STASSID "abbi"
#define STAPSK "hotspot1"

DHT dht(4, DHT11); //note: Node MCU’s D2 is raw pin #4. (see pin mapping)
void setup() {
  Serial.begin(115200);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to wifi...");
  }
  Serial.println("Connected to wifi");
  pinMode(4, INPUT);
  dht.begin();
}
void loop() {
  WiFiClient client;
  HTTPClient http;
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  if(isnan(h) || isnan(t)){
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  else{
    //Serial.println(h);
   // Serial.println(t);
   
    char info[40];
    sprintf(info, "{\"temp\": \"%.2f\", \"humidity\": \"%.2f\"}", t, h);
    Serial.println(info);

    if ((WiFi.status() == WL_CONNECTED)) {
      http.begin(client, SERVER_IP);
      http.addHeader("Content-Type", "application/json");
      int r = http.POST(info);
      Serial.println(r);
      http.end();
    }
  }
  delay(10000);
}