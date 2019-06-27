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

char WIFI_SSID[] = "Temp1";
char WIFI_PASSWORD[] = "qwertyuiop";
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

  LoRa.onReceive(onReceive);
  LoRa.receive();

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
}

void loop() {
  controller.run();
}

void onReceive(int packetSize) {
  //Lora callback function
  String loraBuffer = "";
  for (int i = 0; i < packetSize; i++) {
    loraBuffer += (char)LoRa.read();
  }
  //Serial.println("Having new Lora data");
  node01State->updateState(loraBuffer);
  //node02State->updateState(loraBuffer);
}


void onReceiveMqtt (char *topicName, int payloadLen, char *payLoad)
{
  Serial.println(payLoad);
}

void sendMqttMessage () {
  String node01Buffer = node01State->getState();
  String node02Buffer = node02State->getState();
  String localBuffer = localState->getState();
  //Serial.println(localBuffer);
  //Serial.println(node01Buffer);
  //Serial.println(node02Buffer);

  StaticJsonBuffer<128> jb;
  JsonObject& obj = jb.createObject();

  String s = "";
  char msg[128];
  obj["temp"] = random(40);
  obj["hum"] = random(100);
  obj["sensor_id"] = "sensor01";

  obj.printTo(s);
  s.toCharArray(msg, 128);

  if (aws.publish("sol-farm/feedback", msg) == 0)
  {
    Serial.print("Published Message");
  }
  else
  {
    Serial.println("Publish failed");
  }
  //obj.printTo(Serial);
  Serial.println("");
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
