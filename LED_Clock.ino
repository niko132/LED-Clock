#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define FASTLED_ALLOW_INTERRUPTS 0

#include <NTPClient.h>
#include <FastLED.h>

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
#define BRIGHTNESS  127
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 140

// 2 hours offset
const long utcOffsetInSeconds = 1 * 3600;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int prevNumbers[] = {0, 0, 0, 0};
int numbers[] = {0, 0, 0, 0};
int tmp[] = {0, 0, 0, 0};

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
    
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  timeClient.begin();
}

unsigned char hue = 0;
unsigned char sat = 0;
unsigned long lastUpdateMillis = 0;


unsigned long lastTimeUpdate = 0;

unsigned char numberRep[10][7] = {
  {255, 255, 255, 0, 255, 255, 255},
  {255, 0, 0, 0, 255, 0, 0},
  {255, 255, 0, 255, 0, 255, 255},
  {255, 255, 0, 255, 255, 255, 0},
  {255, 0, 255, 255, 255, 0, 0},
  {0, 255, 255, 255, 255, 255, 0},
  {0, 255, 255, 255, 255, 255, 255},
  {255, 255, 0, 0, 255, 0, 0},
  {255, 255, 255, 255, 255, 255, 255},
  {255, 255, 255, 255, 255, 255, 0}
};

unsigned long lastChangeMillis = 0;

void loop()
{
  unsigned long startMillis = millis();
  
  ArduinoOTA.handle();

  timeClient.update();

  

  /*
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
  }

  FastLED.show();

  return;
  */

  
  
  
  tmp[0] = timeClient.getHours() / 10;
  tmp[1] = timeClient.getHours() % 10;
  tmp[2] = timeClient.getMinutes() / 10;
  tmp[3] = timeClient.getMinutes() % 10;

  if (memcmp(tmp, numbers, 4 * sizeof(int)) != 0) { // numbers changed
    lastChangeMillis = startMillis;

    prevNumbers[0] = numbers[0];
    prevNumbers[1] = numbers[1];
    prevNumbers[2] = numbers[2];
    prevNumbers[3] = numbers[3];
  }

  numbers[0] = tmp[0];
  numbers[1] = tmp[1];
  numbers[2] = tmp[2];
  numbers[3] = tmp[3];

  if (startMillis - lastUpdateMillis > 1000.0 / UPDATES_PER_SECOND) {
    /*
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue, 255, 255);
    }
    
    FastLED.show();
    hue++;
    */

    CHSV fromColor(0, 192, 255);
    CHSV toColor(128, 192 + 32, 255);

    if (numbers[3]% 2 == 0) {
      CHSV tmp = fromColor;
      fromColor = toColor;
      toColor = tmp;
    }

    unsigned int ledOffset = 0;

    for (int i = 0; i < 4; i++) {
      int number = numbers[3 - i];

      if (i == 2) { // middle dots
        for (int i = 0; i < 8; i++) {
          CHSV mah = toColor;

            double val = (millis() - lastChangeMillis) / (2 * 1000.0); // 10s transition
            if (val > 1.0) {
              val = 1.0;
            }
            unsigned char amount = (unsigned char) (val * 255.0);
            mah = blend(fromColor, toColor, amount);

          CRGB led(mah);
          leds[ledOffset++] = mah;
        }
      }
      
      for (int j = 0; j < 7; j++) {
        unsigned char edge = numberRep[number][j];
        unsigned char prevEdge = numberRep[prevNumbers[3 - i]][j];

        double val = (startMillis - lastChangeMillis) / (2 * 1000.0); // 300ms transition
        if (val > 1.0) {
          val = 1.0;
        }
        edge = (unsigned char) (edge * val + prevEdge * (1.0 - val));

        for (int i = 0; i < 8; i++) {
          CHSV mah = fromColor;

            val = (millis() - lastChangeMillis) / (2 * 1000.0); // 10s transition
            if (val > 1.0) {
              val = 1.0;
            }
            unsigned char amount = (unsigned char) (val * 255.0);
            mah = blend(toColor, fromColor, amount);

          CRGB led(mah);
          led.nscale8(edge);
          leds[ledOffset++] = led;
        }
      }
    }

    FastLED.show();
    hue = (unsigned char) (startMillis / 100.0);

    lastUpdateMillis = startMillis;
  }
}
