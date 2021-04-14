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
#include <FS.h>
#include <ESPAsyncWebServer.h>

#include "config.h"

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

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
Espalexa espalexa;

ElementClock clockElem(leds, &espalexa);

String msg = "";

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



  // web server stuff
  if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
  }

  server.on("/free_heap", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", String(ESP.getFreeHeap()));
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  ws.onEvent([msg](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
      if(type == WS_EVT_CONNECT){
          Serial.printf("ws[%s][%u] connect", server->url(), client->id());
          Serial.println();

          DynamicJsonDocument doc(2048 * 2);
          JsonObject root = doc.to<JsonObject>();
          clockElem.toJson(root);

          String output = "";
          serializeJson(doc, output);

          client->text(output);
      } else if(type == WS_EVT_DISCONNECT){
          Serial.printf("ws[%s][%u] disconnect", server->url(), client->id());
          Serial.println();
      } else if(type == WS_EVT_ERROR){
          Serial.printf("ws[%s][%u] error(%u): %s", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
          Serial.println();
      } else if(type == WS_EVT_PONG){
          Serial.printf("ws[%s][%u] pong[%u]: %s", server->url(), client->id(), len, (len)?(char*)data:"");
          Serial.println();
      } else if(type == WS_EVT_DATA){
          AwsFrameInfo *info = (AwsFrameInfo*)arg;
          if(info->final && info->index == 0 && info->len == len){
              //the whole message is in a single frame and we got all of it's data
              Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
              Serial.println();
              msg = "";

              if(info->opcode == WS_TEXT) { // we only care about text messages
                  for(size_t i=0; i < info->len; i++) {
                      msg += (char) data[i];
                  }

                  // TODO: message received
                  handleConfig(msg);
              }
          } else {
              //message is comprised of multiple frames or the frame is split into multiple packets
              if(info->index == 0){
                  if(info->num == 0) {
                      Serial.printf("ws[%s][%u] %s-message start", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
                      Serial.println();
                      msg = "";
                  }

                  Serial.printf("ws[%s][%u] frame[%u] start[%llu]", server->url(), client->id(), info->num, info->len);
                  Serial.println();
              }

              Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
              Serial.println();

              if(info->opcode == WS_TEXT) { // we only care about text messages
                  for(size_t i=0; i < len; i++) {
                      msg += (char) data[i];
                  }

                  if ((info->index + len) == info->len && info->final) {
                      // TODO: message received
                      handleConfig(msg);
                  }
              }
          }
      }
  });

  server.addHandler(&ws);
  server.begin();





  // fade in for 10 secs
  clockElem.addFilter(new BrightnessFilter(0, 255, 10000, 10000));

  // add breathing effect to the middle dots
  clockElem.getColonElement()->addFilter(new BreathingFilter(0, 255, 2000, 0));

  espalexa.begin();
}

void handleConfig(String json) {
    DynamicJsonDocument doc(5 * 2048);
    DeserializationError err = deserializeJson(doc, json);
    JsonObject root = doc.as<JsonObject>();

    if (err) {
        Serial.print("DeserializationError: ");
        Serial.println(err.c_str());
    }

    size_t memCapacity = doc.capacity();
    size_t memUsage = doc.memoryUsage();

    Serial.println("JSON: " + String(memUsage) + "/" + String(memCapacity));

    clockElem.patchJson(root);
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
