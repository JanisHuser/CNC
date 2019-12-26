#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "Arduino.h"

namespace Drivers
{

  #ifndef DRIVER_PIN_CNT
    #define DRIVER_PIN_CNT 9
  #endif
  enum class E_DIRECTION
  {
    FORWARD = 1,
    BACKWARD = 2,
    UNKNOWN = 3
  };

  enum class E_DRIVER_STATE
  {
    MOVING = 1,
    EMERGENCY_STOP = 2,
    SLEEPING = 3,
    HOLDING = 4,
    DISABLED = 5
  };

  enum class E_MCONFIG
  {
    M0 = 0x01,
    M1 = 0x02,
    M2 = 0x04,
  };


  enum class PIN_FUNC
  {
    ENABLE = 0,
    DIRECTION = 1,
    RESET = 2,
    SLEEP = 3,
    M0 = 4,
    M1 = 5,
    M2 = 6,
    STEP_CTRL = 7,
    FAULT = 8,
    UNKNOWN = -1
  };

  struct Pin
{
  PIN_FUNC func;
  int pin;
  bool state = false;
  bool invert = false;

  Pin(PIN_FUNC _func=PIN_FUNC::UNKNOWN, int _pin=-1)
  {
    func = _func;
    pin = _pin;
  }

  Pin(const Pin &value)
  {
    func = value.func;
    pin = value.pin;
    state = value.state;
    invert = value.invert;
  }
  Pin()
  {}
};

struct Range
{
  float from;
  float to;
  Range(float _from, float _to)
  {
    from = _from;
    to = _to;
  }

  Range(const Range &value)
  {
    from = value.from;
    to = value.to;
  }

};


struct StepConfig
{
  bool M0;
  bool M1;
  bool M2;
  StepConfig(bool _m0, bool _m1, bool _m2)
  : M0(_m0)
  , M1(_m1)
  , M2(_m2)
  { }
  StepConfig()
  {
    
  }

  StepConfig(const StepConfig &value)
  {
    M0 = value.M0;
    M1 = value.M1;
    M2 = value.M2;
  }

  String toString()
  {
    String str;
    str = "M0: ";
    str += (M0 ? "1" : "0");
    str += ", M1: ";
    str += (M1 ? "1" : "0");
    str += ", M2: ";
    str += (M2 ? "1" : "0");
    return str;
  }

};

 
  class C_Driver
  {
    public:
      C_Driver();

      ~C_Driver();

      bool setPin(PIN_FUNC func, int pin);
      int getPin(PIN_FUNC func);

      bool getPinStatus(PIN_FUNC func);
      bool setPinStatus(PIN_FUNC func, bool state);

      E_DRIVER_STATE getState();

      //move to pos, within time <default = 10000 msecs>
      bool setPosition(float pos, float ms = 10000.0);
      float getCurrentPosition();
      
      bool setRange(Range *range);
      Range* getRange();

      bool stepped(int steps);

      StepConfig* getStepConfig();
      bool setStepConfig(StepConfig *_stepConfig);

      bool setDriverName(String driverName);
      String getDriverName();

      bool Initialize();


    void setDistancePerRotation(float distance);
    float getDistancePerRotation();
    
     
    protected:
      Pin * mPins[DRIVER_PIN_CNT] = { NULL };


      String mDriverName;
      float mCurrPos = 0.0;
      float mPosition = 0.0;
      Range * mRange;
      StepConfig * mStepConfig;
      E_DRIVER_STATE mState;

      E_DIRECTION mMovingDir;


      float mDistPerRot = 0.0;
      float mDistPerStep = 0.0;

      bool mSetSleep(bool state);
      bool mSetHold(bool state);
      bool mSetDirection(E_DIRECTION dir);
      
  };
};


#endif    // !__DRIVER_H__
