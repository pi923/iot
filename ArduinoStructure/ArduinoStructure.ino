#include "Lib2.h"
#include "LED1.h"

LED led(2);

void setup() 
{
  Serial.begin(115200);
  led.OFF(0);
}

void Add1(int x, int y)
{
  Serial.println(x + y);
}

void loop() 
{
  int x;
  Add1(4, 5); // Self Call
  Sub1(5, 4); // Ext ino call
  x = Mul1(5, 4); // Ext lib call
  Serial.println(x);
}
