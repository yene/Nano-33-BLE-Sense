# Arduino NANO 33 BLE Sense

![Hardware Overview](material/Arduino-Nano-33-BLE-Sense-Hardware-Overview.jpg)

## Notes
* Install the board Arduino nRF528x and select the Arduino NANO 33 BLE
* Most libraries you install in the Arduino IDE come with examples.
* NANO 33 BLE is a 3.3V board.
* The 5V pin does not work, see the docs.

## FAQ
* Can it join Wi-Fi? No, it only does Bluetooth.
* Can you store data? No, the bootloader prevents access to the flash memory.
* Can it be used with micro/cicruit python?
* Is the word detection any good? It is not really accurate, usually its only one serialized spectrogram. I would recommend to use a Raspberry Pi.
* Is the built-in RGB LED analog (PWM)? No, it is only digital.
* Where is documented that the RGB pins are 22,23,24?
* Can the NANO be put to deep sleep? 

## Pinout
![Pinout](material/pinout.png)


## Links
* The most important link is the offical [Getting started with the Arduino NANO 33 BLE Sense](https://www.arduino.cc/en/Guide/NANO33BLESense)
* [Bluetooth examples](https://github.com/arduino-libraries/ArduinoBLE/tree/master/examples)
* https://github.com/arduino-libraries/Arduino_ScienceJournal/blob/master/examples/Nano33BLESenseFirmware/Nano33BLESenseFirmware.ino
* [DaleGia Nano33BLESensor](https://github.com/DaleGia/Nano33BLESensor)
* [Arduino Science Journal](https://sj.arduino.cc)


## Advanced Projects
* [Detecting Dog Tricks with ML](https://create.arduino.cc/projecthub/whatsupdog/the-muttmentor-9d9753?ref=part&ref_id=107215&offset=2)
* [Send data over sound](https://create.arduino.cc/projecthub/ChirpDevs/send-data-with-sound-bf7024?ref=part&ref_id=107215&offset=3)
* [Magic Wand](https://create.arduino.cc/projecthub/andri/ai-powered-magic-wand-ab1c90?ref=part&ref_id=107215&offset=5)
* [Classify Candy in Free Fall Using TinyML](https://create.arduino.cc/projecthub/8bitkick/classify-candy-in-free-fall-using-tinyml-2836bf)


