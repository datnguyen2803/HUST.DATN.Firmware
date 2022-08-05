#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, 1);
  Serial.println("LED on");
  delay(1000);
  digitalWrite(LED_BUILTIN, 0);
  Serial.println("LED off");
  delay(1000);
}
