#include <FlashStorage.h>
#include <EEPROM.h>

#define LOG_LEVEL 3
#include <Logging.h>

void LOG_CALLBACK(const char* txt) {  Serial.println(txt); }

void setup() {
  Serial.begin(115200);
  INFO("We are in setup\n");
  // put your setup code here, to run once:
  EEPROM.begin();
}

void loop() 
{
  static esp8266::polledTimeout::periodicMs nextPing(1000);

  if (nextPing) 
  {
    INFO("Looping...\n");
  }
  // put your main code here, to run repeatedly:

}
