#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include <ArduinoJson.h>

#include "ElementClock.h"
#include "RTC.h"

#include "BrightnessFilter.h"
#include "BreathingFilter.h"

#include "Espalexa.h"

#include "config.h"

const char* ssid = STASSID;
const char* password = STAPSK;

IPAddress ip(192, 168, 178, 118);

IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);

#define LED_PIN     3
#define NUM_LEDS    232
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60

Espalexa espalexa;

ElementClock clockElem(leds, &espalexa);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, gateway);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  RTC.begin();

  // fade in for 10 secs
  clockElem.addFilter(new BrightnessFilter(0, 255, 10000, 10000));

  // add breathing effect to the middle dots
  clockElem.getColonElement()->addFilter(new BreathingFilter(0, 255, 2000, 0));



  DynamicJsonDocument doc1(5120 * 2);
  deserializeJson(doc1, "{\"effect\":{\"name\":\"RandomColor\",\"angle\":0,\"scale\":1,\"angleSpeed\":2,\"shiftSpeed\":0}}");
  JsonObject root1 = doc1.as<JsonObject>();
  clockElem.patchJson(root1);



  DynamicJsonDocument doc(5120 * 2);
  JsonObject root = doc.to<JsonObject>();
  clockElem.toJson(root);

  String output = "";
  serializeJson(doc, output);

  Serial.println(output);


  espalexa.begin();
}

unsigned long lastUpdateMillis = 0;

void loop()
{
    unsigned long startMillis = millis();
    ArduinoOTA.handle();
    espalexa.loop();

    if (startMillis - lastUpdateMillis > 1000.0 / UPDATES_PER_SECOND) {
        clockElem.update(); // update the time and numbers
        FastLED.show();

        // Serial.println("Heap: " + String(ESP.getFreeHeap()));

        lastUpdateMillis = startMillis;
    }
}
