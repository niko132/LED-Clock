#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#include <TZ.h>
#include <time.h>

#define MYTZ TZ_Europe_Berlin

extern "C" uint8_t sntp_getreachability(uint8_t);

class RTCClass {
private:
    bool getNtpServer(bool reply = false);
    tm* getLocalTime();

public:
    void begin();

    int getSeconds();
    int getMinutes();
    int getHours();

    int getDay();
    int getMonth();
    int getYear();

    int getDayOfWeek();
    int getDayOfYear();

    bool isDst();
};

extern RTCClass RTC;

#endif // RTC_H
