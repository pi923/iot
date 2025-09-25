
// WiFi Clint Demo1 by Min => Auto Send and Command Receive

#include <WiFi.h>

// ** Start WiFi Config Zone
char ssid[] = "ZPX/CoreUnit";
char pass[] = "51149000";

const char* host = "192.168.1.102";
const uint16_t port = 1500;
// ** End WiFi Config Zone

unsigned long previousMillis = 0;
long SendTime = 2000;
WiFiClient client;

void setup() 
{
  Serial.begin(115200);  //Initialize serial

  WiFi.mode(WIFI_STA);

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

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP()); /*Print the Local IP*/
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  previousMillis = millis();
}

void SendInfo()
{
  // Protocol => D1,D2\r\n
  String Info = String(random(400))+","+String(random(400));
  client.println(Info);
  Serial.println("Sent!");
}

void loop() 
{
  Serial.print("Connecting to ");
  Serial.println(host);

  if (client.connect(host, port)) {
    Serial.println("Connected to server");

    String message = "Hello from ESP-32 client!";
    client.println(message);
    Serial.print("Sent: ");
    Serial.println(message);

    while (client.connected() || client.available()) 
    {
      unsigned long currentMillis = millis();
      if ((currentMillis - previousMillis) >= SendTime)
      {
        previousMillis = currentMillis;
        SendInfo();
      }

      if (client.available()) 
      {
        String response = client.readStringUntil('#');
        Serial.print("Received from server: ");
        Serial.println(response);
        client.println("ok");

        if (response == "on#")
          Serial.println("Motor ON");
        else if (response == "off#")
            Serial.println("Motor OFF");
          else
            Serial.println("Command Error!");
      }
    }

    client.stop();
    Serial.println("Disconnected from server");
  } else 
  {
    Serial.println("Connection failed");
  }
}