
#include "driver.h"

#include "Arduino.h"

using namespace Drivers;
using Drivers::E_MCONFIG;

#define TABLE_STEP_MASK_CONFIG TABLE_ENTRY(0, 1, "Full Step") \
                              TABLE_ENTRY(0x01, 2, "Half Step") \
                              TABLE_ENTRY(0x02, 4, "1/4 Step") \
                              TABLE_ENTRY(0x01 | 0x02, 8, "1/8 Step") \
                              TABLE_ENTRY(0x04, 16, "1/16 Step") \
                              TABLE_ENTRY(0x01|0x04, 32, "1/32 Step") \
                              TABLE_ENTRY(0x02|0x04, 32, "1/32 Step") \
                              TABLE_ENTRY(0x01|0x02|0x04, 32, "1/32 Step")


#ifndef PIN_FUNC_TABLE
#define PIN_FUNC_TABLE  ENTRY((int)PIN_FUNC::ENABLE, "Enable") \
                        ENTRY((int)PIN_FUNC::DIRECTION, "Direction") \
                        ENTRY((int)PIN_FUNC::RESET, "Reset") \
                        ENTRY((int)PIN_FUNC::SLEEP, "Sleep") \
                        ENTRY((int)PIN_FUNC::M0, "StepCfg M0") \
                        ENTRY((int)PIN_FUNC::M1, "StepCfg M1") \
                        ENTRY((int)PIN_FUNC::M2, "StepCfg M2") \
                        ENTRY((int)PIN_FUNC::STEP_CTRL, "Step Control") \
                        ENTRY((int)PIN_FUNC::FAULT, "Fault")

#endif


C_Driver::~C_Driver()
{
  for (int i=0; i < DRIVER_PIN_CNT; i++)
  {
    if (mPins[i] != NULL)
    {
      delete mPins[i];
    }
  }
}

C_Driver::C_Driver()
{
}


bool C_Driver::setPin(PIN_FUNC func, int pin)
{
  if ((int)func > DRIVER_PIN_CNT)
  {
    return false;
  }

  if (mPins[(int)func] == NULL)
  {
    mPins[(int)func] = new Pin(func);
  }
  mPins[(int)func]->pin = pin;

return true;
}

int C_Driver::getPin(PIN_FUNC func)
{
  if (mPins[(int)func] == NULL)
  {
    return -1;
  }
  return mPins[(int)func]->pin;
}

E_DRIVER_STATE C_Driver::getState()
{
  return mState;
}


bool C_Driver::setRange(Range *range)
{
  mRange = range;
}

Range* C_Driver::getRange()
{
  return mRange;
}

bool C_Driver::setPosition(float pos, float ms)
{
  if (pos > mCurrPos)
  {
    mSetHold(false);
  }
}

float C_Driver::getCurrentPosition()
{
  return mCurrPos;
}

StepConfig* C_Driver::getStepConfig()
{
  if (mStepConfig == NULL)
  {
    mStepConfig = new Drivers::StepConfig();
  }
  return mStepConfig;
}


bool C_Driver::setStepConfig(StepConfig *_stepConfig)
{
  mStepConfig = new StepConfig(_stepConfig->M0,_stepConfig->M1, _stepConfig->M2 );
  if (!setPinStatus(PIN_FUNC::M0, mStepConfig->M0))
  {
    return false;
  }
  if (!setPinStatus(PIN_FUNC::M1, mStepConfig->M1))
  {
    return false;
  }
  if (!setPinStatus(PIN_FUNC::M2, mStepConfig->M2))
  {
    return false;
  }

  int _config = (mStepConfig->M0) | (mStepConfig->M1 << 1) | (mStepConfig->M2 << 2);
  switch (_config)
  {
    #define TABLE_ENTRY(value, stepValue, descr) \
      case value: \
        { \
          mDistPerStep = ((mDistPerRot/200)/ stepValue); \
          Serial.print("Step Config: "); Serial.println(descr); \
        } \
        break;
        TABLE_STEP_MASK_CONFIG
      #undef TABLE_ENTRY
      default:
        mDistPerStep = 0.0;
  }
  
  return true;
}


bool C_Driver::setDriverName(String driverName)
{
  mDriverName = driverName;
  return (mDriverName == driverName);
}



String C_Driver::getDriverName()
{
  return mDriverName;
}

bool C_Driver::mSetSleep(bool state)
{
  if (mPins[(int)PIN_FUNC::SLEEP] == NULL)
  {
    mPins[(int)PIN_FUNC::SLEEP] = new Pin(PIN_FUNC::SLEEP);
  }
}


