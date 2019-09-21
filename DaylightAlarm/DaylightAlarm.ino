#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include "config.h"

CRGB leds[NUM_LEDS];
WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("Server started at IP address: ");
  Serial.println(WiFi.localIP());

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); //You might want to change this line, depending on your LED strip
  server.on("/on", handle_on);
  server.on("/off", handle_off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Webserver started");
}

void loop() {
    server.handleClient();
}


void handle_on(){
  allWhite(true);
  server.send(200, "text/plain", "All lights set to ON");  
}

void handle_off(){
  allWhite(false);
  server.send(200, "text/plain", "All lights set to OFF");    
}

void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
}

void allWhite(boolean on) {
  if (on) {
    for (int i = 0; i < NUM_LEDS; i = i + 1) {
      leds[i] = CRGB(100, 100, 100);
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i = i + 1) {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}
