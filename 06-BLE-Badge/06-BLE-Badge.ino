
/*
  MakeZurich Badge
  Exposes all Arduino Nano 33 BLE Sense features, and the 11 LED neopixel ring.
  See web frontend `vuejs-webui`.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> // The library for 9-axis IMU, install "Arduino_LSM9DS1", docs https://www.arduino.cc/en/Reference/ArduinoLSM9DS1
#include <Arduino_LPS22HB.h> // The library to read Pressure, install "Arduino_LPS22HB", docs https://www.arduino.cc/en/Reference/ArduinoLPS22HB
#include <Arduino_HTS221.h> // The library to read Temperature and Humidity, install "Arduino_HTS221", docs https://www.arduino.cc/en/Reference/ArduinoHTS221
#include <Arduino_APDS9960.h> // The library for colour, proximity and gesture recognition, install "Arduino_APDS9960", docs https://www.arduino.cc/en/Reference/ArduinoAPDS9960
#include <Adafruit_NeoPixel.h> // The library for NeoPixels, docs https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html

#define DEBUG

#ifdef DEBUG
  #define log(x) Serial.print(x);
  #define logln(x) Serial.println(x);
#else
  #define log(x) x;
  #define logln(x) x;
#endif

// Different BLECharacteristic are documented here https://www.arduino.cc/en/Reference/ArduinoBLEBLECharacteristicBLECharacteristic

// Device info, this service UUID is also used to filter out the Badge.
BLEService deviceInfo("ea0669f3-fd44-406f-98e0-5feaeed5cb22");
BLECharacteristic deviceAddr("0D0F17BC-1528-48C0-B303-2267D900CF25", BLERead, 8); // 0FE5F4B4FAA60A5C 8bytes, random from the Arduino NANO 33 BLE Sense

// Environment Sensing Service
BLEService sensors("181A"); 
BLEShortCharacteristic currentTemperature("2A6E", BLERead); // int16, degrees Celsius with a resolution of 0.01 https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.temperature.xml
BLEUnsignedShortCharacteristic currentHumidity("2A6F", BLERead); // uint16, %, with a resolution of 0.01, https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.humidity.xml
// TODO: pressure should not be float but uint16 with 0.1 precision, Pa
BLEFloatCharacteristic pressure("2A6D", BLERead); // float, kilopascal, https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.pressure.xml

// imu
BLEService imuSensor("A4E649F4-4BE5-11E5-885D-FEFF819CDC9F");
BLEStringCharacteristic accelerometer("8E4B75FE-8148-4775-8D48-71B51DF62C46", BLERead, 64); // x,y,z
BLEStringCharacteristic gyroscope("8E4B75FE-8148-4775-8D48-71B51DF62C47", BLERead, 64); // x,y,z
BLEStringCharacteristic magnetometer("8E4B75FE-8148-4775-8D48-71B51DF62C48", BLERead, 64); // x,y,z

// adp
BLEService apdSensor("E2F101C1-8FA8-4BCB-80FC-6469F25ACDB6");
BLEUnsignedIntCharacteristic proximitry("1648A1AE-5A8D-4544-B3CF-6C497D73E771", BLERead); 
BLEStringCharacteristic color("1648A1AE-5A8D-4544-B3CF-6C497D73E772", BLERead, 64);  // String, r,g,b
BLEUnsignedIntCharacteristic lightIntensity("1648A1AE-5A8D-4544-B3CF-6C497D73E773", BLERead); // int for ambient light
BLEByteCharacteristic gesture("1648A1AE-5A8D-4544-B3CF-6C497D73E774", BLERead); // https://github.com/arduino-libraries/Arduino_APDS9960/blob/master/src/Arduino_APDS9960.h#L26

// leds
BLEService leds("3D05109A-7DE0-4146-9EE8-9283AC9E4B00");
BLEByteCharacteristic systemRGB("3D05109A-7DE0-4146-9EE8-9283AC9E4B14", BLERead | BLEWrite); // 0=OFF, 1=RED, 2=GREEN, 3=BLUE
BLEBoolCharacteristic systemLED("3D05109A-7DE0-4146-9EE8-9283AC9E4B12", BLERead | BLEWrite); // on/off
BLECharacteristic ledRing("3d05109a-7de0-4146-9ee8-9283ac9e4b15", BLERead | BLEWrite, 4); // bytes are: position, red, green, blue
BLEByteCharacteristic command("3d05109a-7de0-4146-9ee8-9283ac9e4b16", BLERead | BLEWrite); // 0=clear leds

// NeoPixel
#define LED_PIN 6
#define LED_COUNT 11
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentCommand = 0;

void setup(){
  #ifdef DEBUG
  Serial.begin(9600); //Serial monitor to display all sensor values 
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.printf("DeviceAddr %08X%08X\n", NRF_FICR->DEVICEADDR[1], NRF_FICR->DEVICEADDR[0]);
  #endif
  
  if (!IMU.begin()) { // Initialize IMU sensor 
   logln("Failed to initialize IMU!");
   while (1);
  }

  if (!BARO.begin()) { // Initialize Pressure Sensor 
   logln("Failed to initialize Pressure Sensor!");
   while (1);
  }

  if (!HTS.begin()) { // Initialize Temperature and Humidity sensor 
    logln("Failed to initialize Temperature and Humidity Sensor!");
    while (1);
  }

  if (!APDS.begin()) { // Initialize Colour, Proximity and Gesture sensor 
    logln("Failed to initialize Colour, Proximity and Gesture Sensor!");
    while (1);
  }

  strip.begin();
  strip.setBrightness(20);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'

  if (!BLE.begin()) {
    logln("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Nano Example 06");
  BLE.setDeviceName("Nano Example 06");
  BLE.setAdvertisedService(deviceInfo);
  
  BLEDescriptor deviceAddrDescriptor("2901", "Device Address");
  deviceAddr.addDescriptor(deviceAddrDescriptor);
  uint8_t ret[8];
  uint32_t adr = NRF_FICR->DEVICEADDR[1];
  ret[0] = ((adr >> 24) & 0xFF);
  ret[1] = ((adr >> 16) & 0xFF);
  ret[2] = ((adr >> 8) & 0xFF);
  ret[3] = (adr & 0xFF);
  adr = NRF_FICR->DEVICEADDR[0];
  ret[4] = ((adr >> 24) & 0xFF);
  ret[5] = ((adr >> 16) & 0xFF);
  ret[6] = ((adr >> 8) & 0xFF);
  ret[7] = (adr & 0xFF);
  deviceAddr.writeValue(ret, 8);
  deviceInfo.addCharacteristic(deviceAddr);
  BLE.addService(deviceInfo);

  currentTemperature.setEventHandler(BLERead, readTemperature);
  // readTemperature(BLE.central(), currentTemperature);
  BLEDescriptor tempDescriptor("2901", "Current Temperature");
  currentTemperature.addDescriptor(tempDescriptor);
  sensors.addCharacteristic(currentTemperature);
  currentHumidity.setEventHandler(BLERead, readHumidity);
  // readHumidity(BLE.central(), currentRelativeHumidity);
  sensors.addCharacteristic(currentHumidity);
  pressure.setEventHandler(BLERead, readPressure);
  // readPressure(BLE.central(), pressure);
  sensors.addCharacteristic(pressure);  
  BLE.addService(sensors);
  
  accelerometer.setEventHandler(BLERead, readAccelerometer);
  readAccelerometer(BLE.central(), accelerometer); // initialise the characteristics with values
  imuSensor.addCharacteristic(accelerometer);
  gyroscope.setEventHandler(BLERead, readGyroscope);
  readGyroscope(BLE.central(), gyroscope); // initialise the characteristics with values
  imuSensor.addCharacteristic(gyroscope);
  magnetometer.setEventHandler(BLERead, readMagnetometer);
  readMagnetometer(BLE.central(), magnetometer); // initialise the characteristics with values
  imuSensor.addCharacteristic(magnetometer);
  BLE.addService(imuSensor);

  proximitry.setEventHandler(BLERead, readProximitry);
  apdSensor.addCharacteristic(proximitry);
  color.setEventHandler(BLERead, readColor);
  readColor(BLE.central(), color); // initialize characteristics with values
  apdSensor.addCharacteristic(color);
  lightIntensity.setEventHandler(BLERead, readColor);
  apdSensor.addCharacteristic(lightIntensity);
  gesture.setEventHandler(BLERead, readGesture);
  apdSensor.addCharacteristic(gesture);
  BLE.addService(apdSensor);
  
  systemRGB.setEventHandler(BLEWritten, systemRGBCharacteristicWritten);
  systemRGB.writeValue(0);
  leds.addCharacteristic(systemRGB);
  pinMode(LEDR, OUTPUT); // pin 22
  pinMode(LEDG, OUTPUT); // pin 23
  pinMode(LEDB, OUTPUT); // pin 24
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  
  systemLED.setEventHandler(BLEWritten, systemLEDCharacteristicWritten);
  systemLED.writeValue(0);
  leds.addCharacteristic(systemLED);
  pinMode(LED_BUILTIN, OUTPUT);

  ledRing.setEventHandler(BLEWritten, ledRingWritten);
  uint8_t def[4];
  ledRing.writeValue(def, 4);
  leds.addCharacteristic(ledRing);

  command.setEventHandler(BLEWritten, commandWritten);
  command.writeValue(0);
  leds.addCharacteristic(command);
  
  BLE.addService(leds);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  BLE.advertise();
  logln(("Bluetooth device active, waiting for connections..."));
 }

void loop() {
  BLE.poll();
  if (currentCommand == 1) {
    rainbow();
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  log("Connected event, central: "); logln(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  log("Disconnected event, central: "); logln(central.address());
}

void ledRingWritten(BLEDevice central, BLECharacteristic characteristic) {
  const uint8_t* arr = ledRing.value();
  if (arr[0] >= LED_COUNT) {
    logln("LED does not exist");
    return;
  }

  currentCommand = 0; // stop running animations
  Serial.printf("%02X:%02X:%02X:%02X", arr[0], arr[1], arr[2], arr[3]);
  strip.setPixelColor(arr[0], arr[1], arr[2], arr[3]);
  strip.show();
}

void commandWritten(BLEDevice central, BLECharacteristic characteristic) {
  int commandNumber = command.value();
  currentCommand = commandNumber;
  if (commandNumber == 0) {
    strip.clear();
    strip.show();
  }
}

void systemLEDCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  if (systemLED.value()) {
    logln("LED on");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    logln("LED off");
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void systemRGBCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t v = systemRGB.value();
  log("Set RGB to:"); logln(v);
  // The system RGB is super weird, it takes inverted values, analogwrite does not allow to turn it off, so we just use digitalwrite.
  digitalWrite(LEDR, v != 1);
  digitalWrite(LEDG, v != 2);
  digitalWrite(LEDB, v != 3);
}

// TODO: use parameter instead of global
void readTemperature(BLEDevice central, BLECharacteristic characteristic) {
  float temperature = HTS.readTemperature();
  log("Temperature = "); log(temperature);
  short t = convertFloatToShort(temperature);
  currentTemperature.writeValue(t);
}

void readHumidity(BLEDevice central, BLECharacteristic characteristic) {
  float humidity = HTS.readHumidity();
  log("Humidity = "); logln(humidity);
  short t = convertFloatToShort(humidity);
  currentHumidity.writeValue(t);
}

void readPressure(BLEDevice central, BLECharacteristic characteristic) {
  float p = BARO.readPressure(); // Read Pressure value, in KILOPASCAL
  log("Pressure = "); logln(p);
  pressure.writeValue(p);
}

void readProximitry(BLEDevice central, BLECharacteristic characteristic) {
  if (APDS.proximityAvailable()) {
    int p = APDS.readProximity();
    log("Proximity = "); logln(p);
    proximitry.writeValue(p);
  }
}

void readColor(BLEDevice central, BLECharacteristic characteristic) {
  int tries = 50;
  while(APDS.colorAvailable() == 0 && tries > 0) {
    tries--;
    delay(30);
  }
  int r, g, b, c;
  APDS.readColor(r, g, b, c); // read the color and clear light intensity
  char buf[32];
  sprintf(buf, "%f,%f,%f", r, g, b);
  color.writeValue(String(buf));
  lightIntensity.writeValue(c);
  log("Color = "); logln(buf);
  log("Light Intensity = "); logln(c);
}


void readGesture(BLEDevice central, BLECharacteristic characteristic) {
  // TODO: Find out how to correctly wait for gestures
  int tries = 50;
  while(APDS.gestureAvailable() == 0 && tries > 0) {
    tries--;
    delay(30);
  }
  int res = APDS.readGesture();
  gesture.writeValue(res);
  log("Gesture = "); logln(res);
}

void readAccelerometer(BLEDevice central, BLECharacteristic characteristic) {
  float accel_x, accel_y, accel_z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    log("Accelerometer = "); log(accel_x); log(", "); log(accel_y); log(", "); logln(accel_z);
    char buf[32];
    sprintf(buf, "%f,%f,%f", accel_x, accel_y, accel_z);
    accelerometer.writeValue(String(buf));
  }
}

void readGyroscope(BLEDevice central, BLECharacteristic characteristic) {
  float gyro_x, gyro_y, gyro_z;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyro_x, gyro_y, gyro_z);
    log("Gyroscope = "); log(gyro_x); log(", "); log(gyro_y);log(", "); logln(gyro_z);
    char buf[32];
    sprintf(buf, "%f,%f,%f", gyro_x, gyro_y, gyro_z);
    gyroscope.writeValue(String(buf));
  }
}

void readMagnetometer(BLEDevice central, BLECharacteristic characteristic) {
  float mag_x, mag_y, mag_z;
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mag_x, mag_y, mag_z);
    log("Magnetometer = "); log(mag_x); log(", "); log(mag_y); log(", "); logln(mag_z);
    char buf[32];
    sprintf(buf, "%f,%f,%f", mag_x, mag_y, mag_z);
    magnetometer.writeValue(String(buf));
  }
}

void rainbow() {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(15);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
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
