#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <AWS_IOT.h>
#include <ThreadController.h>
#include <ArduinoJson.h>
#include "LocalState.cpp"
#include "Node01State.cpp"
#include "Node02State.cpp"
#include <AWS_IOT.h>


ThreadController controller = ThreadController();
Thread publish_data_thread = Thread();
LocalState* localState = new LocalState();
Node01State* node01State = new Node01State();
Node02State* node02State = new Node02State();

AWS_IOT aws;
void onReceive(int packetSize);
void onReceiveMqtt (char *topicName, int payloadLen, char *payLoad);
char WIFI_SSID[] = "Temp1";
char WIFI_PASSWORD[] = "qwertyuiop";
//char WIFI_SSID[] = "Yen Huong";
//char WIFI_PASSWORD[] = "yenhuong1954";
char HOST_ADDRESS[] = "a2184o3gtkvd1o-ats.iot.ap-southeast-1.amazonaws.com";
char CLIENT_ID[] = "sol-farm";
char TOPIC_NAME[] = "sol-farm/control";

const int csPin = 5;
const int resetPin = 14;
const int irqPin = 2;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.println("Begin setup serial....");
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Begin setup Lora....");
  LoRa.setPins(csPin, resetPin, irqPin);
  Serial.println("Setup Lora....");
  if (!LoRa.begin(525E6)) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Complete setup lora");


  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }

  Serial.println("Connected to wifi");

  if (aws.connect(HOST_ADDRESS, CLIENT_ID) == 0)
  {
    Serial.println("Connected to AWS");
    delay(1000);

    if (0 == aws.subscribe(TOPIC_NAME, onReceiveMqtt))
    {
      Serial.println("Subscribe Successfull");
    }
    else
    {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while (1);
    }
  }
  else
  {
    Serial.println("AWS connection failed, Check the HOST Address");
    while (1);
  }


  Serial.println("Push job to task queue");

  publish_data_thread.onRun(sendMqttMessage);
  publish_data_thread.setInterval(2000);
  controller.add(&publish_data_thread);
  LoRa.onReceive(onReceive);
  LoRa.receive();

}

void loop() {
  controller.run();
}

void onReceive(int packetSize) {
  //Lora callback function
  char msgToSave[packetSize + 1];
  char messageLora[packetSize + 1];
  for (int i = 0; i < packetSize; i++)
  {
    messageLora[i] = (char)LoRa.read();
    msgToSave[i] = messageLora[i];
  }
  messageLora[packetSize] = '\0';
  msgToSave[packetSize] = '\0';
  Serial.println(messageLora);
  parseJSONRecived(messageLora);
  //  node02State->updateState(messageLora,);
  //  node01State->updateState(messageLora);
}

void parseJSONRecived(char* messageLora) {
  DynamicJsonBuffer jb;
  JsonObject& doc  = jb.parseObject(messageLora);
  if (doc.success()) {
    if (doc.containsKey("nodeID")) {
      String id = doc["nodeID"].as<String>();

      if (id == "node01") {
        Serial.println("processs 1");
        node01State->setTempSHT21(doc["tempSHT21"].as<float>());
        node01State->setHumSHT21(doc["humSHT21"].as<float>());
        node01State->setDs18b20(doc["ds18b20"].as<float>());
        //        node01State->buffString = messageLora;
        Serial.println(doc["tempSHT21"].as<float>());
        Serial.println(doc["humSHT21"].as<float>());
        Serial.println(doc["ds18b20"].as<float>());
      }
      if (id == "node02") {
        Serial.println("processs 2");
        node02State->setTempSHT21(doc["tempSHT21"].as<float>());
        node02State->setHumSHT21(doc["humSHT21"].as<float>());
        node02State->setDacValue(doc["ds18b20"].as<int>());
        Serial.println(doc["tempSHT21"].as<float>());
        Serial.println(doc["humSHT21"].as<float>());
        Serial.println(doc["dacValue"].as<int>());
      }
    }
  } else Serial.println("Parse JSON false");
}

void onReceiveMqtt (char *topicName, int payloadLen, char *payLoad)
{
  // Code nhan dieu khien tu mqtt
  char message[payloadLen + 1];
  for (int i = 0; i < payloadLen; i++) {
    message[i] = (char)payLoad[i];
  }
  message[payloadLen] = '\0';
  Serial.println(message);
  DynamicJsonBuffer jb;
  JsonObject& doc  = jb.parseObject(message);
  if (doc.success()) {
    if (doc.containsKey("message")) {
      Serial.println("Đã parse thành công");
    }
  } else Serial.println("Parse JSON false");

}

void sendMqttMessage () {
  //char arrToSend[3] = {node01State->getState(), node02State->getState(), localState->getState()};
  //========= NODE 1
  String msgSend[3] = {node01State->getState(), node02State->getState(), localState->getState()};
  for (int i = 0; i < 3; i++) {
    converToSend(msgSend[i]);
  }
  //=========
}
void converToSend(String msgSend) {
  char msg[128];
  msgSend.toCharArray(msg, 128);
  if (aws.publish("sol-farm/feedback", msg) == 0)
  {
    Serial.println("Published Message");
  }
  else
  {
    Serial.println("Publish failed");
  }
}
void sendDataLora(String anything) {
  while (LoRa.beginPacket() == 0) {
    Serial.print("waiting for radio ready... ");
    delay(100);
  }
  // send in async / non-blocking mode
  LoRa.beginPacket();
  LoRa.print(anything);
  LoRa.endPacket(true); //true is async mode

}
