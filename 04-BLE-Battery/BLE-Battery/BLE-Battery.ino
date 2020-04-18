
// BLE Battery Monitor
// Once you installed the library "ArduinoBLE" you can find more examples under File > Examples > ArduinoBLE
// reference https://github.com/arduino-libraries/ArduinoBLE/blob/master/examples/Peripheral/BatteryMonitor/BatteryMonitor.ino
// You can use the mobile app "nRF Connect" to 

#include <ArduinoBLE.h> // The library for Bluetooth, install "ArduinoBLE", docs https://www.arduino.cc/en/Reference/ArduinoBLE

BLEService batteryService("180F"); // As defined in the official docs https://www.bluetooth.com/specifications/gatt/services/
BLEUnsignedCharCharacteristic batteryLevelChar("2A19", BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes, https://www.bluetooth.com/specifications/gatt/characteristics/

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("BatteryMonitor");
  BLE.setAdvertisedService(batteryService);
  batteryService.addCharacteristic(batteryLevelChar);
  BLE.addService(batteryService);
  
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // Note: central means a reading radio device in Blueooth LE terms
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);
    
    while (central.connected()) {
      int battery = analogRead(A0);
      int batteryLevel = map(battery, 0, 1023, 0, 100);
      Serial.print("Battery Level % is now: ");
      Serial.println(batteryLevel);
      batteryLevelChar.writeValue(batteryLevel);
      delay(200);
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());    
  }

}
