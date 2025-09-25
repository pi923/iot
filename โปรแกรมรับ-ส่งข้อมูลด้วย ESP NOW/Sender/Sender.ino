#include <WiFi.h>
#include <esp_now.h>

// กำหนด MAC Address ของ Receiver
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};  // แก้ตาม MAC จริง

// สร้างโครงสร้างข้อมูล
typedef struct struct_message {
    int id;
    float temperature;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);

  // ตั้งค่า WiFi ให้เป็น Station
  WiFi.mode(WIFI_STA);
  Serial.println("Sender MAC Address: ");
  Serial.println(WiFi.macAddress());

  // เริ่มต้น ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // เพิ่ม Peer (Receiver)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Callback เมื่อส่งเสร็จ
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  myData.id = 1;
  myData.temperature = random(20, 30); // จำลองอุณหภูมิ

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}
