/*
  Blink & RGB Loop

  Turns orange LED on for one second, then off for one second, repeatedly.
  Most Arduinos have an on-board LED you can control. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.

  Inside the built-in RGB LED, there are three more LED’s. With analogWrite we can change the brightness of these LED’s.
  Important: The built-in RGB LED brightness values are reversed, 255 is off, 0 is max brightness.

  Challenges for the learner:
  * loop over all possible colors

*/

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize digital pin of the built-in RGB LED as an output.
  pinMode(LEDR, OUTPUT); // pin 22
  pinMode(LEDG, OUTPUT); // pin 23
  pinMode(LEDB, OUTPUT); // pin 24
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  // Setting the RGB to red, half brightness
  analogWrite(LEDR, 127);
  analogWrite(LEDG, 255);
  analogWrite(LEDB, 255);
}
