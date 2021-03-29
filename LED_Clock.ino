#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>

#include "ElementManager.h"
#include "RTC.h"

#include "FilterLightUp.h"

#ifndef STASSID
#define STASSID "WGLan"
#define STAPSK  "94384322823429699220"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

IPAddress ip(192, 168, 178, 114);

IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);

#define LED_PIN     3
#define NUM_LEDS    232
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60

ElementManager elementManager(leds, NUM_LEDS);

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

  Element *elem1 = elementManager.addElement(ELEMENT_7_SEGMENTS);
  Element *elem2 = elementManager.addElement(ELEMENT_7_SEGMENTS);
  Element *elem3 = elementManager.addElement(ELEMENT_COLON);
  Element *elem4 = elementManager.addElement(ELEMENT_7_SEGMENTS);
  Element *elem5 = elementManager.addElement(ELEMENT_7_SEGMENTS);

  Serial.println("Elements added: " + String(elem1 != NULL) + " " + String(elem2 != NULL) + " " + String(elem3 != NULL) + " " + String(elem4 != NULL) + " " + String(elem5 != NULL));

  elem1->addFilter(new FilterLightUp(10000));
  elem2->addFilter(new FilterLightUp(10000));
  elem3->addFilter(new FilterLightUp(10000));
  elem4->addFilter(new FilterLightUp(10000));
  elem5->addFilter(new FilterLightUp(10000));
}

unsigned long lastUpdateMillis = 0;

void loop()
{
    unsigned long startMillis = millis();
    ArduinoOTA.handle();

    if (startMillis - lastUpdateMillis > 1000.0 / UPDATES_PER_SECOND) {
        int hourDigit0 = RTC.getHours() / 10;
        int hourDigit1 = RTC.getHours() % 10;
        int minuteDigit0 = RTC.getMinutes() / 10;
        int minuteDigit1 = RTC.getMinutes() % 10;

        Element7Segments *minute1Elem = (Element7Segments*) elementManager.getElementAt(0);
        Element7Segments *minute0Elem = (Element7Segments*) elementManager.getElementAt(1);
        Element7Segments *hour1Elem = (Element7Segments*) elementManager.getElementAt(3);
        Element7Segments *hour0Elem = (Element7Segments*) elementManager.getElementAt(4);

        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(255, 0, 0);
        }


        hour0Elem->display(hourDigit0);
        hour1Elem->display(hourDigit1);
        minute0Elem->display(minuteDigit0);
        minute1Elem->display(minuteDigit1);

        elementManager.applyFilter();

        FastLED.show();



        Serial.println("running");
        lastUpdateMillis = startMillis;
    }
}
