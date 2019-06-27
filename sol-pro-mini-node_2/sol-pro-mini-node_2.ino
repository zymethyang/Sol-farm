#include <SPI.h>
#include <LoRa.h>
#include <ThreadController.h>
#include "LocalState.cpp"
#include "Node01State.cpp"

ThreadController controller = ThreadController();
Thread sync_data_thread = Thread();
Thread get_sht21_thread = Thread();

LocalState* localState = new LocalState();
Node01State* node01State = new Node01State();


const int csPin = 10;
const int resetPin = 5;
const int irqPin = 2;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  Serial.println("Setup Lora....");
  if (!LoRa.begin(525E6)) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Complete setup lora");

  LoRa.onReceive(onReceive);
  LoRa.receive();

  sync_data_thread.onRun(syncData);
  sync_data_thread.setInterval(2000);
  get_sht21_thread.onRun(getSHT21);
  get_sht21_thread.setInterval(1800);
  controller.add(&sync_data_thread);
  controller.add(&get_sht21_thread);
}

void loop() {
  controller.run();
}

void syncData() {
  String dataState = localState->getState();
  sendData(dataState);
}

void getSHT21() {
  //localState->setTempSHT21(sht.getTemperature());
  //localState->setHumSHT21(sht.getHumidity());
  float tempSHT21 = float(random(28, 30));
  float humSHT21 = float(random(60, 80));
  localState->setTempSHT21(tempSHT21);
  localState->setHumSHT21(humSHT21);
}

void onReceive(int packetSize) {
  String loraBuffer = "";
  for (int i = 0; i < packetSize; i++) {
    loraBuffer += (char)LoRa.read();
  }
  node01State->updateState(loraBuffer);
  localState->updateState(loraBuffer);
}

void sendData(String anything) {
  while (LoRa.beginPacket() == 0) {
    Serial.print("waiting for radio ready... ");
    delay(100);
  }
  // send in async / non-blocking mode
  LoRa.beginPacket();
  LoRa.print(anything);
  LoRa.endPacket(true); //true is async mode
}
