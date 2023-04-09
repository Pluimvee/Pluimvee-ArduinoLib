#include "Timer.h"
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
Timer::Timer(uint32_t ms) {
  alarm(ms);
}

// set the current time
void Timer::start() {
  _start = millis();  
}

// set the alarm and restart
void Timer::alarm(uint32_t ms) {
  _alarm = ms;
  start();
}

// check the alarm
bool Timer::alarm() const {
  return elapsed() > _alarm;
}

////////////////////////////////////////////////////////////////////////////////////////////
// getters
int Timer::seconds() const {
  return elapsed() / 1000;
}

int Timer::minutes() const {
  return seconds() / 60;
}

String Timer::stamp() const {
  uint32_t e = elapsed();
  char buf[32];
  sprintf(buf, "%02d:%02d.%03d", e/60000,e/1000%60, e%1000);
  return String(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////
/** get the millis after _start with overflow correction **/
uint32_t Timer::elapsed() const {
  uint32_t now = millis();
  if (now < _start) 
    return (uint32_t)0xFFFFFFFF - _start + now;

  return now - _start;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
#define TZ_DST_AMSTERDAM "CET-1CEST,M3.5.0/2,M10.5.0/3"

bool Clock::ntp_sync()
{
  configTime(TZ_DST_AMSTERDAM, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  struct tm tmstruct;
  int retry=0;
  while (getLocalTime(&tmstruct) == false) {
    if (retry++ > 10)
      return false;
    delay(500);
  }
  adjust(DateTime(tmstruct.tm_year, tmstruct.tm_mon+1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec));
  return true;
}

