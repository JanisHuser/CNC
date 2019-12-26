#include "drv8825.h"

bool C_DRV8825::init(String dName)
{


  
  mDriverName = dName;
  
  
  mSetSleep(false, true);
  mSetHolding(false, true);


    //set the StepConfig
  mUpdateStepSettings();

  
  return true;
}




bool C_DRV8825::setPosition(double pos)
{
  mSetHolding(true);
  if (pos > mMax)
  {
    return false;
  }
  
  if (pos != mCurrPosition)
  {
     mPosition = pos;
    if (pos < mCurrPosition)
    {
      mSetDirection(E_DIRECTION::BACKWARD);
    }
    else
    {
      mSetDirection(E_DIRECTION::FORWARD);
    }
    mSetHolding(false, true);
    
  }
  return true;
}

double C_DRV8825::getPosition()
{
  return mPosition;
}

double C_DRV8825::getCurrentPosition()
{
  return mCurrPosition;
}


C_DRV8825::E_DIRECTION C_DRV8825::getDirection()
{
  return mCurrDir;
}

C_DRV8825::E_DRIVER_STATE C_DRV8825::getState()
{
  return mState;
}
