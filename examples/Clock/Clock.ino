#include <Timer.h>
#include <Clock.h>
#include <LED.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#define LOG_LEVEL 3
#include <Logging.h>

const char* sta_ssid      = STA_SSID;
const char* sta_password  = STA_PASS;

/////////////////////////////////////////////////////////////////////////////////
Clock     rtc;      // the software clock
Timer     sec30;    // a timer for each 30 seconds
DateTime  reconfig; // a reconfig time
LED       led;
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void wifi_connect() 
{
  // We start by connecting to a WiFi network
  INFO("\n\nWifi connecting to %s", sta_ssid);

  WiFi.begin(sta_ssid, sta_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    INFO(".");
  }
  INFO("\nWiFi connected with IP address: %s\n", WiFi.localIP().toString().c_str());
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(115200);
  wifi_connect();

  char buf[64];
  strcpy(buf, "The boot time is YYYY-MM-DD hh:mm:ss\n");
  DEBUG(rtc.now().toString(buf));

  DEBUG("Sync time\n");
  rtc.ntp_sync();
  strcpy(buf, "The time set to YYYY-MM-DD hh:mm:ss\n");
  DEBUG(rtc.now().toString(buf));

  reconfig = rtc.now() + TimeSpan(0,0,5,0);
  strcpy(buf, "We will reconfigure at hh:mm:ss\n");
  DEBUG(reconfig.toString(buf));

  sec30.alarm(1000*30);   // set alarm 30 seconds from now
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  DateTime now = rtc.now();
  char buf[64];
  strcpy(buf, "The time is YYYY-MM-DD hh:mm:ss");
  DEBUG(now.toString(buf));
  DEBUG(" | The system is running for %d ms, the alarm is running for %d ms\n", millis(), sec30.elapsed());

  if (sec30.alarm())
  {
    DEBUG("30 seconds have passed using the timer, lets check the diff between NTP and our software clock\n");
    Clock ntp;
    ntp.ntp_sync();
    TimeSpan diff = rtc.now() - now;
    DEBUG("The difference between ntp and rtc are %d seconds\n", diff.seconds());

    if (diff.minutes() > 0) // more then a minute diff, lets resync
    {
      ERROR("The difference is more than 1 minute. resync\n");
      rtc.adjust(ntp.now());
    }
    // reset alarm timer for another 30 seonds
    sec30.alarm(30000);
  }
  if (now > reconfig)
  {
    DEBUG("5 minutes have pasted on the clock\n");
    reconfig = now + TimeSpan(0,0,5,0);
  }
  led.blink();
  delay(1000);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

