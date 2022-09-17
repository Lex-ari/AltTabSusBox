

int PIR = 2; // the pin that the PIR sensor is attached

#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

int CE = 9;
int CSN = 10;
RF24 radio (CE, CSN);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const int arraySize = 2;
int Array[arraySize];

void setup() {
  delay(5000);
  pinMode(PIR, INPUT);

  radio.begin();
  radio.openWritingPipe(pipe);
  radio.stopListening();
  radio.setPALevel(RF24_PA_MIN);
  pinMode(14, INPUT);
  Serial.begin(9600);
  digitalWrite(PIR, LOW);
}

void loop() {
  int sensorValue = analogRead(14);
  Serial.println(sensorValue);
  if (digitalRead(PIR) == HIGH ) {
    //Serial.println("Motion Detected");
    Array[0] = 1;
  }
  else {
    //Serial.println("No Motion");
    Array[0] = 0;
  }
  Array[1] = sensorValue;
  radio.write(&Array, sizeof(Array));
}
