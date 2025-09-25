#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi config
const char* ssid = "iphone mark";
const char* password = "0956950402";

// Line Notify Token
const char* lineToken = "ioajvaao62ags5s35daioajjm45aoadc";

// Counter for A2
int a2Counter = 0;
SemaphoreHandle_t a2Mutex;

void sendLineNotify(String message) {
  HTTPClient http;

  http.begin("");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer " + String(lineToken));

  String payload = "message=" + message;
  int httpResponseCode = http.POST(payload);
  http.end();
}

void TaskA1(void *parameter) {
  while (true) {
    Serial.println("Sending A1");
    vTaskDelay(pdMS_TO_TICKS(2000));  // 2 seconds
  }
}

void TaskA2(void *parameter) {
  while (true) {
    Serial.println("Sending A2");

    // Increment A2 counter safely
    xSemaphoreTake(a2Mutex, portMAX_DELAY);
    a2Counter++;
    if (a2Counter >= 5) {
      a2Counter = 0;
      xSemaphoreGive(a2Mutex);
      sendLineNotify("A2 was sent 5 times");
    } else {
      xSemaphoreGive(a2Mutex);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));  // 5 seconds
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  a2Mutex = xSemaphoreCreateMutex();

  // Create tasks
  xTaskCreatePinnedToCore(TaskA1, "Task A1", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskA2, "Task A2", 2048, NULL, 1, NULL, 1);
}

void loop() {
  // Nothing to do here
}
