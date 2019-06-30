#include <Arduino.h>
#include <ArduinoJson.h>

class LocalState {
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

    void setDacValue(float dacValue) {
      this->dacValue = dacValue;
    }
    float getDacValue() {
      return this->dacValue;
    }

    String getState() {
      StaticJsonBuffer<128> jsonBuffer;
      JsonObject& doc = jsonBuffer.createObject();
      String s = "";
      doc["tempSHT21"] = this->tempSHT21;
      doc["humSHT21"] =  this->humSHT21;
      doc["dacValue"] = this->dacValue;
      doc["nodeID"] = this->nodeID;
      doc.printTo(s);
      return s;
    }

    void updateState(String loraBuffer) {
      DynamicJsonBuffer jb;
      JsonObject& doc  = jb.parseObject(loraBuffer);
      if (doc.containsKey("nodeID")) {
        if (doc["nodeID"] == "node02") {
          if (doc.containsKey("dacValue")) {
            this->dacValue = doc["dacValue"];
          }
        }
      }
    }
};
