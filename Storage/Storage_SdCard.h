#ifndef __STORAGE_SD_CARD_H__
#define __STORAGE_SD_CARD_H__

#include "Storage_BaseClass.h"
#include "../Helpers/Vector.h"
#include "StorageTypes.h"
#include "../Helpers/macros.h"

#include <SD.h>

namespace Storage
{
  class SDCard : public BaseClass
  {
    public:
      SDCard(unsigned char chipselect);
      ~SDCard();

      bool Initialize();
      String getLine(unsigned long position);
      bool openDir(String path);
      bool openFile(String filename );
      void ListDirectoryContent();

      void Continue();
      String getNextLine();
      Callback directoryListed;
    private:
      enum class ContState
      {
        UNKNOWN,
        LIST_DIR_OPEN_PATH,
        LIST_DIR_NEXT_FILE,
        OPEN_FILE,
        READ_FILE_CONTENT
      };
      File mFile;
      unsigned char mChipSelect;
      SleepState<ContState> mSleepState = SleepState<ContState>(ContState::UNKNOWN);
      Vector<String> mFileContent;

      String mCurrLine;
      char mPreCharacter;
      bool mCurrLineReady;
      
  };
};


#endif // !__STORAGE_SD_CARD_H__