#include <Arduino.h>

#include "PositionHandler.h"
#include "pinDefinitions.h"
#include "config.h"




uint8_t PositionHandler::SetPosition(Positioning::E_MOVEMENT type,
    Positioning::E_UNIT unit,
    Positioning::Point3D linear,
    Positioning::Arc arc, Positioning::E_ARC_ROT arcDirection)
{
  mPositionReached = false;
  Serial.println("SetPosition");
  if (type == Positioning::E_MOVEMENT::LINEAR_INTERPOLATION)
  {
    
    mDrivers[0].setPosition(linear.x);
    mDrivers[1].setPosition(linear.x);
    mDrivers[2].setPosition(linear.y);
    mDrivers[3].setPosition(linear.z);
    mSleepState.Enable(0, ContState::LINEAR_MOVEMENT);
  }
  //just move to the position, no care
  if (type == Positioning::E_MOVEMENT::RAPID_MOVEMENT)
  {
    mDrivers[0].setPosition(linear.x);
    mDrivers[1].setPosition(linear.x);
    mDrivers[2].setPosition(linear.y);
    mDrivers[3].setPosition(linear.z);
    mSleepState.Enable(0, ContState::RAPID_MOVEMENT);
  }
  if (type == Positioning::E_MOVEMENT::ARC)
  {
    mSleepState.Enable(0, ContState::ARC_CLOCKWISE);
  }
  
}

uint8_t PositionHandler::Continue()
{
  if (mSleepState.IsSleeping())
  {
    return 1;
  }
  digitalWrite(mStepPin, LOW);
  digitalWrite(mStepPin, HIGH);
  for (int i=0; i < 4; i++)
  {
    mDrivers[i].stepped(1);
  }
  
  if (mSleepState.StateFit(ContState::LINEAR_MOVEMENT))
  {
    Serial.println("moving: lin ");
    if (mDrivers[0].getCurrentPosition() == mRunningLinear.x &&
        mDrivers[1].getCurrentPosition() == mRunningLinear.x &&
        mDrivers[2].getCurrentPosition() == mRunningLinear.y &&
        mDrivers[3].getCurrentPosition() == mRunningLinear.z)
        {
          mPositionReached = true;
         
        }
    Serial.print("LINEAR_MOVEMENT: ");
    Serial.print(mDrivers[0].getCurrentPosition()); Serial.print("\t");
    Serial.print(mDrivers[1].getCurrentPosition()); Serial.print("\t");
    Serial.print(mDrivers[2].getCurrentPosition()); Serial.print("\t");
    Serial.print(mDrivers[3].getCurrentPosition()); Serial.print("\n");
  }
  if (mSleepState.StateFit(ContState::RAPID_MOVEMENT))
  {
    Serial.println("moving: rapid");
    if (mDrivers[0].getCurrentPosition() == mRunningLinear.x &&
      mDrivers[1].getCurrentPosition() == mRunningLinear.x &&
      mDrivers[2].getCurrentPosition() == mRunningLinear.y &&
      mDrivers[3].getCurrentPosition() == mRunningLinear.z)
      {
        mPositionReached = true;
      }
  }
  if (mSleepState.StateFit(ContState::ARC_CLOCKWISE))
  {
    Serial.println("moving: arc cw");
    mPositionReached = true;
  }
  if (mSleepState.StateFit(ContState::ARC_COUNTER_CLOCKWISE))
  {
    Serial.println("moving: arc ccw");
    mPositionReached = true;
  }
  


  if (mPositionReached)
  {
    mSleepState.Reset();
     
  }
  return 0;
}


void PositionHandler::SetStepPin(uint8_t pin)
{
  mStepPin = pin;
}

uint8_t PositionHandler::getStepPin()
{
  return mStepPin;
}

uint8_t PositionHandler::InitDrivers()
{

  pinMode(mStepPin, OUTPUT);
  #define T_ENTRY(driverName, index, setDriverPins, rangeFrom, rangeTo, mmpr, m0,m1,m2) \
    if (index >= 0 && index <= 3) { \
      mDrivers[index].setDriverName(driverName); \
      PIN_DEFS::setDriverPins(mDrivers[index]); \
      mDrivers[index].setRange(new Drivers::Range(rangeFrom, rangeTo)); \
      mDrivers[index].setDistancePerRotation(mmpr); \
      mDrivers[index].setStepConfig(new Drivers::StepConfig(m0,m1,m2)); \
      mDrivers[index].Initialize(); \
    }
    DRIVERS_TABLE
  #undef T_ENTRY
  return 1;
}

void PositionHandler::FileOpened()
{
  Serial.println("Position Reached");

  if (!this->getInstance().mPositionReached_cb.Execute())
  {
    Serial.println("callback empty");
  }
}

void PositionHandler::PositionReachedCB(void * cb)
{
  mPositionReached_cb = Callback(cb);
}

bool PositionHandler::PositionReached()
{
  return mPositionReached;
}