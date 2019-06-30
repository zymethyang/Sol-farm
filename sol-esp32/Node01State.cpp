#include <Arduino.h>
#include <ArduinoJson.h>

class Node01State {
  private:
    float  tempSHT21 = 0.0;
    float humSHT21 = 0.0;
    float ds18b20 = 0.0;
    String nodeID = "node01";

  public:
    String buffString;
    void setTempSHT21(float tempSHT21) {
      this->tempSHT21 = tempSHT21;
    }
    float getTempSHT21() {
      return this->tempSHT21;
    }

    void setHumSHT21(float humSHT21) {
      this->humSHT21 = humSHT21;
    }
    float getHumSHT21() {
      return this->humSHT21;
    }

    void setDs18b20(float ds18b20) {
      this->ds18b20 = ds18b20;
    }
    float getDs18b20() {
      return this->ds18b20;
    }

    String getState() {
      DynamicJsonBuffer jb;
      JsonObject& doc = jb.createObject();
      doc["tempSHT21"] = this->tempSHT21;
      doc["humSHT21"] =  this->humSHT21;
      doc["ds18b20"] = this->ds18b20;
      doc["nodeID"] = this->nodeID;
      String s = "";
      doc.printTo(s);
      return s;
    }
    void updateState(JsonObject& doc) {
      this->tempSHT21 = doc["tempSHT21"].as<int>();
      this->humSHT21 = doc["humSHT21"].as<int>();
      this->ds18b20 = doc["ds18b20"].as<int>();
      Serial.println("Update node1 OK");
    }
    void updateState(char* loraBuffer) {
      //Lỗi không parse được
      //DynamicJsonBuffer jb;
      StaticJsonBuffer<128> jb;
      JsonObject& doc  = jb.parseObject(loraBuffer);

      if (doc.containsKey("nodeID")) {
        String nodeid = doc["nodeID"].as<String>();
        //Serial.println(nodeid);
        if (nodeid == "node01") {
          //strncpy(buffString, loraBuffer, strlen(loraBuffer));

          //doc.printTo(buffString);
          //          this->tempSHT21 = doc["tempSHT21"].as<float>();
          //          this->humSHT21 = doc["humSHT21"].as<float>();
          //          this->ds18b20 = doc["ds18b20"].as<float>();

          //Serial.println(buffString);
          Serial.println("Update node1 OK");

          //Serial.println(getState());
        }
      }
    }
    void setBuffString(String buff) {
      buffString = buff;
    }
    String getBuffString() {
      return buffString;
    }
};
