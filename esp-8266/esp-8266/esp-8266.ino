#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ThreadController.h>
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
ThreadController controller = ThreadController();
Thread publish_data_thread = Thread();
//Thread check_mqtt_connect_thread = Thread();

LocalState* localState = new LocalState();
Node01State* node01State = new Node01State();
Node02State* node02State = new Node02State();

const char* AWS_endpoint = "a2184o3gtkvd1o-ats.iot.ap-southeast-1.amazonaws.com"; //MQTT broker ip


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient);

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

  //check_mqtt_connect_thread.onRun(checkMqttConnect);
  //check_mqtt_connect_thread.setInterval(3000);

  controller.add(&publish_data_thread);
  //controller.add(&check_mqtt_connect_thread);

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());
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
  checkMqttConnect();
  controller.run();
}



void serialEvent() {
  if (serialSW.available()) {
    String inputString = "";
    while (serialSW.available()) {
      char inChar = (char)serialSW.read();
      inputString += inChar;
    }
    Serial.print(inputString);
  }
}

void checkMqttConnect() {
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
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
