
#include <Wire.h>
#include "../config.h"

#include "I2C_Handler.h"


using namespace I2C;

void Handler::mInit()
{
  if (!mInitialized)
  {
    mInitialized = true;
    Wire.begin(); 
  }
}


uint16_t Handler::AddEvent(const Event &ev)
{
  mQueue.PushBack(ev);
  return mQueue.Size();
}

Handler& Handler::operator<<(const Event &ev)
{
  AddEvent(ev);
  return *this;
}


void Handler::Continue (void)
{
  if (mSleepState.IsSleeping())
  {
    return;
  }
  int index;
  if (mSleepState.StateFit(0))
  {
    Wire.requestFrom(Config::SD_CARD_ADDRESS, 3);
    mSleepState.Enable(1000,1);
  }
  if (mSleepState.StateFit(1))
  {
    unsigned char temp_buff[3];
    index = 0;
    while (Wire.available() && index < 3)
    {
      temp_buff[index++] = Wire.read();
    }

    //Don't mind this event, the slave doesn't have new data for us
    if (temp_buff[0] == 0x00)
    {
      mSleepState.Reset();
      return;
    }
    //everything ok, get the data
    if (index >= 3)
    {
      LOG(temp_buff[0]);
      //get the rest of the data
      int len = temp_buff[1] | (temp_buff[2] << 8);
      SLAVE_ANSWER answ;
      answ.address = Config::SD_CARD_ADDRESS;
      answ.reg = temp_buff[0];
      
      for (int i=0; i < len && Wire.available(); i++)
      {
        answ.data.PushBack(Wire.read());
      }
      int lenQueue = mQueue.Size();
      for (int i=0; i < lenQueue; i++)
      {
        if (mQueue[i].handled && mQueue[i].reg == answ.reg)
        {
          I2C::Event event = mQueue[i];
          I2C::Request_Answer * req = (Request_Answer* )event.callback.data;
          for (int i=0; i < answ.len; i++)
          {
            req->data[i] = answ.data[i];
          }
          req->len = answ.len;
          if (event.callback.function != nullptr)
          {
            event.callback.function(req);
          }
          mQueue.Erase(i);
          // mQueue[i].finalised = true;
          break;
        } 
      }
      mSleepState.Reset();
    } 
  }
}

uint16_t Handler::ExecuteEvent(void)
{
  int lenQueue = mQueue.Size();
  if ( lenQueue == 0)
  {
    return 0;
  }
  
  int i=0;
  for (int zl=0; zl < lenQueue; zl++)
  {
    if (!mQueue[zl].handled)
    {
      Event event = mQueue[zl];
      if (event.address <= 0x00 || event.address >= 0x7F)
      {
        mQueue.Erase(zl);
        return 0;
      }
      if (event.type == Event_Type::WRITE)
      {
        Wire.beginTransmission(event.address);
        int dataLen = event.writeEvent.len;
        for (int i=0; i < dataLen; i++)
        {
          Wire.write(event.writeEvent.data[i]);
        }
        Wire.endTransmission();
      }
      else if (event.type == Event_Type::REQUEST)
      {
        if (event.reqEvent.len > 0)
        {
          unsigned char buff[255];
          int buffPos = 0;
          Wire.requestFrom(event.address, event.reqEvent.len);
          delayMicroseconds(50);
          while (Wire.available() && buffPos < 255)
          {
            buff[buffPos++] = Wire.read();
          }
        }
      }
      else if (event.type == Event_Type::WRITE_REQUEST)
      {
        Wire.beginTransmission(event.address);    
        int dataLen = event.writeEvent.len;
        for (int i=0; i < dataLen; i++)
        {
          Wire.write(event.writeEvent.data[i]);
        }
        Wire.endTransmission();
        delayMicroseconds(50);
        int l = event.reqEvent.len;
        if (l > 0)
        {
          unsigned char buff[255];
          int buffPos = 0;
          Wire.requestFrom(event.address, l);
          delayMicroseconds(50);
          while (Wire.available() && buffPos < 255)
          {
            unsigned char c = Wire.read();
            buff[buffPos++] = c;
          }
          if (event.callback.function != nullptr)
          {
            Request_Answer * req = (Request_Answer* )event.callback.data;
            memcpy(&req->data, buff, buffPos);
            req->len = l;
            event.callback.function(event.callback.data);
          }
        }
      }
      mQueue[zl].handled = true;
      break;
    }
  }
  return mQueue.Size();
}