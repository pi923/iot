#include <WiFi.h>
#include "ThingSpeak.h"

char ssid[] = "iPhone mark";                    //
char pass[] = "0956950402";                         
unsigned long myChannelNumber = 3011294;            //à¹€à¸¥à¸‚ ID
const char * myWriteAPIKey = "0UFJOHT2LHRIFAVX";    //API KEY

int keyIndex = 0;

WiFiClient  client;

void setup() 
{
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() 
{
  ThingSpeak.setField(1, random(100));
  ThingSpeak.setField(2, random(200));
  ThingSpeak.setField(3, random(300));
  thingspeak_multi_write();
  delay(2000);
}

void wifi_reconnect() 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) 
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}

void thingspeak_single_write(byte ch, int value) 
{
  int x = ThingSpeak.writeField(myChannelNumber, ch, value, myWriteAPIKey);
  if (x == 200) 
  {
    Serial.println("Channe " + String(ch) + " update value " + String(value) + " successful.");
  }
  else 
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void thingspeak_multi_write() 
{
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) 
  {
    Serial.println("Multichannel update successful.");
  }
  else 
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}