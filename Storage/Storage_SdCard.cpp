#include "Storage_SdCard.h"
#include "../config.h"

#include <SD.h>
#include <SPI.h>




using namespace Storage;


static SDLib::File root;

SDCard::SDCard(unsigned char chipselect)
: BaseClass(Type::SD_CARD)
, mChipSelect(chipselect)
, mCurrLine("")
, mCurrLineReady(false)
, mPreCharacter("")
{ }


bool SDCard::Initialize()
{
  if (!SD.begin(mChipSelect))
  {
    mStatus = Status::INIT_FAILED;
    return false;
  }
  mStatus = Status::INITIALIZED;
  mCurrPath = "/";
  return true;
}

void SDCard::ListDirectoryContent() 
{
  mSleepState.Enable(0, ContState::LIST_DIR_OPEN_PATH);
}



String SDCard::getLine(unsigned long position)
{
  return "";
}
bool SDCard::openDir(String path)
{
  //since we closed the root file, we need to open it again
  String nPath = mCurrPath  + (mCurrPath == "/" ? "" : "/") + path;
  root = SD.open(nPath);
  if (!root)
  {
    root.close();
    return false;
  }
  root.close();
  mCurrPath = nPath;
  return true;
}
bool SDCard::openFile(String filename)
{
  if (SD.exists(filename))
  {
    mCurrPath = filename;
    mCurrLine = "";
    mSleepState.Enable(0, ContState::OPEN_FILE);
    return true;
  }
  return false;
}

SDCard::~SDCard()
{
  mDirectory.Clear();
}


void SDCard::Continue()
{
  if (mSleepState.IsSleeping())
  {
    return;
  }
  if(mSleepState.StateFit(ContState::LIST_DIR_OPEN_PATH))
  {
    mDirectory.Clear();
    root = SD.open(mCurrPath);
    if (root)
    {
      mSleepState.Enable(0,ContState::LIST_DIR_NEXT_FILE);
    }
    else 
    {
      root.close();
      if (directoryListed.function != nullptr)
      {
        directoryListed.function(directoryListed.data);
      }
      mSleepState.Reset();
    }
  }

  if (mSleepState.StateFit(ContState::LIST_DIR_NEXT_FILE))
  {
    if (root)
    {
      File entry = root.openNextFile();
      if (!entry)
      {
        entry.close();
        root.close();
        mSleepState.Reset();
        if (directoryListed.function != nullptr)
        {
          directoryListed.function(directoryListed.data);
        }
        return;
      }
      Directory_Content ct;
      
      memcpy(ct.name, entry.name(),25);
      ct.dir = entry.isDirectory();
      if (!ct.dir)
      {
        String c = String(ct.name);
        c.toLowerCase();
        if (c.indexOf(".nc") == -1)
        {
          entry.close();
          return;
        }
      }
      mDirectory.PushBack(ct);
      entry.close();
    }
    else
    {
      root.close();
      if (directoryListed.function != nullptr)
      {
        directoryListed.function(directoryListed.data);
      }
      mSleepState.Reset();
    }
  }

  if (mSleepState.StateFit(ContState::OPEN_FILE))
  {
    if (SD.exists(mCurrPath))
    {
      mFile = SD.open(mCurrPath);
      if (mFile)
      {
        mSleepState.Enable(0, ContState::READ_FILE_CONTENT);
      }
      else
      {
        mSleepState.Reset();
      }
    }
  }

  if (mSleepState.StateFit(ContState::READ_FILE_CONTENT))
  {
    int av = mFile.available();
    char c;
    if (mCurrLineReady)
    {
      return;
    }
    if (av > 0)
    {
      c = mFile.read();
      if ( c == '\r' || c == '\n')
      {
        mCurrLineReady = true;
        return;
      }
      mCurrLine += c;
    }
    else
    {
      mSleepState.Reset();
    }
  }
}

String SDCard::getNextLine()
{
  String str ="";
  if (mCurrLineReady)
  {
    str = mCurrLine;
    mCurrLine = "";
    mCurrLineReady = false;
  }
  return str;
}