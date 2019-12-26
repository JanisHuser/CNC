#ifndef __STORAGE_TYPES_H__
#define __STORAGE_TYPES_H__

#include <Arduino.h>

namespace Storage
{
  enum class Type
  {
    SD_CARD,
    USB,
    UNKNOWN
  };

  enum class Status
  {
    INIT_FAILED,
    INITIALIZED,
    STORAGE_DEVICE_MISSING,
    UNKNOWN
  };

  struct Directory_Content
  {
    char name[26];
    bool dir;
  };

};


#endif // !__STORAGE_TYPES_H__