bool C_Driver::mSetHold(bool state)
{
  if (mPins[(int)PIN_FUNC::STEP_CTRL] == NULL)
  {
    Serial.println("ERROR: STEP Pin not set!");
    return false;
  }
  if (mState != E_DRIVER_STATE::HOLDING)
  {
    mState = E_DRIVER_STATE::HOLDING;
    mPins[(int)PIN_FUNC::STEP_CTRL]->state = state && state == mPins[(int)PIN_FUNC::STEP_CTRL]->invert;
    digitalWrite(mPins[(int)PIN_FUNC::STEP_CTRL]->pin, mPins[(int)PIN_FUNC::STEP_CTRL]->state);
  }
}


void C_Driver::setDistancePerRotation(float distance)
{
  mDistPerRot = distance;
}


float C_Driver::getDistancePerRotation()
{
  return mDistPerRot;
}



bool C_Driver::stepped(int steps)
{
  if (steps <= 0 || mState != E_DRIVER_STATE::MOVING)
  {
    return false;
  }
  if (mMovingDir == E_DIRECTION::FORWARD)
  {
    mCurrPos += (steps * mDistPerStep);
    if (mCurrPos >= mPosition)
    {
      mSetHold(true);
    }
  }
  else if (mMovingDir == E_DIRECTION::BACKWARD)
  {
    mCurrPos -= (steps * mDistPerStep);
    if (mCurrPos <= mPosition)
    {
      mSetHold(true);
    }
  }
}

bool C_Driver::mSetDirection(E_DIRECTION dir)
{
  if (mPins[(int)PIN_FUNC::DIRECTION] == NULL)
  {
    Serial.println("ERROR: Direction Pin not set!");
    return false;
  }
  if (mMovingDir != dir)
  {
    bool _hold = false;
    if (mState == E_DRIVER_STATE::HOLDING)
    {
      _hold = true;
    }
    mSetHold(true && !_hold);
    digitalWrite(mPins[(int)PIN_FUNC::DIRECTION]->pin, !mPins[(int)PIN_FUNC::DIRECTION]->invert && true);
    delayMicroseconds(200);
    digitalWrite(mPins[(int)PIN_FUNC::DIRECTION]->pin, !mPins[(int)PIN_FUNC::DIRECTION]->invert && false);

    mMovingDir = dir;
    if (!_hold)
    {
      mSetHold(false);
    }
    
  }
}


bool C_Driver::getPinStatus(PIN_FUNC func)
{
  if (int(func) >= DRIVER_PIN_CNT || mPins[(int)func] == NULL)
  {
    Serial.print("ERROR: Pin <");
    switch ((int)func)
    {
      #define ENTRY(val, pinName) \
      case val: \
      { \
        Serial.print(pinName); \
      } \
      break;
        PIN_FUNC_TABLE
      #undef ENTRY
      default:
      Serial.print("undefined");
    }
    Serial.println("> not set!");
    return false;
  }
  return mPins[(int)func]->state;
}

bool C_Driver::setPinStatus(PIN_FUNC func, bool state)
{
  if (int(func) >= DRIVER_PIN_CNT || mPins[(int)func] == NULL)
  {
    Serial.print("ERROR: Pin <");
    switch ((int)func)
    {
      #define ENTRY(val, pinName) \
      case val: \
      { \
        Serial.print(pinName); \
      } \
      break;
        PIN_FUNC_TABLE
      #undef ENTRY
      default:
      Serial.print("undefined");
    }
    Serial.println("> not set!");
    return false;
  }
  digitalWrite(mPins[(int)func]->pin, mPins[(int)func]->invert ? !state : state);
  
  mPins[(int)func]->state = state;
}


bool C_Driver::Initialize()
{

  Serial.print("Initializing Driver: "); Serial.println(mDriverName);
  Serial.println("Pinconfig: ");
  for (int i=0; i < DRIVER_PIN_CNT; i++)
  {
    Serial.print("\tPin ");
    switch (i)
    {
      #define ENTRY(val, pinName) \
      case val: \
      { \
        Serial.print("\tPin "); Serial.print(pinName); \
      } \
      break;
        PIN_FUNC_TABLE
      #undef ENTRY
      default:
      Serial.print("undefined");
    }
    Serial.print(": ");
    
    if (mPins[i] == NULL)
    {
      Serial.println("not set!");
    }
    else
    {
      Serial.println(mPins[i]->pin);


      pinMode(mPins[i], (i == (int)PIN_FUNC::FAULT) ? INPUT_PULLUP : OUTPUT);
    }
  }
  Serial.println("Parameters: ");
  Serial.print("\tRange: "); Serial.print(mRange->from); Serial.print("mm - "); Serial.print(mRange->to); Serial.println("mm");
  Serial.print("\tDistance per Rotation: "); Serial.print(mDistPerRot); Serial.println("mm");
  Serial.print("\tStepConfig: "); Serial.println(mStepConfig->toString());
  Serial.println("***************************************");

  
  return true;
}
