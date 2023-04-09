#pragma once

#include <Arduino.h>
#include <RTClib.h>

////////////////////////////////////////////////////////////////////////////////////////////
// The ntp_sync is used to get the time synced with NTP servers
// it can be used to adjust the RTClibs
// use it  after being connected to the WiFi on a regular (daily?) basis
////////////////////////////////////////////////////////////////////////////////////////////
class Clock : public RTC_Millis {
public:
  bool ntp_sync();
  inline uint_t unitxtime() { return lastUnix; };
};

////////////////////////////////////////////////////////////////////////////////////////////
// A timer class based on milliseconds
// 
////////////////////////////////////////////////////////////////////////////////////////////
class Timer 
{
public:
  Timer(uint32_t ms=0);       // arguments sets a alarm target
  void start();
  uint32_t elapsed() const;   // elapsed ms, corrected for rollover (which occurs each 49.7 days)
  int seconds() const;        // elapsed sec
  int minutes() const;        // elapsed min
  String stamp() const;       // elepsed timestamp as mm:ss.mmm
  
  // alarm
  void alarm(uint32_t ms);    // sets the alarm target in ms, also resets the offset
  bool alarm() const;         // returns true when target reached

private:
  uint32_t _start = 0,        // the time we started
           _alarm = 0;        // alarm target
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

