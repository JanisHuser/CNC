#ifndef __GCODE_H__
#define __GCODE_H__


#include <Arduino.h>
#include "../Positions.h"
#include "../Helpers/Vector.h"
#include "../Helpers/macros.h"

namespace gcode
{
  struct S_PARAM
  {
    char p;
    float val;
  };



  class Handler
  {
    public:
      static Handler& getInstance()
      {
        static Handler handler;
        if (!handler.mInitialized)
        {
          handler.mInitialized = true;
        }
        return handler;
      }

      uint8_t DecodeCommand(String cmdLine);
      Positioning::E_MOVEMENT GetType();

      Positioning::Arc GetArc();
      Positioning::Point3D GetLinear();
      Positioning::Feedrate GetFeedrate();
      Positioning::E_UNIT GetUnit();
      Positioning::E_ARC_ROT GetArcDirection();

      
    private:
      enum class ContState
      {
        UNKNOWN,
        HANDLE
      };
      Handler()
      : mInitialized(false)
      { }
      bool mInitialized;


      int getGCode(String line);

      void handleSettings();
      void handlePosition();
      void handleLinearInterpolation();

      S_PARAM parseParam(String p);
      
      String removeComments(String line);

      SleepState<ContState> mSleepState = SleepState<ContState>(ContState::UNKNOWN);
      Positioning::E_UNIT mUnit;

      Positioning::E_MOVEMENT mGType = Positioning::E_MOVEMENT::UNKNOWN;
      Positioning::Arc mArc;
      Positioning::E_ARC_ROT mArcDirection;

      Positioning::Point3D mLinear;
      Positioning::Feedrate mFeedrate;
      Positioning::Point3D mLastPosition;

  };



  

  
};

#endif // !__GCODE_H__