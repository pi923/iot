#include <BLEDevice.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32_Mark"); // ✅ ตั้งชื่อ BLE ให้ไม่ซ้ำ
  Serial.print("MAC: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());

  // ทำ BLE ปกติ
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");
  BLECharacteristic *pChar = pService->createCharacteristic(
    "abcd1234-abcd-1234-abcd-1234567890ab",
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pChar->setValue("Hello!");
  pService->start();
  pServer->getAdvertising()->start();
}

void loop() {}
