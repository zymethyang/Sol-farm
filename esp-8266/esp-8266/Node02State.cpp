#include <Arduino.h>
#include <ArduinoJson.h>

class Node02State {
  private:
    float tempSHT21 = 0.0;
    float humSHT21 = 0.0;
    int dacValue = 0;
    String nodeID = "node02";
  public:
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

    void setDacValue(int dacValue) {
      this->dacValue = dacValue;
    }
    float getDacValue() {
      return this->dacValue;
    }

    String getState() {
      StaticJsonDocument<128> doc;
      String s = "";
      doc["tempSHT21"] = this->tempSHT21;
      doc["humSHT21"] =  this->humSHT21;
      doc["dacValue"] = this->dacValue;
      doc["nodeID"] = this->nodeID;
      serializeJson(doc, s);
      return s;
    }

    void updateState(char* loraBuffer) {
      StaticJsonDocument<128> doc;
      deserializeJson(doc, loraBuffer);

      if (doc.containsKey("nodeID")) {
        String nodeid = doc["nodeID"].as<String>();
        Serial.println(nodeid);
        if (nodeid == "node02") {
          //doc.printTo(buffString);
          //          this->tempSHT21 = doc["tempSHT21"].as<float>();
          //          this->humSHT21 = doc["humSHT21"].as<float>();
          //          this->dacValue = doc["dacValue"].as<int>();
          //strncpy(buffString, loraBuffer, strlen(loraBuffer));
          //Serial.println(buffString);
          Serial.println("Update node2 OK");
          //Serial.println(getState());
        }
      }
    }
};
