#include "LED1.h"

LED::LED(int pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void LED::ON(int Status)
{
  digitalWrite(_pin, Status);
}

void LED::OFF(int Status)
{
  digitalWrite(_pin, Status);
}