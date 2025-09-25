#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "iPhone mark";
const char* password = "0956950402";

const int outputPin = 1;

WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="th">
<head>
  <meta charset="UTF-8">
  <title>ควบคุม ESP32</title>
  <style>
    body {
      background-color: #ffe6f0;
      font-family: sans-serif;
      text-align: center;
      padding-top: 50px;
    }
    a.button {
      display: inline-block;
      padding: 15px 30px;
      margin: 20px;
      text-decoration: none;
      font-size: 20px;
      border-radius: 10px;
      color: white;
    }
    .on {
      background-color: green;
    }
    .off {
      background-color: red;
    }
  </style>
</head>
<body>
  <h1>ควบคุมอุปกรณ์ด้วย ESP32</h1>
  <a class="button on" href="/on">เปิด (ON)</a>
  <a class="button off" href="/off">ปิด (OFF)</a>
</body>
</html>

)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/on", []() {
    digitalWrite(outputPin, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/off", []() {
    digitalWrite(outputPin, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
