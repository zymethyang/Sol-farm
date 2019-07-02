#include <Arduino.h>
#include <ArduinoJson.h>

class LocalState {
  private:
    bool pressure = false;
    int esc = 0;
    bool relayOne = false;
    bool relayTwo = false;
    bool relayThree = false;
    int k1on = 10;
    int k1off = 10;
    int k2on = 10;
    int k2off = 10;
    int k3on = 10;
    int k3off = 10;

    bool k1CalOn = false;
    bool k2CalOn = false;
    bool k3CalOn = false;

    bool k1CalOff = false;
    bool k2CalOff = false;
    bool k3CalOff = false;

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
      StaticJsonDocument<128> doc;
      doc["nodeID"] = this->nodeID;
      doc["pressure"] = this->pressure;
      doc["esc"] =  this->esc;
      doc["relayOne"] = this->relayOne;
      doc["relayTwo"] = this->relayTwo;
      doc["relayThree"] = this->relayThree;
      String s = "";
      serializeJson(doc, s);
      return s;
    }

    void setK1On(int k1on) {
      this->k1on = k1on;
    }
    void setK2On(int k2on) {
      this->k2on = k2on;
    }
    void setK3On(int k3on) {
      this->k3on = k3on;
    }
    void setK1Off(int k1off) {
      this->k1off = k1off;
    }
    void setK2Off(int k2off) {
      this->k2off = k2off;
    }
    void setK3Off(int k3off) {
      this->k3off = k3off;
    }

    int getK1On() {
      return this->k1on;
    }
    int getK2On() {
      return this->k2on;
    }
    int getK3On() {
      return this->k3on;
    }
    int getK1Off() {
      return this->k1off;
    }
    int getK2Off() {
      return this->k2off;
    }
    int getK3Off() {
      return this->k3off;
    }

    bool getK1CalOn() {
      return this->k1CalOn;
    }
    bool getK2CalOn() {
      return this->k2CalOn;
    }
    bool getK3CalOn() {
      return this->k3CalOn;
    }

    void setK1CalOn(bool k1CalOn) {
      this->k1CalOn = k1CalOn;
    }
    void setK2CalOn(bool k2CalOn) {
      this->k2CalOn = k2CalOn;
    }
    void setK3CalOn(bool k3CalOn) {
      this->k3CalOn = k3CalOn;
    }

    bool getK1CalOff() {
      return this->k1CalOff;
    }
    bool getK2CalOff() {
      return this->k2CalOff;
    }
    bool getK3CalOff() {
      return this->k3CalOff;
    }

    void setK1CalOff(bool k1CalOff) {
      this->k1CalOff = k1CalOff;
    }
    void setK2CalOff(bool k2CalOff) {
      this->k2CalOff = k2CalOff;
    }
    void setK3CalOff(bool k3CalOff) {
      this->k3CalOff = k3CalOff;
    }
};
