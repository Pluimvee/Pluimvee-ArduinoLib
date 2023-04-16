#include <FlashBuffer.h>
#define LOG_REMOTE
#define LOG_LEVEL   2
#define LOG_PREFIX  "[FlashBuffer] - "
#include <Logging.h>

#if (0)
  #define BLOCK_SIZE    1024
#else
  #define BLOCK_SIZE    info.blockSize
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashBuffer::begin(const char* name, size_t record_size, size_t capacity)
{
  if (!LittleFS.begin())
     return false;
  FSInfo info;
  LittleFS.info(info);
  INFO(LOG_PREFIX "File system mounted: blocksize %d, total %d, used %d\n", info.blockSize, info.totalBytes, info.usedBytes);

  _file = LittleFS.open(name, "r+");      // first try to open an existing file for reading and writing
  if (!_file)
    _file = LittleFS.open(name, "w+");    // second try to open a new file for writing and reading (we can not use a+ as all writing would be appended)
  if (!_file)
  {
    ERROR(LOG_PREFIX "Failed to open caching file for writing"); // still no file.... error!
    return false;
  }
  INFO(LOG_PREFIX "File \'%s\' opened\n", name);

  if (_file.size() < sizeof(_meta))
  {
    // when no capacity is given we calculate based on a file block
    // substracting the meta header and divided by the record size
    if (capacity == 0)     
      capacity = (BLOCK_SIZE - sizeof(_meta)) / record_size;

    INFO(LOG_PREFIX "Creating new cache for records holding %d bytes. capacity nr of records %d\n", record_size, capacity);
    _meta.size  = record_size;
    _meta.max   = capacity;
    _meta.head  = 0;
    _meta.tail  = 0;
    _meta.count = 0;

    return _flush();
  }
  else
  { 
    if (!_file.seek(0) ||
        _file.read((uint8_t*)&_meta, sizeof(_meta)) != sizeof(_meta))
      return false;
    DEBUG(LOG_PREFIX "Meta data read with size %d, max %d, head %d, tail %d, count %d\n", _meta.size, _meta.max, _meta.head, _meta.tail, _meta.count);
    if (_meta.size != record_size)
    {
      INFO(LOG_PREFIX "Record size changed !! Wipe existing cache and start over with with a new cache\n");
      _file.truncate(0);  // truncate file to 0 bytes
      _file.close();      // close the file as the call to begin will re-open
      return begin(name, record_size, capacity);
    }
  }
  return true; 
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashBuffer::_seek(uint16_t index) const {
  return _file.seek(sizeof(_meta) + index * _meta.size);
}

bool FlashBuffer::_flush() {
  if (_file.seek(0) &&
      _file.write((uint8_t*) &_meta, sizeof(_meta)) == sizeof(_meta))
  {
    _file.flush();
    INFO(LOG_PREFIX "Meta flushed [size %d, max %d, head %d, tail %d, count %d]\n", _meta.size, _meta.max, _meta.head, _meta.tail, _meta.count);
    return true;
  }
  return false;
}

bool FlashBuffer::reset() // reset head, tail and cache in existing cache
{
  _meta.head = _meta.tail = _meta.count = 0;
  return _flush();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashBuffer::push(const void *record, bool force)
{
  if (isfull() && !force)
    return false;
  DEBUG(LOG_PREFIX "Writing record at %d\n", _meta.head);

  if (!_seek(_meta.head) || _file.write((uint8_t*) record, _meta.size) != _meta.size) {
    ERROR(LOG_PREFIX "Error pushing record to cache\n");
    return false;
  }
  // If the cache is full (and force is set) we have just written over the tail record 
  // we must increase the tail and rollover if needed
  if (isfull()) {
    if (++_meta.tail == capacity())
      _meta.tail = 0;
  }
  // change the write pointer, rolover if reached max
  if (++_meta.head == capacity())
    _meta.head = 0;
  if (++_meta.count > capacity())
    _meta.count = capacity(); 

  return _flush();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashBuffer::peek(void *record) const
{
  DEBUG(LOG_PREFIX "Peeking record at %d\n", _meta.tail);
  return  !isempty() &&
          _seek(_meta.tail) &&
          _file.read((uint8_t*) record, _meta.size) == _meta.size;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool FlashBuffer::pop(void *record)
{
  DEBUG(LOG_PREFIX "Retrieving record at %d\n", _meta.tail);
  if (record && !peek(record))  // if record is given, retrieve the record using the peek method
    return false;
  
  _meta.count--;                // the peek method checked for empty cache (count > 0)
  if (++_meta.tail == capacity())    // add moving the read pointer
    _meta.tail = 0;

  return _flush();
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

