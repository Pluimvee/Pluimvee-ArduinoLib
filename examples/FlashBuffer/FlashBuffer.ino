#include <FlashBuffer.h>
#define LOG_LEVEL 0
#include <Logging.h>

FlashBuffer cache;
////////////////////////////////////////////////////////////////////////////////////////////
// 
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
  cache.begin<record>("/test");

  DEBUG("Cache status: full %d, empty %d, count %d, space %d\n", cache.isfull(), cache.isempty(), cache.count(), cache.space());
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
/////////////////////////////////////////////////////////////////////////////////
