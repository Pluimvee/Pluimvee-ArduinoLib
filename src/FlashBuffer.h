#pragma once
#include <arduino.h>
#include <LittleFS.h>

////////////////////////////////////////////////////////////////////////////////////////////
// FlashBuffer is a cyclic buffer in flash memory based on LittleFS
// On push() with a full buffer the force flag will overwrite the oldest record
////////////////////////////////////////////////////////////////////////////////////////////
class FlashBuffer
{
public:
  template <typename T> bool begin(const char *name) { return begin(name, sizeof(T), 0); };

  bool begin(const char* name, size_t record_size, size_t capacity=0); 
  bool reset(); // reset head and tail in existing cache
  bool push(const void *record, bool force=false);  // if force=true and cache is full it will overwrite the oldest record
  bool peek(void *record) const;
  bool pop(void *record = 0);     // without a record it will pop and ignore
  inline size_t count() const     { return _meta.count; };
  inline size_t capacity() const  { return _meta.max; };
  inline size_t space() const     { return capacity() - count(); };
  inline bool isempty() const     { return count() == 0; };
  inline bool isfull() const      { return capacity() == count(); };

private:
  mutable File _file;
  struct _pointers {
    uint16_t size;        // size of each record
    uint16_t max;         // max records in cache
    uint16_t count;       // current nr of records  --> a simple head-tail isnt working when the buffer is cycling
    uint16_t head;        // next index for writing
    uint16_t tail;        // next index for reading
  } _meta;
  bool _seek(uint16_t index) const; // set file pointer to record offset
  bool _flush();          // flush the meta and anything else what was written
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
