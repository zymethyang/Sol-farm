#include <Arduino.h>
#include <ArduinoJson.h>

class Node01State {
  private:
    float tempSHT21 = 0.0;
    float humSHT21 = 0.0;
    float ds18b20 = 0.0;
    String nodeID = "node01";

  public:
    void setTempSHT21(float tempSHT21) {
      this->tempSHT21 = tempSHT21;
    }
    float getTempSHT21() {
      return this->tempSHT21;
    }

    void setHumSHT21(float humSHT21) {
      this->tempSHT21 = humSHT21;
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
      StaticJsonDocument<128> doc;
      String s = "";
      doc["tempSHT21"] = this->tempSHT21;
      doc["humSHT21"] =  this->humSHT21;
      doc["ds18b20"] = this->ds18b20;
      doc["nodeID"] = this->nodeID;
      serializeJson(doc, s);
      return s;
    }

    void updateState(String loraBuffer) {
      StaticJsonDocument<128> doc;
      deserializeJson(doc, loraBuffer);

      if (doc.containsKey("nodeID")) {
        if (doc["nodeID"] == "node01") {
          this->tempSHT21 = doc["tempSHT21"];
          this->humSHT21 = doc["humSHT21"];
          this->ds18b20 = doc["ds18b20"];
        }
      }
    }
};
