#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ThreadController.h>
#include <AsyncDelay.h>
#include <ArduinoJson.h>
#include "LocalState.cpp"
#include "Node01State.cpp"
#include "Node02State.cpp"

const char* ssid = "Temp1";
const char* password = "qwertyuiop";
const char* publish_topic = "sol-farm/feedback";
const char* subcribe_topic = "sol-farm/control";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
SoftwareSerial serialSW(D3, D4);
AsyncDelay delay_on_k1;
AsyncDelay delay_off_k1;
AsyncDelay delay_on_k2;
AsyncDelay delay_off_k2;
AsyncDelay delay_on_k3;
AsyncDelay delay_off_k3;

ThreadController controller = ThreadController();
Thread publish_data_thread = Thread();
Thread output_contactor_thread = Thread();
Thread caculate_k1_circle_thread = Thread();
Thread caculate_k2_circle_thread = Thread();
Thread caculate_k3_circle_thread = Thread();

LocalState* localState = new LocalState();
Node01State* node01State = new Node01State();
Node02State* node02State = new Node02State();

const char* AWS_endpoint = "a2184o3gtkvd1o-ats.iot.ap-southeast-1.amazonaws.com"; //MQTT broker ip


void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  String loraSend = "";

  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  StaticJsonDocument<128> doc;
  deserializeJson(doc, message);

  String nodeID = doc["nodeID"].as<String>();
  if (nodeID == "esp32") {
    if (doc.containsKey("esc")) {
      int esc = doc["esc"].as<int>();
      localState->setESC(esc);
    }
    if (doc.containsKey("k1on")) {
      int k1on = doc["k1on"].as<int>();
      localState->setK1On(k1on);
    }
    if (doc.containsKey("k2on")) {
      int k2on = doc["k2on"].as<int>();
      localState->setK2On(k2on);
    }
    if (doc.containsKey("k3on")) {
      int k3on = doc["k3on"].as<int>();
      localState->setK3On(k3on);
    }
    if (doc.containsKey("k1off")) {
      int k1off = doc["k1off"].as<int>();
      localState->setK1Off(k1off);
    }
    if (doc.containsKey("k2off")) {
      int k2off = doc["k2off"].as<int>();
      localState->setK2Off(k2off);
    }
    if (doc.containsKey("k3off")) {
      int k3off = doc["k3off"].as<int>();
      localState->setK3Off(k3off);
    }
  } else {
    serializeJson(doc, loraSend);
    sendDataLora(loraSend);
  }
}

WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callbackMQTT, espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  espClient.setBufferSizes(512, 512);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());
}



void setup() {
  Serial.begin(9600);
  while (!Serial);
  serialSW.begin(9600);
  Serial.setDebugOutput(true);
  setup_wifi();
  delay(1000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  }
  else {
    Serial.println("Success to open cert file");
  }

  delay(1000);

  if (espClient.loadCertificate(cert)) {
    Serial.println("cert loaded");
  }
  else {
    Serial.println("cert not loaded");
  }


  File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  }
  else {
    Serial.println("Success to open private cert file");
  }

  delay(1000);

  if (espClient.loadPrivateKey(private_key)) {
    Serial.println("private key loaded");
  }
  else {
    Serial.println("private key not loaded");
  }

  // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  }
  else {
    Serial.println("Success to open ca");
  }

  delay(1000);

  if (espClient.loadCACert(ca)) {
    Serial.println("ca loaded");
  }
  else {
    Serial.println("ca failed");
  }

  Serial.println("Push job to task queue");
  publish_data_thread.onRun(sendMqttMessage);
  publish_data_thread.setInterval(2000);
  output_contactor_thread.onRun(outputContactor);
  output_contactor_thread.setInterval(2000);

  caculate_k1_circle_thread.onRun(caculateK1Circle);
  caculate_k1_circle_thread.setInterval(500);
  caculate_k2_circle_thread.onRun(caculateK2Circle);
  caculate_k2_circle_thread.setInterval(500);
  caculate_k3_circle_thread.onRun(caculateK3Circle);
  caculate_k3_circle_thread.setInterval(500);

  controller.add(&publish_data_thread);
  controller.add(&output_contactor_thread);

  controller.add(&caculate_k1_circle_thread);
  controller.add(&caculate_k2_circle_thread);
  controller.add(&caculate_k3_circle_thread);

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());
}


