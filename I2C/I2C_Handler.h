#ifndef __I2C_HANDLER_H__
#define __I2C_HANDLER_H__

#include <Arduino.h>
#include "../Helpers/macros.h"
#include "../Helpers/Vector.h"

namespace I2C
{
  enum class Event_Type
  {
    WRITE,
    REQUEST,
    WRITE_REQUEST,
    UNKNOWN
  };

  typedef unsigned char I2C_Address;

  struct Write_Event
  {
    unsigned char data[255];
    int len;
  };

  struct Request_Answer
  {
    unsigned char data[255];
    unsigned char len;
  };

  struct Request_Event
  {
    int len;
  };

  struct Event
  {
    Event_Type type = Event_Type::UNKNOWN;
    I2C_Address address;
    unsigned char reg;

    Write_Event writeEvent;
    Request_Event reqEvent;

    bool handled = false;
    bool finalised = false;
    Callback callback; 

    Event(  Event_Type _type = Event_Type::UNKNOWN,
            I2C_Address _address = 0x00)
    :type(_type)
    , address(_address)
    {}  
  };

  struct SLAVE_ANSWER
  {
    unsigned char address;
    unsigned char reg;
    int len;
    Vector<unsigned char> data;
  };

  class Handler
  {
    public:
      static Handler& getInstance()
      {
        static Handler i2cHandler;
        if (!i2cHandler.mInitialized)
        {
          i2cHandler.mInit();
        }
        return i2cHandler;
      }
      
      bool isInitialized() const { return mInitialized; }




      void Continue (void);
      uint16_t AddEvent(const Event &ev);
      uint16_t ExecuteEvent(void);

      Handler& operator<<(const Event &ev);


    private:
      bool mInitialized;
      ///< resources have been loaded
      void mInit();

      /* disabled: singleton class */
      Handler(): mInitialized(false)
      {}

      Handler(const Handler& other);
      Handler& operator=(const Handler& other);

      Vector<Event> mQueue;
      Vector<SLAVE_ANSWER> mSlaveAnswers;

      SleepState<> mSleepState;

      uint16_t mQueueCnt=0;

  };
};


#endif
