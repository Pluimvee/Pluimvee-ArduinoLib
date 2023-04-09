////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
#include "LED.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
LED::LED(int pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  off();
}


////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
void LED::on() {
    _state = LOW;                // pull down to power led
    digitalWrite(_pin, _state);
}

void LED::off() {
    _state = HIGH;                // pull up to set led off
    digitalWrite(_pin, _state);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
void LED::blink()
{
  _state = !_state;
  digitalWrite(_pin, _state);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