void caculateK1Circle() {
  int k1on = localState->getK1On() * 1000;
  int k1off = localState->getK1Off() * 1000;

  if (!localState->getK1CalOn() && delay_off_k1.isExpired()) {
    localState->setK1CalOn(true);
    localState->setRelayOne(true);
    Serial.println("On relay 1");
    delay_on_k1.start(k1on, AsyncDelay::MILLIS);
  }

  if (delay_on_k1.isExpired() && !localState->getK1CalOff() && localState->getK1CalOn()) {
    localState->setRelayOne(false);
    Serial.println("Off relay 1");
    delay_off_k1.start(k1off, AsyncDelay::MILLIS);
    localState->setK1CalOff(true);
  }

  if (delay_off_k1.isExpired() && localState->getK1CalOn() && localState->getK1CalOff()) {
    localState->setK1CalOff(false);
    localState->setK1CalOn(false);
  }
}

void caculateK2Circle() {
  int k2on = localState->getK2On() * 1000;
  int k2off = localState->getK2Off() * 1000;
  if (!localState->getK2CalOn() && delay_off_k2.isExpired()) {
    localState->setK2CalOn(true);
    localState->setRelayTwo(true);
    Serial.println("On relay 2");
    delay_on_k2.start(k2on, AsyncDelay::MILLIS);
  }

  if (delay_on_k2.isExpired() && !localState->getK2CalOff() && localState->getK2CalOn()) {
    localState->setRelayTwo(false);
    Serial.println("Off relay 2");
    delay_off_k2.start(k2off , AsyncDelay::MILLIS);
    localState->setK2CalOff(true);
  }

  if (delay_off_k2.isExpired( ) && localState->getK2CalOn() && localState->getK2CalOff() ) {
    localState->setK2CalOff(false);
    localState->setK2CalOn(false);
  }
}

void caculateK3Circle() {
  int k3on = localState->getK3On() * 1000;
  int k3off = localState->getK3Off() * 1000;

  if (!localState->getK3CalOn() && delay_off_k3.isExpired()) {
    localState->setK3CalOn(true);
    localState->setRelayThree(true);
    Serial.println("On relay 3");
    delay_on_k3.start(k3on , AsyncDelay::MILLIS);
  }
  if (delay_on_k3.isExpired() && !localState->getK3CalOff() && localState->getK3CalOn() ) {
    localState->setRelayThree(false);
    Serial.println("Off relay 3");
    delay_off_k3.start(k3off, AsyncDelay::MILLIS);
    localState->setK3CalOff(true);
  }
  if (delay_off_k3.isExpired() && localState->getK3CalOn() && localState->getK3CalOff()) {
    localState->setK3CalOff(false);
    localState->setK3CalOn(false);
  }
}

void outputContactor() {
  bool relayOne = localState->getRelayOne();
  bool relayTwo = localState->getRelayTwo();
  bool relayThree = localState->getRelayThree();
  //Output pin mode;

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
  if (client.publish("sol-farm/feedback", msg) == 0) {
    //Serial.println("Published Message");
  }
  else
  {
    //Serial.println("Publish failed");
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  controller.run();
  serialEvent();
}



void serialEvent() {
  String str = "";
  if (serialSW.available() > 0)
  {
    str = serialSW.readStringUntil('\n');
  }
  if (str != "") {
    parseJSONRecived(str);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(subcribe_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      char buf[256];
      espClient.getLastSSLError(buf, 256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);
    }
  }
}

void sendDataLora(String anything) {
  serialSW.println(anything);
}

void parseJSONRecived(String messageLora) {
  StaticJsonDocument<128> doc;
  auto error = deserializeJson(doc, messageLora);
  if (!error) {
    //Error handling
    if (doc.containsKey("nodeID")) {
      String id = doc["nodeID"].as<String>();

      if (id == "node01") {
        Serial.println("processs 1");
        node01State->setTempSHT21(doc["tempSHT21"].as<float>());
        node01State->setHumSHT21(doc["humSHT21"].as<float>());
        node01State->setDs18b20(doc["ds18b20"].as<float>());
      }
      if (id == "node02") {
        Serial.println("processs 2");
        node02State->setTempSHT21(doc["tempSHT21"].as<float>());
        node02State->setHumSHT21(doc["humSHT21"].as<float>());
        node02State->setDacValue(doc["dacValue"].as<int>());
      }
    }
  }
}
