#pragma once
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////////
// A timer class based on milliseconds
////////////////////////////////////////////////////////////////////////////////////////////
class Timer 
{
public:
  Timer(uint32_t ms=0);       // argument sets the alarm
  void start();               // set to current time
  uint32_t elapsed() const;   // elapsed ms, corrected for rollover (which occurs each 49.7 days)
  int seconds() const;        // elapsed sec
  int minutes() const;        // elapsed min
  String stamp() const;       // elapsed timestamp as mm:ss.mmm
  
  // alarm
  void set(uint32_t ms);    // sets the alarm target in ms from now
  bool passed() const;      // returns true when target reached (ms elapsed)

  inline operator bool() const { return passed(); };
private:
  uint32_t _start = 0,        // the time we started
           _alarm = 0;        // alarm target
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

