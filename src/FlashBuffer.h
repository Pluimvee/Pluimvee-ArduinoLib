#pragma once
#include <arduino.h>
////////////////////////////////////////////////////////////////////////////////////////////
// FlashCache is a cyclic buffer in flash memeory based on LittleFS
// It can be used to read and write records in flash, it overwrites the oldest records 
// when reached full
////////////////////////////////////////////////////////////////////////////////////////////
class FlashStack
{
public:
//  inline bool begin(size_t size, const char* name=0) { return begin(sizeof(Type), size, name); };
//  inline bool put(const Type& t) { return put((const uint8_t*) t); };

  bool begin(const char* cache_name, size_t record_size, size_t cache_size=0); 
  bool reset(); // reset head and tail in existing cache
  bool push(const void *record, bool force=false);  // if force=true and cache is full it will overwrite the oldest record
  bool peek(void *record) const;
  bool pop(void *record = 0);     // without a record it will pop and ignore
  inline size_t count() const { return _meta.count; };
  inline size_t max() const   { return _meta.max; };
  inline bool isempty() const { return count() == 0; };
  inline size_t space() const { return max() - count(); };
  inline bool isfull() const  { return max() == count(); };

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

/*
template <typename Type> class FStack : public FlashStack
{
public:
  inline bool begin(const char* cache_name) { return FlashStack::begin(cache_name, sizeof(Type), 0); };
  inline bool push(Type *record, bool force=false) { return FlashStack::push((void*) record, force); };
  inline bool peek(Type *record) const { return FlashStack::peek((void*) record); };
  inline bool pop(Type *record=0) { return FlashStack::pop((void*) record); };
};
*/
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
