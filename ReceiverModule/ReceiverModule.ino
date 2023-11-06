#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <EEPROM.h>

#define HID_CUSTOM_LAYOUT

// LED Control
int REDPIN = 3;
int GREENPIN = 5;
int BLUEPIN = 6;
int ledIncr = false;
int ledStep = 20;

// RADIO Control
int CE = 9;
int CSN = 10;
RF24 radio(CE, CSN);
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Function Control
bool isConnected = false;
bool isMotion = false;
bool enabledMotion = false;
bool eepromInitialized = false;
bool lastLEDstate = false;

// Voltage Control
const int arraySize = 2;
int Array[arraySize];
int deadVoltageAddress = 0;
int lastVoltageAddress = 2;
int deadVoltage;
int voltage;
int lastVoltage;
const int warningThreshold = 70;

int tries = 0;
int maxtries = 100;

void setup() {
  
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  radio.setPALevel(RF24_PA_MIN);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (!eepromInitialized){
    Serial.println(eepromInitialized);
    deadVoltage = EEPROM.read(deadVoltageAddress) * 4;
    voltage = EEPROM.read(lastVoltageAddress) * 4;
    lastVoltage = EEPROM.read(lastVoltageAddress) * 4;
    Serial.println("EEPROM Variables Initialized.");
    eepromInitialized = true;
  }
  if (isConnected == false){
    flashRedConnection(isConnected);
  }
  while (radio.available()){
    tries = 0;
    isConnected = true;
    if (lastLEDstate == false){
      flashRedConnection(isConnected);
      lastLEDstate = true;
    }
    radio.read(&Array, sizeof(Array)); // Read Info from Sender
    if (Array[0] == 1){isMotion = true;} // Turning isMotion from Array[0] into a boolean from an int.
    else {isMotion = false;}
    voltage = Array[1]; // Turning voltage from Array[1] into a float from a 3 digit int.
    if (isMotion){
      if (!enabledMotion){ // In case motion has not been enabled yet, to prevent it from hapening twice.
        Serial.println("MOTION");
        enabledMotion = true;
        doLedTriggered();
      }
    } else if (enabledMotion){ // isMotion is false, but previously motion detected.
      Serial.println("No Motion");
      enabledMotion = false;
    }

    Serial.print("Voltage: ");
    Serial.println(voltage);
    Serial.print("DeadVoltage: ");
    Serial.println(deadVoltage);
    if (voltage <= deadVoltage + warningThreshold){
      doLedSteps();
    } else {
      RGB_color (255, 255, 255);
    }
    delay(10);
  } 
  delay(10);
  tries += 1;
  if (isConnected && tries >= maxtries){
    Serial.println("Lost Connection to Sender");
    isConnected = false;
    lastLEDstate = false;
    updateDeadVoltage(voltage > lastVoltage + 20);
    //eepromInitialized = false;
    tries = 0;
  }
  //delay(100);
}

// if the voltage is less than the dead voltage, then set the dead voltage to the current voltage.
// if the voltage is greater than 7 and the previous voltage differs form the dead voltage, set the dead voltage to the previous voltage.
//    this determines when a different kind of battery is replaced.
void updateDeadVoltage(bool batteryChanged){
  EEPROM.update(lastVoltageAddress, voltage / 4);
  Serial.print("DeadVoltage: ");
  Serial.println(deadVoltage);
  Serial.print("lastVoltage: ");
  Serial.println(lastVoltage);
  Serial.print("voltage: ");
  Serial.println(voltage);
  if (voltage < deadVoltage){
    deadVoltage = voltage;
    EEPROM.update(deadVoltageAddress, deadVoltage / 4);
    Serial.print("Updated Dead Voltage to DeadVoltage: ");
    Serial.println(deadVoltage);
  } 
  if (batteryChanged && lastVoltage > deadVoltage + warningThreshold + 10){
    deadVoltage = lastVoltage;
    EEPROM.update(deadVoltageAddress, deadVoltage / 4);
    Serial.print("Updated Dead Voltage to lastVoltage: ");
    Serial.println(deadVoltage);
  }
}

void RGB_color(int redval, int greenval, int blueval){
  analogWrite(REDPIN, redval);
  analogWrite(GREENPIN, greenval);
  analogWrite(BLUEPIN, blueval);
}

void flashRedConnection(bool isConnected){
  if (isConnected){
    RGB_color (20, 20, 20);
  } else {
    RGB_color(255, 0, 0);
    delay(200);
    RGB_color(0,0,0);
    delay(200);
  }
}

void doLedSteps(){
  if (ledStep >= 255){ledIncr = false;} 
  if (ledStep <= 0){ledIncr = true;}
  RGB_color(ledStep, ledStep * 0.25, 0);
  if (ledIncr){ledStep += 10;}
  else {ledStep -= 10;}
}

void doLedTriggered(){
  for (int a = 0; a < 10; a++){
    RGB_color(0, 255, 0);
    delay(100);
    RGB_color(0, 0, 0);
    delay(100);
  }
}

  
