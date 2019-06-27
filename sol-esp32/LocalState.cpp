#include <Arduino.h>
#include <ArduinoJson.h>

class LocalState {
  private:
    bool pressure = false;
    int esc = 0;
    bool relay[3] = {false, false, false};
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

    void setRelay(int index, bool value) {
      this->relay[index] = value;
    }
    bool getRelay(int index) {
      return this->relay[index];
    }
    String getState() {
      StaticJsonBuffer<128> jb;
      JsonObject& doc = jb.createObject();
      doc["pressure"] = this->pressure;
      doc["esc"] =  this->esc;
      //doc["relay"] = this->relay;
      String s = "";
      doc.printTo(s);
      return s;
    }
};
