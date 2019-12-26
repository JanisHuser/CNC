#ifndef __STORAGE_BASE_CLASS_H__
#define __STORAGE_BASE_CLASS_H__

#include <Arduino.h>
#include "../Storage/StorageTypes.h"
#include "../Helpers/Vector.h"

namespace Storage
{
  class BaseClass
  {
    public:
      BaseClass(const Type &type) : mType(type) {}
      virtual ~BaseClass() { mDirectory.Clear();}

      Type getType()
      {
        return mType;
      }
      bool IsInitialized()
      {
        return mInitialized;
      }
      String getCurrentPath()
      {
        return mCurrPath;
      }

      Directory_Content getItemAtPosition(int position)
      {
        if (mDirectory.Size() > position)
        {
          return mDirectory[position];
        }
        return Directory_Content();
      }
      int getDirectoryItemSize()
      {
        return mDirectory.Size();
      }

      virtual bool Initialize() = 0;
      virtual String getLine(unsigned long position [[maybe_unused]]) = 0;
      
      virtual bool openDir(String path [[maybe_unused]]) = 0;
      virtual bool openFile(String filename [[maybe_unused]]) = 0;
      
      virtual void ListDirectoryContent() = 0;

    protected:
      Storage::Type mType = Type::UNKNOWN;
      bool mInitialized = false;
      String mCurrPath;
      Storage::Status mStatus;
      Vector<Directory_Content> mDirectory;

  };
};

#endif //!__STORAGE_BASE_CLASS_H__