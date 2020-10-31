// inspiration https://www.circleelectronic.com/ledring
// https://learn.adafruit.com/pixel-chase-game/coding-the-pixel-chase-game

// color blue rouns in circle, hit when it is on top (changes to green)
// on win you get wheel
// on lose you get red

#include <Adafruit_NeoPixel.h> // The library for NeoPixels, docs https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html
#include <Arduino_APDS9960.h> // The library for colour, proximity and gesture recognition, install "Arduino_APDS9960", docs https://www.arduino.cc/en/Reference/ArduinoAPDS9960


#define LED_PIN D4
#define LED_COUNT 11
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int colorRed = strip.Color(64, 0, 0);
int colorGreen = strip.Color(0, 64, 0);
int colorBlue = strip.Color(0, 0, 64);

int choosenPin = 0;

//#define DEBUG

#ifdef DEBUG
  #define log(x) Serial.print(x);
  #define logln(x) Serial.println(x);
#else
  #define log(x) x;
  #define logln(x) x;
#endif

void setup() {
  #ifdef DEBUG
  Serial.begin(9600); //Serial monitor to display all sensor values 
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.printf("DeviceAddr %08X%08X\n", (unsigned int)NRF_FICR->DEVICEADDR[1], (unsigned int)NRF_FICR->DEVICEADDR[0]);
  #endif

  if (!APDS.begin()) { // Initialize Colour, Proximity and Gesture sensor 
    while (1);
  }

  strip.begin();
  strip.setBrightness(20);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
  resetGame();
}

void loop() {
  ledaction();
}

void didWin() {
  for (int i=0; i< 2; i++) {
    rainbow();
    delay(500);
  }
  
}

void didLose() {
  blinkColor(colorRed, 4);
}

void resetGame() {
  choosenPin = random(LED_COUNT);
}
 
void ledaction() {
  for (int i = 0; i<=LED_COUNT; i++) {
    strip.clear();
    strip.setPixelColor(i, colorBlue);
    if (i==choosenPin) {
      strip.setPixelColor(i, colorGreen);
    }
    strip.show();
    delay(250);
    if (APDS.proximityAvailable()) {
      int p = APDS.readProximity();
      logln(p);
      if (p < 100) {
        if (i==choosenPin) {
          didWin();
        } else {
          didLose();
        }
        resetGame();
      }
    }
    
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

void blinkColor(int color, int times) {
  for (int j=0; j<times; j++) {
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(500);
    strip.clear();
    strip.show();
    delay(500);
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
