#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>

SoftwareSerial serialSW(4, 3);

const int csPin = 10;
const int resetPin = 5;
const int irqPin = 2;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  serialSW.begin(9600);

  LoRa.setPins(csPin, resetPin, irqPin);
  Serial.println("Setup Lora....");
  if (!LoRa.begin(525E6)) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Complete setup lora");

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  //Nothing here.
}


void onReceive(int packetSize) {
  char messageLora[packetSize + 1];
  for (int i = 0; i < packetSize; i++) {
    messageLora[i] = (char)LoRa.read();
  }
  messageLora[packetSize] = '\0';
  serialSW.println(messageLora);
  Serial.println(messageLora);
}

void sendData(String anything) {
  if (LoRa.beginPacket() != 0) {
    LoRa.beginPacket();
    LoRa.print(anything);
    LoRa.endPacket();
    LoRa.receive();
  }
}
