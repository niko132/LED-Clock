#include "RTC.h"

#include <Arduino.h>

bool RTCClass::getNtpServer(bool reply) {
    uint32_t timeout = millis();

    configTime(MYTZ, "fritz.box", "de.pool.ntp.org", "pool.ntp.org");

    do {
        delay(25);
        if (millis() - timeout >= 1000) {
            Serial.printf("Warten auf NTP-Antwort %02ld sec\n", (millis() - timeout) / 1000);
            delay(975);
        }

        reply = sntp_getreachability(0) || sntp_getreachability(1) || sntp_getreachability(2);
    } while (millis() - timeout <= 10000 && !reply);

    return reply;
}

void RTCClass::begin() {
    bool success = getNtpServer();
    Serial.println("NTP Sync Success: " + String(success));
}

tm* RTCClass::getLocalTime() {
    time_t now = time(NULL);
    return localtime(&now);
}

int RTCClass::getSeconds() {
    return getLocalTime()->tm_sec;
}

int RTCClass::getMinutes() {
    return getLocalTime()->tm_min;
}

int RTCClass::getHours() {
    return getLocalTime()->tm_hour;
}

int RTCClass::getDay() {
    return getLocalTime()->tm_mday;
}

int RTCClass::getMonth() {
    return getLocalTime()->tm_mon + 1;
}

int RTCClass::getYear() {
    return getLocalTime()->tm_year + 1900;
}

int RTCClass::getDayOfWeek() {
    return (getLocalTime()->tm_wday + 6) % 7;
}

int RTCClass::getDayOfYear() {
    return getLocalTime()->tm_yday;
}

bool RTCClass::isDst() {
    return getLocalTime()->tm_isdst;
}

RTCClass RTC;
