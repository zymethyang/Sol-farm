#include <SPI.h>
#include <LoRa.h>
#include <ThreadController.h>
#include <SHT21.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "LocalState.cpp"
#include "Node01State.cpp"

ThreadController controller = ThreadController();
Thread sync_data_thread = Thread();
Thread get_sht21_thread = Thread();
Thread output_dac_thread = Thread();
Adafruit_MCP4725 dac;
SHT21 sht;

LocalState* localState = new LocalState();
Node01State* node01State = new Node01State();


const int csPin = 10;
const int resetPin = 5;
const int irqPin = 2;

void setup() {
  Wire.begin();
  dac.begin(0x62);
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
  output_dac_thread.onRun(outputDAC);
  output_dac_thread.setInterval(2000);
  controller.add(&sync_data_thread);
  controller.add(&get_sht21_thread);
  controller.add(&output_dac_thread);
}

void loop() {
  controller.run();
}

void syncData() {
  String dataState = localState->getState();
  sendData(dataState);
  Serial.println(dataState);
}

void getSHT21() {
  //float temp = sht.getTemperature();  // get temp from SHT
  //float humidity = sht.getHumidity(); // get temp from SHT
  //localState->setTempSHT21(sht.getTemperature());
  //localState->setHumSHT21(sht.getHumidity());
  float tempSHT21 = random(100, 2000) / 100.0;
  float humSHT21 = random(100, 2000) / 100.0;
  localState->setTempSHT21(tempSHT21);
  localState->setHumSHT21(humSHT21);
}

void onReceive(int packetSize) {
  char messageLora[packetSize + 1];
  for (int i = 0; i < packetSize; i++) {
    messageLora[i] = (char)LoRa.read();
  }
  messageLora[packetSize] = '\0';
  Serial.println(messageLora);
  node01State->updateState(messageLora);
  localState->updateState(messageLora);
}

void sendData(String anything) {
  if (LoRa.beginPacket() != 0) {
    LoRa.beginPacket();
    LoRa.print(anything);
    LoRa.endPacket();
    LoRa.receive();
  }
}

void outputDAC() {
  int dacValue = localState->getDacValue();
  dac.setVoltage(dacValue, false);
  Serial.println("Output DAC Value");
}
