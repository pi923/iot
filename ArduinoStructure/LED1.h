#ifndef LED1_H
#define LED1_H
#include <Arduino.h>

class LED
{
  public:
    LED(int pin);
    void ON(int Status);
    void OFF(int Status);

  private:
    int _pin;
};

#endif