// Temperature & Humidity Sensor

#include <ArduinoBLE.h> // The library to work with Bluetooth, docs https://www.arduino.cc/en/Reference/ArduinoBLE
#include <Arduino_HTS221.h> // The library to read Temperature and Humidity, install "Arduino_HTS221", docs https://www.arduino.cc/en/Reference/ArduinoHTS221

BLEService sensors("181A"); // Environment Sensing Service, docs https://www.bluetooth.com/specifications/gatt/services/
// NOTE: It's a short with implied exponent of 10^-2. For example a value of 2321 = 23.21 degrees Celsiusa
BLEShortCharacteristic currentTemperature("2A6E", BLERead); // short (sint16), degrees Celsius with a resolution of 0.01 https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.temperature.xml

BLEUnsignedShortCharacteristic currentHumidity("2A6F", BLERead); // uint16, %, with a resolution of 0.01, https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.humidity.xml

void setup() {
  Serial.begin(9600); // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  if (!HTS.begin()) { // Initialize Temperature and Humidity sensor 
    Serial.println("Failed to initialize Temperature and Humidity Sensor!");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setDeviceName("");
  BLE.setLocalName("Temperature & Humidity Sensor");
  
  BLE.setAdvertisedService(sensors);
  sensors.addCharacteristic(currentTemperature);
  currentTemperature.setEventHandler(BLERead, readTemperature);
  readTemperature(BLE.central(), currentTemperature);
  currentHumidity.setEventHandler(BLERead, readHumidity);
  readHumidity(BLE.central(), currentHumidity);
  sensors.addCharacteristic(currentHumidity);
  BLE.addService(sensors);

  /* Start advertising BLE. It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */
  
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Connected event, central: "); Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected event, central: "); Serial.println(central.address());
}

void readTemperature(BLEDevice central, BLECharacteristic characteristic) {
  float temperature = HTS.readTemperature();
  short t = convertFloatToShort(temperature);
  currentTemperature.writeValue(t);
  Serial.print("Temperature = "); Serial.println(temperature);
}

void readHumidity(BLEDevice central, BLECharacteristic characteristic) {
  float humidity = HTS.readHumidity();
  short t = convertFloatToShort(humidity);
  currentHumidity.writeValue(humidity);
  Serial.print("Humidity = "); Serial.println(humidity);
}


short convertFloatToShort(float x) {
  x = x * 100;
  if (x < -32768) {
    return -32768;
  }
  if (x > 32767) {
    return 32767;
  }
  return (short)round(x);
}
