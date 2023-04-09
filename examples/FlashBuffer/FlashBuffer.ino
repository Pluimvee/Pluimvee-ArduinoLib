#include <ESP8266WiFi.h>
//#include "..\libraries\PluimLib\timer.h"
#include "secrets.h"
#include <FlashBuffer.h>

#define DEBUG(...)    //Serial.printf(__VA_ARGS__)
#define INFO(...)     Serial.printf(__VA_ARGS__)
#define ERROR(...)    Serial.printf(__VA_ARGS__)

const char* sta_ssid    = STA_SSID;
const char* sta_password = STA_PASS;

//Clock rtc;
FlashStack cache;
////////////////////////////////////////////////////////////////////////////////////////////
// Connect to the STA network
////////////////////////////////////////////////////////////////////////////////////////////
struct record {
  uint32_t time;
  char text[32];
  uint8_t other[30];
};

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  DEBUG("\nStarting cache library\n");
  cache.begin("/test", sizeof(record));

  DEBUG("Cache status: full %d, empty %d, count %d, space %d, head %d, tail %d\n", cache.isfull(), cache.isempty(), cache.count(), cache.space(), cache.head(), cache.tail());

}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////

int loopidx = 0;
int last_popped = 0;

void loop() 
{
  INFO("Cache status: full %d, empty %d, count %d, space %d\n", cache.isfull(), cache.isempty(), cache.count(), cache.space());

  record r;
  switch (random() % 3)
  {
    case 0:
    default:
      if (!cache.peek(&r))
        ERROR("FAILED peeking record -- buffer empty? %s\n", cache.isempty()? "YES": "NO");
      else
        DEBUG("Record peeked: id %d, stamp %d, text %s\n", r.other[0], r.time, r.text);
      break;

    case 1:
      r.time = millis();
      r.other[0] = loopidx;
      sprintf(r.text, "Record %d", loopidx++);
      if (cache.push(&r))
        DEBUG("Record added: id %d, stamp %d, text %s\n", r.other[0], r.time, r.text);
      else
        ERROR("FAILED adding new record\n");
      break;

    case 2:
      if (!cache.pop(&r))
        ERROR("FAILED getting record -- buffer empty? %s\n", cache.isempty()? "YES": "NO");
      else
      {
        DEBUG("Record popped: id %d, stamp %d, text %s\n", r.other[0], r.time, r.text);
        if (last_popped != r.other[0])
          ERROR("ERROR popped msg id %d while expected %d -- reason overflow? %s\n", r.other[0], last_popped, cache.isfull()? "YES": "NO");

        last_popped = r.other[0] +1;
      }
      break;
  }
  delay(1000);
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
/*
void wifi_connect() 
{
  // We start by connecting to a WiFi network
  INFO("Wifi connecting to %s", sta_ssid);

  WiFi.begin(sta_ssid, sta_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    INFO(".");
  }
  INFO("\nWiFi connected with IP address: %s\n", WiFi.localIP().toString().c_str());
}

DateTime reconfig;

void setup() {
  Serial.begin(115200);
  wifi_connect();

  char buf[64];
  strcpy(buf, "The boot time is YYYY-MM-DD hh:mm:ss\n");
  DEBUG(rtc.now().toString(buf));

  DEBUG("Sync time\n");
  rtc.ntp_sync();
  strcpy(buf, "The time set to YYYY-MM-DD hh:mm:ss\n");
  DEBUG(rtc.now().toString(buf));

  reconfig = rtc.now() + TimeSpan(0,0,1,0);
  strcpy(buf, "We will reconfigure at hh:mm:ss\n");
  DEBUG(reconfig.toString(buf));
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////

void loop() {
  DateTime now = rtc.now();
  char buf[64];
  strcpy(buf, "The time is YYYY-MM-DD hh:mm:ss");
  DEBUG(now.toString(buf));
  DEBUG(" | The system is running for %d ms\n", millis());

  if (now > reconfig)
  {
    DEBUG("Its time to sync again\n");
    rtc.ntp_sync();
    strcpy(buf, "The time set to YYYY-MM-DD hh:mm:ss\n");
    DEBUG(rtc.now().toString(buf));

    reconfig = reconfig+TimeSpan(0,0,1,0);
  }
  delay(1000);
}
*/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

