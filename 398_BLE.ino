#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define UUIDs for BLE
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic; // Pointer to characteristic

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE");

  BLEDevice::init("SHI-1234"); // Naming device
  BLEServer *pServer = BLEDevice::createServer(); // Starting server
  BLEService *pService = pServer->createService(SERVICE_UUID); 

  pCharacteristic = pService->createCharacteristic( // Defining charecteristic 
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | // Allowing device to read
                      BLECharacteristic::PROPERTY_WRITE | // Allowing device to write                   
                      BLECharacteristic::PROPERTY_NOTIFY // Enable notifications
                    );

  pCharacteristic->setValue("0"); // Initialize and start
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE Device is Ready.");
}

void loop() {
  uint8_t heartRate = random(0, 255); // Generate a random number 
  // change to read sensor every second, maybe filter, average 

  if(heartRate > 199){
    Serial.print("Heart rate: " + String(heartRate) + " BPM Abnormal heart beat detected\n"); // Message to serial monitor
    pCharacteristic->setValue(&heartRate, sizeof(heartRate)); // Send as raw number
    pCharacteristic->notify(); // Send update to device  
    // Some kind of alert vibration on the device
  }

  else{
    Serial.print("Heart rate: " + String(heartRate) + " BPM\n"); // Message to serial monitor
    pCharacteristic->setValue(&heartRate, sizeof(heartRate)); // Send as raw number
    pCharacteristic->notify(); // Send update to device   
  } 


  delay(5000); // Wait 5s before sending next value
}
