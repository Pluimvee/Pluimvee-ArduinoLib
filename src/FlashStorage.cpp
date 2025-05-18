//#include <ArduinoJson.h>
#include <FlashStorage.h>
//#include <EEPROM.h>

#define MAGIC_LENGTH 2
////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashStorage::begin()
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
/*
  bool begin();
  template<T> bool write(const T* r)  { return write(r, sizeof(r)); }
  template<T> bool read(T* r)         { return read(r, sizeof(r)); }
  bool write(const void *record, size_t lg);
  bool read(void *record, size_t lg);

https://github.com/nrwiersma/ConfigManager/blob/master/src/ConfigManager.cpp
https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

*/
/*
  JsonDocument<200> doc;
  auto error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  const char* serverName = doc["serverName"];
  const char* accessToken = doc["accessToken"];

  // Real world application would store these values in some variables for
  // later use.

  Serial.print("Loaded serverName: ");
  Serial.println(serverName);
  Serial.print("Loaded accessToken: ");
  Serial.println(accessToken);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////
bool saveConfig() {
  StaticJsonDocument<200> doc;
  doc["serverName"] = "api.example.com";
  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  return true;
}
*/