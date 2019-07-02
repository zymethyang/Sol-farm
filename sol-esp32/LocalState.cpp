#include <Arduino.h>
#include <ArduinoJson.h>

class LocalState {
  private:
    bool pressure = false;
    int esc = 0;
    bool relayOne = false;
    bool relayTwo = false;
    bool relayThree = false;
    String nodeID = "esp32";
  public:
    void setPressure(bool state) {
      this->pressure = state;
    }
    bool getPressure() {
      return this->pressure;
    }

    void setESC(int esc) {
      this->esc = esc;
    }
    bool getESC() {
      return this->esc;
    }

    void setRelayOne(bool value) {
      this->relayOne = value;
    }
    bool getRelayOne() {
      return this->relayOne;
    }
    void setRelayTwo(bool value) {
      this->relayTwo = value;
    }
    bool getRelayTwo() {
      return this->relayTwo;
    }

    void setRelayThree(bool value) {
      this->relayThree = value;
    }
    bool getRelayThree() {
      return this->relayThree;
    }

  
    String getState() {
      StaticJsonBuffer<128> jb;
      JsonObject& doc = jb.createObject();
      doc["nodeID"] = this->nodeID;
      doc["pressure"] = this->pressure;
      doc["esc"] =  this->esc;
      doc["relayOne"] = this->relayOne;
      doc["relayTwo"] = this->relayTwo;
      doc["relayThree"] = this->relayThree;
      String s = "";
      doc.printTo(s);
      return s;
    }
};
