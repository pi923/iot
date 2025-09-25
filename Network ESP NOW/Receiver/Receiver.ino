#include <WiFi.h>
#include <esp_now.h>

// สร้างโครงสร้างข้อมูลให้ตรงกับ sender
typedef struct struct_message {
    int id;
    float temperature;
} struct_message;

struct_message incomingData;

void setup() {
  Serial.begin(115200);

  // ตั้งค่า WiFi ให้เป็น Station
  WiFi.mode(WIFI_STA);
  Serial.println("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());

  // เริ่มต้น ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Callback เมื่่อมีข้อมูลเข้า
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // รอรับข้อมูล
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  Serial.print("Received ID: ");
  Serial.print(incomingData.id);
  Serial.print(" | Temp: ");
  Serial.println(incomingData.temperature);
}
