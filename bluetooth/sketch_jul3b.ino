#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// สร้าง UUID แบบสุ่มหรือใช้ของตัวเอง
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-abcd-1234-abcd-1234567890ab"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("📱 เชื่อมต่อกับ iPhone แล้ว");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("❌ ยกเลิกการเชื่อมต่อ");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
  String value = String(pCharacteristic->getValue().c_str());  // 👈 ใช้ String แบบ Arduino

  if (value.length() > 0) {
    Serial.print("📥 ข้อความจาก iPhone: ");
    Serial.println(value);

    pCharacteristic->setValue("✅ รับข้อมูลแล้ว!");
    pCharacteristic->notify();
  }
}

  
};

void setup() {
  Serial.begin(115200);
  Serial.println("กำลังเริ่ม BLE...");

  BLEDevice::init("ESP32_BLE510");  // ชื่อที่จะเห็นบน iPhone
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue("สวัสดีจาก ESP32!");

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("✅ รอ iPhone เชื่อมต่อผ่านแอป Bluetooth LE");
}

void loop() {
  // ไม่ต้องทำอะไรใน loop สำหรับ BLE
}
