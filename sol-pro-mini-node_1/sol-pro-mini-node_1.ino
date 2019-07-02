#include <SPI.h>
#include <LoRa.h>
#include <ThreadController.h>
#include <SHT21.h>
#include "LocalState.cpp"

ThreadController controller = ThreadController();
Thread sync_data_thread = Thread();
Thread get_sht21_thread = Thread();
Thread get_ds1820b_thread = Thread();
SHT21 sht; 

LocalState* localState = new LocalState();

const int csPin = 10;
const int resetPin = 5;
const int irqPin = 2;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  Serial.println("Setup Lora....");
  if (!LoRa.begin(525E6)) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Complete setup lora");

  sync_data_thread.onRun(syncData);
  sync_data_thread.setInterval(3000);
  get_sht21_thread.onRun(getSHT21);
  get_sht21_thread.setInterval(2800);
  get_ds1820b_thread.onRun(getDs18b20);
  get_ds1820b_thread.setInterval(2800);

  controller.add(&sync_data_thread);
  controller.add(&get_sht21_thread);
  controller.add(&get_ds1820b_thread);
}

void loop() {
  controller.run();
}

void syncData() {
  String dataState = localState->getState();
  Serial.println(dataState);
  sendData(dataState);
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

void getSHT21() {
  //float temp = sht.getTemperature();   
  //float humidity = sht.getHumidity(); 
  //localState->setTempSHT21(sht.getTemperature());
  //localState->setHumSHT21(sht.getHumidity());
  float tempSHT21 =random(100, 2000) / 100.0;
  float humSHT21 = random(100, 2000) / 100.0;
  localState->setTempSHT21(tempSHT21);
  localState->setHumSHT21(humSHT21);
}

void getDs18b20() {
  float ds18b20 = random(100, 2000) / 100.0;
  localState->setDs18b20(ds18b20);
}
