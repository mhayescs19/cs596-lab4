#include <Arduino.h>
#include <BLEDevice.h>

#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define GREEN_PIN 27 // output LED pin

class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();

     if(value == "on") {
      digitalWrite(GREEN_PIN, HIGH);
     } else if (value == "off") {
      digitalWrite(GREEN_PIN, LOW);
     }
   }
};
 
void setup() {
 Serial.begin(9600); // ensure that the serial output is readable

 pinMode(GREEN_PIN, OUTPUT); // Set the green pin as an output

 
 BLEDevice::init("SDSUCS");
 BLEServer *pServer = BLEDevice::createServer();
 
 BLEService *pService = pServer->createService(SERVICE_UUID);
 
 BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
 
 pCharacteristic->setCallbacks(new MyCallbacks());
 
 pCharacteristic->setValue("Hello World");
 pService->start();
 
 BLEAdvertising *pAdvertising = pServer->getAdvertising();
 pAdvertising->start();
}
 
void loop() {
 delay(2000);
}