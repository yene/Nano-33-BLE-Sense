// from getting started guide https://www.arduino.cc/en/Guide/NANO33BLESense
// 1. Install Arduino Desktop IDE
// 2. Tools > Board... > Board Manager..
// 3. Install  Arduino nRF528x NANO 33 BLE board
// 4. Select the correct Board under Tool > Board and Port under Tool > Port
// 5. upload

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");
  delay(2000);
}
