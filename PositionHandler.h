#ifndef __POSITION_HANDLER_H__
#define __POSITION_HANDLER_H__

#include "driver.h"
#include "Helpers/macros.h"
#include "config.h"
#include "Positions.h"

#define FEED_RATE_LOOKUP \
        ENTRY(2.00, 150, 500)


namespace Running
{
  typedef struct Arc
  {
    Positioning::Arc arc;
    double angle;
  } Arc;

  typedef Positioning::Point3D Linear;
} // namespace Running

  


class PositionHandler
{
  
  public:
    static PositionHandler getInstance()
    {
      static PositionHandler instance;
      if (!instance.mInitialized)
      {
        instance.mInitialized = true;
      }
      return instance;
    }

    bool PositionReached();

    //Set the position for the spindle to move, after it reached the previously set position.
    
    uint8_t SetPosition(Positioning::E_MOVEMENT type,
    Positioning::E_UNIT unit,
    Positioning::Point3D linear,
    Positioning::Arc arc,
    Positioning::E_ARC_ROT arcDirection);

    uint8_t Continue();

    void SetStepPin(uint8_t pin);
    uint8_t getStepPin();

    uint8_t InitDrivers();

    void PositionReachedCB(void * cb);

    void FileOpened();

  private:
    enum class ContState
    {
      UNKNOWN,
      ARC_CLOCKWISE,
      ARC_COUNTER_CLOCKWISE,
      LINEAR_MOVEMENT,
      RAPID_MOVEMENT
    };

    PositionHandler()
    : mInitialized(false)
    , mIsMoving(false)
    , mStepPin(0)
    , mPositionReached(false)
    { }

    bool mInitialized;
    bool mIsMoving;

    uint8_t mStepPin;

    Callback mPositionReached_cb;

    Drivers::C_Driver mDrivers[4];

    SleepState <ContState>mSleepState = SleepState<ContState>(ContState::UNKNOWN);

    Running::Arc mRunningArc;
    Running::Linear mRunningLinear;

    bool mPositionReached;

    


};

#endif //!__POSITION_HANDLER_H__