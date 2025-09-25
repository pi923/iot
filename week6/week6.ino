#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp32httpota.h>

// Line Notify Token
const char* lineToken = "atppvd1534qtodpv54xpeso2";

// OTA Config
const char* versionURL = "http://127.0.0.1:1880/#flow/ba16e8a8e74e5d4f";  // JSON path
String currentVersion = "1.0.0";  // Local firmware version

int a2Counter = 0;
SemaphoreHandle_t a2Mutex;

void sendLineNotify(const String& message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://notify-api.line.me/api/notify");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Bearer " + String(lineToken));

    int httpCode = http.POST("message=" + message);
    http.end();
    Serial.println("Line Notify sent: " + message);
  }
}

void TaskA1(void *parameter) {
  while (true) {
    Serial.println("Sending A1");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2s
  }
}

void TaskA2(void *parameter) {
  while (true) {
    Serial.println("Sending A2");

    xSemaphoreTake(a2Mutex, portMAX_DELAY);
    a2Counter++;
    if (a2Counter >= 5) {
      a2Counter = 0;
      xSemaphoreGive(a2Mutex);
      sendLineNotify("A2 was sent 5 times");
    } else {
      xSemaphoreGive(a2Mutex);
    }

    vTaskDelay(pdMS_TO_TICKS(5000)); // 5s
  }
}

void checkOTAUpdate() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(versionURL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (!error) {
      String newVersion = doc["version"];
      String firmwareURL = doc["url"];
      if (newVersion != currentVersion) {
        Serial.println("New version found: " + newVersion);
        ESP32HTTPOTA ota(firmwareURL.c_str());
        ota.downloadAndUpdate();
      } else {
        Serial.println("Firmware is up to date.");
      }
    }
  } else {
    Serial.println("Failed to fetch version file.");
  }
  http.end();
}

void setup() {
  Serial.begin(115200);

  // Connect WiFi using WiFiManager
  WiFiManager wm;
  bool res = wm.autoConnect("ESP32-WIFI-SETUP");

  if (!res) {
    Serial.println("Failed to connect, rebooting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi.");

  // Check OTA update on startup
  checkOTAUpdate();

  a2Mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(TaskA1, "Task A1", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskA2, "Task A2", 2048, NULL, 1, NULL, 1);
}

void loop() {
  // Optional: check OTA every 30 mins
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 1800000UL) { // 30 minutes
    checkOTAUpdate();
    lastCheck = millis();
  }
  delay(100);
}
